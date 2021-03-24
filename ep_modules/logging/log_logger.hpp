#ifndef LOG_LOGGER_HPP_
#define LOG_LOGGER_HPP_

#include <log_handler.hpp>
#include <log_message.hpp>
#include <log_formatter.hpp>
#include <string>
#include <list>

extern "C"{
#include <mpconfigport.h>
}

class BasicLogger{
    private: 
        std::string hostname;
        std::string appname;
        int facility;
        LogLevel level;
        std::list<BasicLogHandler*> logHandlers;
        LogFormatter* lf;
        int msgid = 0;
        std::string _logLevelToString(LogLevel a_ll);
    public:
        BasicLogger(const std::string a_hostname=MICROPY_PY_SYS_PLATFORM, 
            const std::string a_appname="Micropython", 
            const int a_facility=16, const LogLevel a_level=llDebug);
        void log(std::string a_msg, LogLevel a_severity);
        void addHandler(BasicLogHandler* a_lh);
        void setFormatter(LogFormatter* a_lf);
        std::string to_string();
};

#endif