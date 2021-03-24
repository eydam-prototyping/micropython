#include "py/runtime.h"
#include <stdio.h>
#include "py/obj.h"

extern const mp_obj_type_t logger_type;
extern const mp_obj_type_t fileStreamHandler_type;

typedef struct _logger_class_obj_t {
    mp_obj_base_t base;
    mp_obj_t obj;
} logger_class_obj_t;
