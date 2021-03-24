#include <log_logger.hpp>
#include <log_formatter.hpp>
#include <log_handler.hpp>
#include <iostream>

extern "C"{
    #include <log_c_functions.h>

    mp_obj_t logger___init__(const char* hostname, const char* appname, bool colored){
        LogFormatter* lf = new LogFormatter(colored);
        StreamLogHandler* slh = new StreamLogHandler();
        BasicLogger* logger = new BasicLogger(hostname, appname);

        logger->setFormatter(lf);
        logger->addHandler(slh);

        return logger;
    }

    mp_obj_t logger___str__(mp_obj_t logger){
        BasicLogger* l = (BasicLogger*) logger;
        std::string repr = l->to_string();
        return mp_obj_new_str(repr.c_str(), repr.length());
    }

    void logger_log(mp_obj_t logger, const char* message, int severity){
        BasicLogger* l = (BasicLogger*) logger;
        l->log((std::string)message, (LogLevel)severity);
    }

    void logger_addHandler(mp_obj_t logger, mp_obj_t handler){
        BasicLogger* l = (BasicLogger*) logger;
        BasicLogHandler* h = (BasicLogHandler*) handler;
        l->addHandler(h);
    }

    mp_obj_t fileStreamHandler___init__(const char* filename){
        FileStreamLogHandler* fslh = new FileStreamLogHandler(filename);
        return fslh;
    }

    mp_obj_t fileStreamHandler___str__(mp_obj_t logger){
        BasicLogHandler* l = (BasicLogHandler*) logger;
        std::string repr = l->to_string();
        return mp_obj_new_str(repr.c_str(), repr.length());
    }
}