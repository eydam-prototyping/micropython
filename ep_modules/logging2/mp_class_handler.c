#include <mp_class_handler.h>

/** 
 * constructor for handler
 * args are: 
 *   formatter
 *   dest (LOG_TO_CONSOLE, LOG_TO_FILE, LOG_TO_STREAM)
 *   filename, required if dest is LOG_TO_FILE
 *   stream, required if dest is LOG_TO_STREAM
 */ 
mp_obj_t handler_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    mp_arg_check_num(n_args, n_kw, 0, 3, true);

    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_formatter, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = MP_OBJ_NULL} },
        { MP_QSTR_dest, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = LOG_TO_CONSOLE} },
        { MP_QSTR_filename, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = MP_OBJ_NULL} },
        { MP_QSTR_stream, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_rom_obj = MP_OBJ_NULL} },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // create new handler-object
    handler_class_obj_t *self = m_new_obj(handler_class_obj_t);
    self->base.type = &handler_type;
    
    // create new formatter, if no one is passed
    if (args[0].u_obj == MP_OBJ_NULL){
        self->formatter = formatter_make_new(MP_OBJ_NULL, 0,0, MP_OBJ_NULL);
    }else if (mp_obj_is_type(args[0].u_obj, &formatter_type)){
        self->formatter = (formatter_class_obj_t*)MP_OBJ_TO_PTR(args[0].u_obj);
    }else{
        mp_raise_TypeError("Formatter must be of type LogFormatter");
    }

    self->dest = (uint8_t)args[1].u_int;
    if (self->dest == LOG_TO_FILE){
        self->filename = args[2].u_obj;
        if (args[2].u_obj == MP_OBJ_NULL){
            self->filename = mp_obj_new_str("log.txt", 7);
        }
        mp_obj_t args[2] = {
            self->filename,
            MP_OBJ_NEW_QSTR(MP_QSTR_at),
        };
        self->file = mp_builtin_open(MP_ARRAY_SIZE(args), &args[0], (mp_map_t *)&mp_const_empty_map);
    }

    if (self->dest == LOG_TO_STREAM){
        self->stream = args[3].u_obj;
        if (args[3].u_obj == MP_OBJ_NULL){
            mp_raise_ValueError("Stream is required");
        }
    }

    self->level = 7;
    return MP_OBJ_FROM_PTR(self);
}

/**
 * print function for handler
 */
STATIC void handler_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    handler_class_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_print_str(print, "LogHandler(dest=");
    if (self->dest == LOG_TO_CONSOLE){
        mp_print_str(print, "LogToConsole");    
    }
    if (self->dest == LOG_TO_FILE){
        mp_print_str(print, "LogToFile, filename=");    
        mp_obj_print_helper(print, self->filename, PRINT_REPR);
    }
    if (self->dest == LOG_TO_STREAM){
        mp_print_str(print, "LogToStream");
    }
    mp_print_str(print, ")");
}

/**
 * attributes for handler:
 *   formatter (get/set)
 *   level (get/set)
 *   filename (get)
 *   stream (get)
 */
STATIC void handler_attr(mp_obj_t self_in, qstr attribute, mp_obj_t *destination) {
    handler_class_obj_t *self = MP_OBJ_TO_PTR(self_in);

    // set value
    if (destination[0] != MP_OBJ_NULL) {

        if(attribute == MP_QSTR_formatter) {
            if (mp_obj_is_type(destination[1], &formatter_type)){
                formatter_class_obj_t *formatter = MP_OBJ_TO_PTR(destination[1]);
                self->formatter = formatter;
            }else{
                mp_raise_TypeError("Formatter must be of type LogFormatter");
            }
            destination[0] = MP_OBJ_NULL; // indicate success
        }

        if(attribute == MP_QSTR_logLevel) {
            self->level = mp_obj_get_int(destination[1]);
            destination[0] = MP_OBJ_NULL; // indicate success
        }

        return;

    }

    // get value
    if(attribute == MP_QSTR_formatter) {
        destination[0] = self->formatter;
    }

    if(attribute == MP_QSTR_dest) {
        destination[0] = mp_obj_new_int(self->dest);
    }

    if(attribute == MP_QSTR_logLevel) {
        destination[0] = mp_obj_new_int(self->level);
    }

    if(attribute == MP_QSTR_stream) {
        if (self->dest == LOG_TO_STREAM){ 
            destination[0] = self->stream;
        }else{
            destination[0] = mp_const_none;
        }
    }
    
    if(attribute == MP_QSTR_filename) {
        if (self->dest == LOG_TO_FILE){ 
            destination[0] = self->filename;
        }else{
            destination[0] = mp_const_none;
        }
    }
}

STATIC const mp_rom_map_elem_t handler_locals_dict_table[] = {
};

STATIC MP_DEFINE_CONST_DICT(handler_locals_dict, handler_locals_dict_table);

const mp_obj_type_t handler_type = {
    { &mp_type_type },
    .flags = 0,
    .name = MP_QSTR_LogHandler,
    .print = handler_print,
    .make_new = handler_make_new,
    .attr = handler_attr,
    .locals_dict = (mp_obj_dict_t*)&handler_locals_dict,
};