#include "py/runtime.h"

extern mp_obj_t logger___init__(const char* hostname, const char* appname, bool colored);
extern mp_obj_t logger___str__(mp_obj_t logger);

extern void logger_log(void* logger, const char* message, int severity);
