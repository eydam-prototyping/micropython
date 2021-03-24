#include "py/runtime.h"

extern mp_obj_t logger___init__(const char* hostname, const char* appname, bool colored);
extern mp_obj_t logger___str__(mp_obj_t logger);
extern void logger_log(mp_obj_t logger, const char* message, int severity);
extern void logger_addHandler(mp_obj_t logger, mp_obj_t handler);

extern mp_obj_t fileStreamHandler___init__(const char* filename);
extern mp_obj_t fileStreamHandler___str__(mp_obj_t logger);