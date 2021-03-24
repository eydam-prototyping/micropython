#ifndef LOG_MESSAGE_HPP_
#define LOG_MESSAGE_HPP_

#include <string>
#include <ctime>
#include <vector>

enum LogLevel{ 
    llEmergency,
    llAlarm, 
    llCritical, 
    llWarning, 
    llError, 
    llNotice, 
    llInfo, 
    llDebug
};

class LogMessage{
    public:
        int facility;
        LogLevel severity;
        time_t timestamp = time(nullptr);
        std::string hostname;
        std::string appname;
        std::string procid;
        int msgid;
        std::string msg;

        LogMessage(const std::string a_msg, const int a_facility, 
            const LogLevel a_severity, const std::string a_hostname, 
            const std::string a_appname, const std::string a_procid, 
            const int a_msgid);
};

#endif