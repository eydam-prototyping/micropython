#ifndef C_FUNCS_LOGGER_H_
#define C_FUNCS_LOGGER_H_

#include <py/runtime.h>
#include <py/obj.h>
#include <time.h>
#include <mp_class_logger.h>
#include <mp_class_handler.h>
#include <mp_class_formatter.h>
#include <mpconfigport.h>
#include <extmod/vfs_posix.h>

typedef enum _LogLevel{ 
    llEmergency,
    llAlarm, 
    llCritical, 
    llWarning, 
    llError, 
    llNotice, 
    llInfo, 
    llDebug
} LogLevel;

typedef struct _message_t {
    mp_obj_t message;
    LogLevel severity;
    uint8_t facility;
    time_t timestamp;
    mp_obj_t hostname;
    mp_obj_t appname;
    uint16_t procid;
    uint16_t msgid;
} message_t;

extern void handle_message(mp_obj_t logger_in, mp_obj_t message_in, LogLevel lvl);

#endif