#include <mp_loggermodule.h>
#include <log_c_functions.h>

#include <mpconfigport.h>
 
STATIC void logger_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    logger_class_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_obj_t repr = logger___str__(self->obj);
    mp_obj_print_helper(print, repr, PRINT_REPR);
}

STATIC mp_obj_t logger_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    mp_arg_check_num(n_args, n_kw, 0, 1, true);

    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_hostname, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = MP_OBJ_NULL} },
        { MP_QSTR_appname, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = MP_OBJ_NULL} },
        { MP_QSTR_colored, MP_ARG_KW_ONLY | MP_ARG_BOOL, {.u_bool = false} },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    mp_obj_t mp_hostname = args[0].u_obj;
    char* s_hostname = MICROPY_PY_SYS_PLATFORM;
    
    if (mp_hostname != MP_OBJ_NULL){
        s_hostname = (char*)mp_obj_str_get_str(mp_hostname);
    }

    mp_obj_t mp_appname = args[1].u_obj;
    char* s_appname = "micropython";
    
    if (mp_appname != MP_OBJ_NULL){
        s_appname = (char*)mp_obj_str_get_str(s_appname);
    }

    bool colored = args[2].u_bool;

    logger_class_obj_t *self = m_new_obj(logger_class_obj_t);
    self->base.type = &logger_type;
    self->obj = (mp_obj_t)logger___init__(s_hostname, s_appname, colored);
    return MP_OBJ_FROM_PTR(self);
}

mp_obj_t logger_debug(mp_obj_t self_in, mp_obj_t message) {
    logger_class_obj_t *self = MP_OBJ_TO_PTR(self_in);
    logger_log(self->obj, mp_obj_str_get_str(message), 7); 
    return mp_const_none;
}

mp_obj_t logger_info(mp_obj_t self_in, mp_obj_t message) {
    logger_class_obj_t *self = MP_OBJ_TO_PTR(self_in);
    logger_log(self->obj, mp_obj_str_get_str(message), 6); 
    return mp_const_none;
}

mp_obj_t logger_notice(mp_obj_t self_in, mp_obj_t message) {
    logger_class_obj_t *self = MP_OBJ_TO_PTR(self_in);
    logger_log(self->obj, mp_obj_str_get_str(message), 5); 
    return mp_const_none;
}

mp_obj_t logger_warning(mp_obj_t self_in, mp_obj_t message) {
    logger_class_obj_t *self = MP_OBJ_TO_PTR(self_in);
    logger_log(self->obj, mp_obj_str_get_str(message), 4); 
    return mp_const_none;
}

mp_obj_t logger_error(mp_obj_t self_in, mp_obj_t message) {
    logger_class_obj_t *self = MP_OBJ_TO_PTR(self_in);
    logger_log(self->obj, mp_obj_str_get_str(message), 3); 
    return mp_const_none;
}

mp_obj_t logger_critical(mp_obj_t self_in, mp_obj_t message) {
    logger_class_obj_t *self = MP_OBJ_TO_PTR(self_in);
    logger_log(self->obj, mp_obj_str_get_str(message), 2); 
    return mp_const_none;
}

mp_obj_t logger_alarm(mp_obj_t self_in, mp_obj_t message) {
    logger_class_obj_t *self = MP_OBJ_TO_PTR(self_in);
    logger_log(self->obj, mp_obj_str_get_str(message), 1); 
    return mp_const_none;
}

mp_obj_t logger_emergency(mp_obj_t self_in, mp_obj_t message) {
    logger_class_obj_t *self = MP_OBJ_TO_PTR(self_in);
    logger_log(self->obj, mp_obj_str_get_str(message), 0); 
    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_2(logger_debug_obj, logger_debug);
MP_DEFINE_CONST_FUN_OBJ_2(logger_info_obj, logger_info);
MP_DEFINE_CONST_FUN_OBJ_2(logger_notice_obj, logger_notice);
MP_DEFINE_CONST_FUN_OBJ_2(logger_warning_obj, logger_warning);
MP_DEFINE_CONST_FUN_OBJ_2(logger_error_obj, logger_error);
MP_DEFINE_CONST_FUN_OBJ_2(logger_critical_obj, logger_critical);
MP_DEFINE_CONST_FUN_OBJ_2(logger_alarm_obj, logger_alarm);
MP_DEFINE_CONST_FUN_OBJ_2(logger_emergency_obj, logger_emergency);

STATIC const mp_rom_map_elem_t logger_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_ldebug), MP_ROM_PTR(&logger_debug_obj) },
    { MP_ROM_QSTR(MP_QSTR_linfo), MP_ROM_PTR(&logger_info_obj) },
    { MP_ROM_QSTR(MP_QSTR_lnotice), MP_ROM_PTR(&logger_notice_obj) },
    { MP_ROM_QSTR(MP_QSTR_lwarning), MP_ROM_PTR(&logger_warning_obj) },
    { MP_ROM_QSTR(MP_QSTR_lerror), MP_ROM_PTR(&logger_error_obj) },
    { MP_ROM_QSTR(MP_QSTR_lcritical), MP_ROM_PTR(&logger_critical_obj) },
    { MP_ROM_QSTR(MP_QSTR_lalarm), MP_ROM_PTR(&logger_alarm_obj) },
    { MP_ROM_QSTR(MP_QSTR_lemergency), MP_ROM_PTR(&logger_emergency_obj) },
};

STATIC MP_DEFINE_CONST_DICT(logger_locals_dict, logger_locals_dict_table);

const mp_obj_type_t logger_type = {
    { &mp_type_type },
    .flags = 0,
    .name = MP_QSTR_Logger,
    .print = logger_print,
    .make_new = logger_make_new,
    .locals_dict = (mp_obj_dict_t*)&logger_locals_dict,
};
