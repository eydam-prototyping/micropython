#include <py/runtime.h>
#include <py/obj.h>
#include <mp_class_logger.h>
#include <mp_class_formatter.h>
#include <mp_class_handler.h>

STATIC const mp_rom_map_elem_t logging_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_logging) },
    { MP_ROM_QSTR(MP_QSTR_Logger), (mp_obj_t)&logger_type},
    { MP_ROM_QSTR(MP_QSTR_LogFormatter), (mp_obj_t)&formatter_type},
    { MP_ROM_QSTR(MP_QSTR_LogHandler), (mp_obj_t)&handler_type},
    { MP_ROM_QSTR(MP_QSTR_LogToConsole), MP_ROM_INT(LOG_TO_CONSOLE) },
    { MP_ROM_QSTR(MP_QSTR_LogToFile), MP_ROM_INT(LOG_TO_FILE) },
    { MP_ROM_QSTR(MP_QSTR_LogToStream), MP_ROM_INT(LOG_TO_STREAM) },
    { MP_ROM_QSTR(MP_QSTR_LogDebug), MP_ROM_INT(LOG_DEBUG) },
    { MP_ROM_QSTR(MP_QSTR_LogInfo), MP_ROM_INT(LOG_INFO) },
    { MP_ROM_QSTR(MP_QSTR_LogNotice), MP_ROM_INT(LOG_NOTICE) },
    { MP_ROM_QSTR(MP_QSTR_LogWarning), MP_ROM_INT(LOG_WARNING) },
    { MP_ROM_QSTR(MP_QSTR_LogError), MP_ROM_INT(LOG_ERROR) },
    { MP_ROM_QSTR(MP_QSTR_LogCritical), MP_ROM_INT(LOG_CRITICAL) },
    { MP_ROM_QSTR(MP_QSTR_LogAlarm), MP_ROM_INT(LOG_ALARM) },
    { MP_ROM_QSTR(MP_QSTR_LogEmergency), MP_ROM_INT(LOG_EMERGENCY) },
};

STATIC MP_DEFINE_CONST_DICT(logging_module_globals, logging_module_globals_table);

const mp_obj_module_t logging_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&logging_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_logging, logging_cmodule, MODULE_EPMODULES_ENABLED);