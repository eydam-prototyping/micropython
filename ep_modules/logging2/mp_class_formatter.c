#include <mp_class_formatter.h>

mp_obj_t formatter_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    mp_arg_check_num(n_args, n_kw, 0, 2, true);

    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_format, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = MP_OBJ_NULL} },
        { MP_QSTR_colored, MP_ARG_KW_ONLY | MP_ARG_BOOL, {.u_bool = false} },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    formatter_class_obj_t *self = m_new_obj(formatter_class_obj_t);
    self->base.type = &formatter_type;

    if (args[0].u_obj == MP_OBJ_NULL){
        self->format = mp_obj_new_str("protocol23format", sizeof("protocol23format")-1);
    }else{
        self->format = args[0].u_obj;
    }

    self->colored = args[1].u_bool;
    

    return MP_OBJ_FROM_PTR(self);
}

STATIC void formatter_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    formatter_class_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_print_str(print, "LogFormatter(format=");
    mp_obj_print_helper(print, self->format, PRINT_REPR);
    mp_print_str(print, ", colored=");
    mp_obj_print_helper(print, mp_obj_new_bool(self->colored), PRINT_REPR);
    mp_print_str(print, ")");
}

STATIC void formatter_attr(mp_obj_t self_in, qstr attribute, mp_obj_t *destination) {
    formatter_class_obj_t *self = MP_OBJ_TO_PTR(self_in);
    if (destination[0] != MP_OBJ_NULL) {
        if(attribute == MP_QSTR_format) {
            self->format = destination[1];
            destination[0] = MP_OBJ_NULL; // indicate success
        }
        if(attribute == MP_QSTR_colored) {
            self->colored = destination[1] == mp_const_true;
            destination[0] = MP_OBJ_NULL; // indicate success
        }
        return;
    }
    if(attribute == MP_QSTR_format) {
        destination[0] = self->format;
    }
    if(attribute == MP_QSTR_colored) {
        destination[0] = mp_obj_new_bool(self->colored);
    }
}

STATIC const mp_rom_map_elem_t formatter_locals_dict_table[] = {
};

STATIC MP_DEFINE_CONST_DICT(formatter_locals_dict, formatter_locals_dict_table);

const mp_obj_type_t formatter_type = {
    { &mp_type_type },
    .flags = 0,
    .name = MP_QSTR_LogFormatter,
    .print = formatter_print,
    .make_new = formatter_make_new,
    .attr = formatter_attr,
    .locals_dict = (mp_obj_dict_t*)&formatter_locals_dict,
};