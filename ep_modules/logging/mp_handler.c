#include <mp_loggermodule.h>
#include <log_c_functions.h>

#include <mpconfigport.h>

STATIC void fileStreamHandler_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    logger_class_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_obj_t repr = fileStreamHandler___str__(self->obj);
    mp_obj_print_helper(print, repr, PRINT_REPR);
}

STATIC mp_obj_t fileStreamHandler_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    mp_arg_check_num(n_args, n_kw, 0, 1, true);

    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_filename, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = MP_OBJ_NULL} },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    mp_obj_t mp_filename = args[0].u_obj;
    char* s_filename = "logger_%Y-%m-%d.log";
    
    if (mp_filename != MP_OBJ_NULL){
        s_filename = (char*)mp_obj_str_get_str(mp_filename);
    }

    logger_class_obj_t *self = m_new_obj(logger_class_obj_t);
    self->base.type = &fileStreamHandler_type;
    self->obj = (mp_obj_t)fileStreamHandler___init__(s_filename);
    return MP_OBJ_FROM_PTR(self);
}

STATIC const mp_rom_map_elem_t handler_locals_dict_table[] = {
};

STATIC MP_DEFINE_CONST_DICT(handler_locals_dict, handler_locals_dict_table);

const mp_obj_type_t fileStreamHandler_type = {
    { &mp_type_type },
    .flags = 0,
    .name = MP_QSTR_FileStreamHandler,
    .print = fileStreamHandler_print,
    .make_new = fileStreamHandler_make_new,
    .locals_dict = (mp_obj_dict_t*)&handler_locals_dict,
};