#ifndef MP_CLASS_FORMATTER_H_
#define MP_CLASS_FORMATTER_H_

#include <py/runtime.h>
#include <py/obj.h>
#include <mpconfigport.h>

extern const mp_obj_type_t formatter_type;
extern mp_obj_t formatter_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args);

typedef struct _formatter_class_obj_t {
    mp_obj_base_t base;
    mp_obj_t format;
    bool colored;
} formatter_class_obj_t;

#endif