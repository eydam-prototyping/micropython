/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 "Eric Poulsen" <eric@zyxod.com>
 *
 * Based on the ESP IDF example code which is Public Domain / CC0
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include "py/runtime.h"
#include "py/mphal.h"

//#include "eth_phy/phy.h"
//#include "eth_phy/phy_tlk110.h"
//#include "eth_phy/phy_lan8720.h"
//#include "eth_phy/phy_ip101.h"
//#include "tcpip_adapter.h"

#include "esp_netif.h"
#include "esp_eth.h"
#include "esp_event.h"
#include "esp_log.h"

#include "modnetwork.h"

typedef struct _lan_if_obj_t {
    mp_obj_base_t base;
    int if_id; // MUST BE FIRST to match wlan_if_obj_t
    bool initialized;
    bool active;
    uint8_t mdc_pin;
    uint8_t mdio_pin;
    int8_t phy_power_pin;
    uint8_t phy_addr;
    uint8_t phy_type;
    esp_eth_phy_t *phy;
    esp_netif_t *eth_netif;
    esp_eth_handle_t eth_handle;
} lan_if_obj_t;

const mp_obj_type_t lan_if_type;
STATIC lan_if_obj_t lan_obj = {{&lan_if_type}, ESP_IF_ETH, false, false};
/*
STATIC void phy_power_enable(bool enable) {
    lan_if_obj_t *self = &lan_obj;

    if (self->phy_power_pin != -1) {

        if (!enable) {
            // Do the PHY-specific power_enable(false) function before powering down
            self->power_func(false);
        }

        gpio_pad_select_gpio(self->phy_power_pin);
        gpio_set_direction(self->phy_power_pin, GPIO_MODE_OUTPUT);
        if (enable) {
            gpio_set_level(self->phy_power_pin, 1);
        } else {
            gpio_set_level(self->phy_power_pin, 0);
        }

        // Allow the power up/down to take effect, min 300us
        vTaskDelay(1);

        if (enable) {
            // Run the PHY-specific power on operations now the PHY has power
            self->power_func(true);
        }
    }
}

STATIC void init_lan_rmii() {
    lan_if_obj_t *self = &lan_obj;
    phy_rmii_configure_data_interface_pins();
    phy_rmii_smi_configure_pins(self->mdc_pin, self->mdio_pin);
}
*/



static const char *TAG = "eth_example";

/** Event handler for Ethernet events */
static void eth_event_handler(void *arg, esp_event_base_t event_base,
                              int32_t event_id, void *event_data)
{
    //uint8_t mac_addr[6] = {0};
    /* we can get the ethernet driver handle from event data */
    esp_eth_handle_t eth_handle = *(esp_eth_handle_t *)event_data;

    switch (event_id) {
        case ETHERNET_EVENT_CONNECTED:
            //esp_eth_ioctl(eth_handle, ETH_CMD_G_MAC_ADDR, mac_addr);
            ESP_LOGI(TAG, "Ethernet Link Up");
            //ESP_LOGW(TAG, "Ethernet HW Addr %02x:%02x:%02x:%02x:%02x:%02x",
            //        mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
            break;
        case ETHERNET_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "Ethernet Link Down");
            break;
        case ETHERNET_EVENT_START:
            ESP_LOGI(TAG, "Ethernet Started");
            break;
        case ETHERNET_EVENT_STOP:
            ESP_LOGI(TAG, "Ethernet Stopped");
            break;
        default:
            break;
    }
}

/** Event handler for IP_EVENT_ETH_GOT_IP 
static void got_ip_event_handler(void *arg, esp_event_base_t event_base,
                                 int32_t event_id, void *event_data)
{
    ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
    const esp_netif_ip_info_t *ip_info = &event->ip_info;

    ESP_LOGI(TAG, "Ethernet Got IP Address");
    ESP_LOGI(TAG, "~~~~~~~~~~~");
    ESP_LOGI(TAG, "ETHIP:" IPSTR, IP2STR(&ip_info->ip));
    ESP_LOGI(TAG, "ETHMASK:" IPSTR, IP2STR(&ip_info->netmask));
    ESP_LOGI(TAG, "ETHGW:" IPSTR, IP2STR(&ip_info->gw));
    ESP_LOGI(TAG, "~~~~~~~~~~~");
}*/


STATIC mp_obj_t get_lan(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    lan_if_obj_t *self = &lan_obj;

    if (self->initialized) {
        return MP_OBJ_FROM_PTR(&lan_obj);
    }
    
    enum { ARG_id, ARG_mdc, ARG_mdio, ARG_power, ARG_phy_addr, ARG_phy_type, ARG_clock_mode };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_id,           MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_mdc,          MP_ARG_KW_ONLY | MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_mdio,         MP_ARG_KW_ONLY | MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_power,        MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = mp_const_none} },
        { MP_QSTR_phy_addr,     MP_ARG_KW_ONLY | MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_phy_type,     MP_ARG_KW_ONLY | MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_clock_mode,   MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = -1} },
    };
    
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);
    
    if (args[ARG_id].u_obj != mp_const_none) {
        if (mp_obj_get_int(args[ARG_id].u_obj) != 0) {
            mp_raise_ValueError(MP_ERROR_TEXT("invalid LAN interface identifier"));
        }
    }
    
    self->mdc_pin = machine_pin_get_id(args[ARG_mdc].u_obj);
    self->mdio_pin = machine_pin_get_id(args[ARG_mdio].u_obj);
    self->phy_power_pin = args[ARG_power].u_obj == mp_const_none ? -1 : machine_pin_get_id(args[ARG_power].u_obj);
    
    if (args[ARG_phy_addr].u_int < 0x00 || args[ARG_phy_addr].u_int > 0x1f) {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid phy address"));
    }

    if (args[ARG_phy_type].u_int != PHY_LAN8720 &&
        args[ARG_phy_type].u_int != PHY_TLK110 &&
        args[ARG_phy_type].u_int != PHY_IP101) {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid phy type"));
    }
    
    if (args[ARG_clock_mode].u_int != -1 &&
        args[ARG_clock_mode].u_int != CONFIG_ETH_RMII_CLK_IN_GPIO) {
        // Disabled due ESP-IDF (see modnetwork.c note)
        // args[ARG_clock_mode].u_int != CONFIG_ETH_RMII_CLK_OUTPUT_GPIO0 &&
        //args[ARG_clock_mode].u_int != ETH_CLOCK_GPIO16_OUT &&
        //args[ARG_clock_mode].u_int != ETH_CLOCK_GPIO17_OUT) {
        mp_raise_ValueError(MP_ERROR_TEXT("invalid clock mode"));
    }    

    // Use internal MAC
    eth_mac_config_t mac_config = ETH_MAC_DEFAULT_CONFIG();
    mac_config.smi_mdc_gpio_num = self->mdc_pin;
    mac_config.smi_mdio_gpio_num = self->mdio_pin;
    esp_eth_mac_t *mac = esp_eth_mac_new_esp32(&mac_config);
    printf("get_lan#5\n");

    // 
    eth_phy_config_t phy_config = ETH_PHY_DEFAULT_CONFIG();
    phy_config.phy_addr = self->phy_addr;
    phy_config.reset_gpio_num = self->phy_power_pin;
    self->phy = NULL;

    switch (args[ARG_phy_type].u_int) {
        case PHY_TLK110:
            //config = phy_tlk110_default_ethernet_config;
            mp_raise_ValueError(MP_ERROR_TEXT("TLK110 is not implemented yet."));
            break;
        case PHY_LAN8720:
            //config = phy_lan8720_default_ethernet_config;
            self->phy = esp_eth_phy_new_lan8720(&phy_config);
            break;
        case PHY_IP101:
            //config = phy_ip101_default_ethernet_config;
            self->phy = esp_eth_phy_new_ip101(&phy_config);
            break;
        default:
            mp_raise_ValueError(MP_ERROR_TEXT("Unknown PHY"));
    }
    printf("get_lan#6\n");

    

    //esp_eth_handle_t eth_handle = NULL;

    //self->link_func = self->phy->get_link;
    
    // Replace default power func with our own
    //self->power_func = config.phy_power_enable;
    //config.phy_power_enable = phy_power_enable;
    
    //config.phy_addr = args[ARG_phy_addr].u_int;
    //config.gpio_config = init_lan_rmii;
    //config.tcpip_input = tcpip_adapter_eth_input;
    
    
    //if (args[ARG_clock_mode].u_int != -1) {
    //    config.clock_mode = args[ARG_clock_mode].u_int;
    //}

    ESP_ERROR_CHECK(esp_netif_init());
    //ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_config_t cfg = ESP_NETIF_DEFAULT_ETH();
    self->eth_netif = esp_netif_new(&cfg);

    ESP_ERROR_CHECK(esp_eth_set_default_handlers(self->eth_netif));

    ESP_ERROR_CHECK(esp_event_handler_register(ETH_EVENT, ESP_EVENT_ANY_ID, &eth_event_handler, NULL));
    //ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_ETH_GOT_IP, &got_ip_event_handler, NULL));

    esp_eth_config_t config = ETH_DEFAULT_CONFIG(mac, self->phy);
    
    if (esp_eth_driver_install(&config, &self->eth_handle) == ESP_OK) {
        self->active = false;
        self->initialized = true;
    } else {
        mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("esp_eth_driver_install() failed"));
    }
    printf("get_lan#7\n");

    
    esp_netif_attach(self->eth_netif, esp_eth_new_netif_glue(self->eth_handle));
    
    printf("get_lan#8\n");
    
    return MP_OBJ_FROM_PTR(&lan_obj);
}
MP_DEFINE_CONST_FUN_OBJ_KW(get_lan_obj, 0, get_lan);


STATIC mp_obj_t lan_active(size_t n_args, const mp_obj_t *args) {
    lan_if_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    
    if (n_args > 1) {
        if (mp_obj_is_true(args[1])) {
            
            self->active = (esp_eth_start(self->eth_handle) == ESP_OK);
            if (!self->active) {
                mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("ethernet enable failed"));
            }
        } else {
            self->active = !(esp_eth_stop(self->eth_handle) == ESP_OK);
            if (self->active) {
                mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("ethernet disable failed"));
            }
        }
    }

    return mp_obj_new_bool(self->active);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(lan_active_obj, 1, 2, lan_active);


STATIC mp_obj_t lan_status(mp_obj_t self_in) {
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(lan_status_obj, lan_status);

STATIC mp_obj_t lan_isconnected(mp_obj_t self_in) {
    lan_if_obj_t *self = MP_OBJ_TO_PTR(self_in);
    //esp_eth_ioctl(self->eth_handle, ETH_CMD_G_MAC_ADDR, mac_addr);
    return self->active ? mp_obj_new_bool(self->phy->get_link(self->phy) == ESP_OK) : mp_const_false;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(lan_isconnected_obj, lan_isconnected);

STATIC const mp_rom_map_elem_t lan_if_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_active), MP_ROM_PTR(&lan_active_obj) },
    { MP_ROM_QSTR(MP_QSTR_isconnected), MP_ROM_PTR(&lan_isconnected_obj) },
    { MP_ROM_QSTR(MP_QSTR_status), MP_ROM_PTR(&lan_status_obj) },
    { MP_ROM_QSTR(MP_QSTR_config), MP_ROM_PTR(&esp_config_obj) },
    { MP_ROM_QSTR(MP_QSTR_ifconfig), MP_ROM_PTR(&esp_ifconfig_obj) },
};

STATIC MP_DEFINE_CONST_DICT(lan_if_locals_dict, lan_if_locals_dict_table);

const mp_obj_type_t lan_if_type = {
    { &mp_type_type },
    .name = MP_QSTR_LAN,
    .locals_dict = (mp_obj_dict_t *)&lan_if_locals_dict,
};

