#ifndef MP_CLASS_LOGGER_H_
#define MP_CLASS_LOGGER_H_

#include <py/runtime.h>
#include <py/obj.h>
#include <mpconfigport.h>
#include <mp_class_handler.h>
#include <c_funcs_logger.h>

#define LOG_DEBUG               7
#define LOG_INFO                6
#define LOG_NOTICE              5
#define LOG_WARNING             4
#define LOG_ERROR               3
#define LOG_CRITICAL            2
#define LOG_ALARM               1
#define LOG_EMERGENCY           0

extern const mp_obj_type_t logger_type;

typedef struct _logger_class_obj_t {
    mp_obj_base_t base;
    mp_obj_t hostname;
    mp_obj_t appname;
    uint8_t facility;
    uint16_t message_counter;
    mp_obj_t handlers;
} logger_class_obj_t;

#endif