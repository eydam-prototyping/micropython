#ifndef MP_CLASS_HANDLER_H_
#define MP_CLASS_HANDLER_H_

#include <py/runtime.h>
#include <py/obj.h>
#include <mpconfigport.h>
#include <py/builtin.h>
#include <py/stream.h>

#include <mp_class_formatter.h>

#define LOG_TO_CONSOLE          0
#define LOG_TO_FILE             1
#define LOG_TO_STREAM           2

extern const mp_obj_type_t handler_type;
extern mp_obj_t handler_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args);

typedef struct _handler_class_obj_t{
    mp_obj_base_t base;
    uint8_t dest;
    mp_obj_t filename;
    mp_obj_t file;
    uint8_t level;
    formatter_class_obj_t *formatter;
    mp_obj_t stream;
} handler_class_obj_t;

#endif