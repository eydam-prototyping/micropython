#include <mp_class_logger.h>

STATIC mp_obj_t logger_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    mp_arg_check_num(n_args, n_kw, 0, 3, true);

    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_hostname, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = MP_OBJ_NULL} },
        { MP_QSTR_appname, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = MP_OBJ_NULL} },
        { MP_QSTR_handler, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = MP_OBJ_NULL} },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    logger_class_obj_t *self = m_new_obj(logger_class_obj_t);
    self->base.type = &logger_type;
    
    if (args[0].u_obj == MP_OBJ_NULL){
        self->hostname = mp_obj_new_str(MICROPY_PY_SYS_PLATFORM, sizeof(MICROPY_PY_SYS_PLATFORM)-1);
    }else{
        self->hostname = args[0].u_obj;
    }

    if (args[1].u_obj == MP_OBJ_NULL){
        self->appname = mp_obj_new_str("micropython", sizeof("micropython")-1);
    }else{
        self->appname = args[1].u_obj;
    }

    self->handlers = mp_obj_new_list(0, NULL);
    if (args[2].u_obj == MP_OBJ_NULL){
        mp_obj_list_append(self->handlers, handler_make_new(MP_OBJ_NULL, 0,0, MP_OBJ_NULL));
    }else{
        mp_obj_list_append(self->handlers, args[2].u_obj);
    }
    return MP_OBJ_FROM_PTR(self);
}

STATIC void logger_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    logger_class_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_print_str(print, "Logger(hostname=");
    mp_obj_print_helper(print, self->hostname, PRINT_REPR);
    mp_print_str(print, ", appname=");
    mp_obj_print_helper(print, self->appname, PRINT_REPR);
    mp_print_str(print, ")");
}

STATIC mp_obj_t logger_addHandler(mp_obj_t self_in, mp_obj_t handler_in) {
    logger_class_obj_t *self = MP_OBJ_TO_PTR(self_in);
    if (mp_obj_is_type(handler_in, &handler_type)){
        handler_class_obj_t *handler = MP_OBJ_TO_PTR(handler_in);
        mp_obj_list_append(self->handlers, handler);
    }else{
        //mp_raise_TypeError("Only LogHandlers allowed");
    }
    return mp_const_none;
}

STATIC mp_obj_t logger_debug(mp_obj_t self_in, mp_obj_t message) {
    handle_message(MP_OBJ_TO_PTR(self_in), message, 7);
    return mp_const_none;
}

STATIC mp_obj_t logger_info(mp_obj_t self_in, mp_obj_t message) {
    handle_message(MP_OBJ_TO_PTR(self_in), message, 6);
    return mp_const_none;
}

STATIC mp_obj_t logger_notice(mp_obj_t self_in, mp_obj_t message) {
    handle_message(MP_OBJ_TO_PTR(self_in), message, 5);
    return mp_const_none;
}

STATIC mp_obj_t logger_warning(mp_obj_t self_in, mp_obj_t message) {
    handle_message(MP_OBJ_TO_PTR(self_in), message, 4);
    return mp_const_none;
}

STATIC mp_obj_t logger_error(mp_obj_t self_in, mp_obj_t message) {
    handle_message(MP_OBJ_TO_PTR(self_in), message, 3);
    return mp_const_none;
}

STATIC mp_obj_t logger_critical(mp_obj_t self_in, mp_obj_t message) {
    handle_message(MP_OBJ_TO_PTR(self_in), message, 2);
    return mp_const_none;
}

STATIC mp_obj_t logger_alarm(mp_obj_t self_in, mp_obj_t message) {
    handle_message(MP_OBJ_TO_PTR(self_in), message, 1);
    return mp_const_none;
}

STATIC mp_obj_t logger_emergency(mp_obj_t self_in, mp_obj_t message) {
    handle_message(MP_OBJ_TO_PTR(self_in), message, 0);
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_2(logger_addHandler_obj, logger_addHandler);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(logger_debug_obj, logger_debug);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(logger_info_obj, logger_info);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(logger_notice_obj, logger_notice);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(logger_warning_obj, logger_warning);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(logger_error_obj, logger_error);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(logger_critical_obj, logger_critical);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(logger_alarm_obj, logger_alarm);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(logger_emergency_obj, logger_emergency);

STATIC void logger_attr(mp_obj_t self_in, qstr attribute, mp_obj_t *destination) {
    logger_class_obj_t *self = MP_OBJ_TO_PTR(self_in);
    if (destination[0] != MP_OBJ_NULL) {
        if(attribute == MP_QSTR_hostname) {
            self->hostname = destination[1];
            destination[0] = MP_OBJ_NULL; // indicate success
        }
        if(attribute == MP_QSTR_appname) {
            self->appname = destination[1];
            destination[0] = MP_OBJ_NULL; // indicate success
        }
        return;
    }
    if(attribute == MP_QSTR_hostname) {
        destination[0] = self->hostname;
    }
    if(attribute == MP_QSTR_appname) {
        destination[0] = self->appname;
    }
    if(attribute == MP_QSTR_handlers) {
        destination[0] = self->handlers;
    }
    if(attribute == MP_QSTR_addHandler) {
        destination[0] = MP_OBJ_FROM_PTR(&logger_addHandler_obj);
        destination[1] = self_in;
    }
    if(attribute == MP_QSTR_log_debug) {
        destination[0] = MP_OBJ_FROM_PTR(&logger_debug_obj);
        destination[1] = self_in;
    }
    if(attribute == MP_QSTR_log_info) {
        destination[0] = MP_OBJ_FROM_PTR(&logger_info_obj);
        destination[1] = self_in;
    }
    if(attribute == MP_QSTR_log_notice) {
        destination[0] = MP_OBJ_FROM_PTR(&logger_notice_obj);
        destination[1] = self_in;
    }
    if(attribute == MP_QSTR_log_warning) {
        destination[0] = MP_OBJ_FROM_PTR(&logger_warning_obj);
        destination[1] = self_in;
    }
    if(attribute == MP_QSTR_log_error) {
        destination[0] = MP_OBJ_FROM_PTR(&logger_error_obj);
        destination[1] = self_in;
    }
    if(attribute == MP_QSTR_log_critical) {
        destination[0] = MP_OBJ_FROM_PTR(&logger_critical_obj);
        destination[1] = self_in;
    }
    if(attribute == MP_QSTR_log_alarm) {
        destination[0] = MP_OBJ_FROM_PTR(&logger_alarm_obj);
        destination[1] = self_in;
    }
    if(attribute == MP_QSTR_log_emergency) {
        destination[0] = MP_OBJ_FROM_PTR(&logger_emergency_obj);
        destination[1] = self_in;
    }
}


STATIC const mp_rom_map_elem_t logger_locals_dict_table[] = {
};

STATIC MP_DEFINE_CONST_DICT(logger_locals_dict, logger_locals_dict_table);

const mp_obj_type_t logger_type = {
    { &mp_type_type },
    .flags = 0,
    .name = MP_QSTR_Logger,
    .print = logger_print,
    .make_new = logger_make_new,
    .attr = logger_attr,
    .locals_dict = (mp_obj_dict_t*)&logger_locals_dict,
};