#include "py/runtime.h"
#include <mp_loggermodule.h>

// Define all properties of the module.
// Table entries are key/value pairs of the attribute name (a string)
// and the MicroPython object reference.
// All identifiers and strings are written as MP_QSTR_xxx and will be
// optimized to word-sized integers by the build system (interned strings).
STATIC const mp_rom_map_elem_t ep_logging_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ep_logging) },
    { MP_ROM_QSTR(MP_QSTR_logger), (mp_obj_t)&logger_type},
};

STATIC MP_DEFINE_CONST_DICT(ep_logging_module_globals, ep_logging_module_globals_table);

// Define module object.
const mp_obj_module_t ep_logging_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&ep_logging_module_globals,
};

// Register the module to make it available in Python.
MP_REGISTER_MODULE(MP_QSTR_ep_logging, ep_logging_cmodule, MODULE_EPMODULES_ENABLED);