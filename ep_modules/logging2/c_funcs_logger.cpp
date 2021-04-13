#include <string>
#include <ctime>

extern "C"{
#include <c_funcs_logger.h>
#include <string.h>
#include <extmod/vfs.h>
#include <py/stream.h>
#include <mpconfigport.h>
#include <py/builtin.h>

const char* _logLevelToColor(LogLevel a_ll){
    switch (a_ll)
    {
    case 0: return "\033[41m"; break;
    case 1: return "\033[30;41m"; break;
    case 2: return "\033[31;43m"; break;
    case 3: return "\033[31m"; break;
    case 4: return "\033[33m"; break;
    case 5: return "\033[36m"; break;
    case 6: return "\033[37m"; break;
    case 7: return "\033[90m"; break;
    default:return "UNKNOWN"; break;
    }    
}

const char* _logLevelToString(LogLevel a_ll){
    switch (a_ll)
    {
    case 0: return "EMERGENCY"; break;
    case 1: return "ALARM    "; break;
    case 2: return "CRITICAL "; break;
    case 3: return "ERROR    "; break;
    case 4: return "WARNING  "; break;
    case 5: return "NOTICE   "; break;
    case 6: return "INFO     "; break;
    case 7: return "DEBUG    "; break;
    default:return "UNKNOWN"; break;
    }    
}

std::string format_message(message_t *msg, formatter_class_obj_t *fmt){
    std::string result = "UNKNOWN FORMAT";
    char buffer[32];
    std::strftime(buffer, 32, "%Y-%m-%dT%H:%M:%SZ", std::localtime(&(msg->timestamp)));
    
    if (strcmp(mp_obj_str_get_str(fmt->format),"protocol23format")==0){
        result  = "<" + std::to_string(msg->facility * 8 + msg->severity) + ">";
        if (fmt->colored){result += "\033[34m";}
        result += " " + (std::string)buffer;
        if (fmt->colored){result += "\033[0m";}
        result += " " + (std::string)mp_obj_str_get_str(msg->hostname);
        if (fmt->colored){result += "\033[32m";}
        result += " " + (std::string)mp_obj_str_get_str(msg->appname);
        if (fmt->colored){result += "\033[0m";}
        result += " " + std::to_string(msg->procid);
        result += " " + std::to_string(msg->msgid) + " ";
        if (fmt->colored){result += _logLevelToColor(msg->severity);}
        result += (std::string)mp_obj_str_get_str(msg->message);
        if (fmt->colored){result += "\033[0m";}
    }else if (strcmp(mp_obj_str_get_str(fmt->format),"standard")==0){
        result = "";
        if (fmt->colored){result += "\033[90m";}
        result  += "[" + (std::string)buffer + "] ";
        if (fmt->colored){result += "\033[0m";}

        if (fmt->colored){result += _logLevelToColor(msg->severity);}
        result += (std::string)_logLevelToString(msg->severity);
        if (fmt->colored){result += "\033[0m";}
        result  += " ";

        if (fmt->colored){result += "\033[0m";}
        result += " " + (std::string)mp_obj_str_get_str(msg->hostname);
        if (fmt->colored){result += "\033[32m";}
        result += " " + (std::string)mp_obj_str_get_str(msg->appname);
        if (fmt->colored){result += "\033[0m";}
        result += " " + std::to_string(msg->procid);
        result += " " + std::to_string(msg->msgid) + " ";
        
        result += (std::string)mp_obj_str_get_str(msg->message);
        
    }
    return result + "\n";
}


void handle_message(mp_obj_t logger_in, mp_obj_t message_in, LogLevel lvl){
    logger_class_obj_t *logger = (logger_class_obj_t*)MP_OBJ_TO_PTR(logger_in);

    message_t m;
    m.appname = logger->appname;
    m.facility = 16;
    m.hostname = logger->hostname;
    m.message = message_in;
    m.msgid = logger->message_counter++;
    m.procid = 0;
    m.timestamp = time(NULL);
    m.severity = lvl;

    mp_obj_iter_buf_t iter_buf;
    mp_obj_t iterable = mp_getiter(logger->handlers, &iter_buf);
    handler_class_obj_t *handler;

    while ((handler = (handler_class_obj_t*)mp_iternext(iterable)) != MP_OBJ_STOP_ITERATION){
        if (handler->level >= m.severity){
            if (handler->dest == LOG_TO_CONSOLE){
                printf("%s",format_message(&m, handler->formatter).c_str());
            }else if (handler->dest == LOG_TO_FILE){
                int errcode;
                std::string s_msg = format_message(&m, handler->formatter).c_str();
                char buf[s_msg.length()];
                strncpy(buf,s_msg.c_str(),s_msg.length());
                buf[s_msg.length()];
                mp_stream_rw(handler->file, (byte*)buf, sizeof(buf), &errcode, MP_STREAM_RW_WRITE);
                if (errcode != 0) {
                    mp_raise_OSError(errcode);
                } 
            }else if (handler->dest == LOG_TO_STREAM){
                int errcode;
                std::string s_msg = format_message(&m, handler->formatter).c_str();
                char buf[s_msg.length()];
                strncpy(buf,s_msg.c_str(),s_msg.length());
                buf[s_msg.length()];
                mp_stream_rw(handler->stream, (byte*)buf, sizeof(buf), &errcode, MP_STREAM_RW_WRITE);
                if (errcode != 0) {
                    mp_raise_OSError(errcode);
                } 
            }
            
        }
    }
}

}