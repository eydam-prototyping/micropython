#include <log_message.hpp>

LogMessage::LogMessage(const std::string a_msg, const int a_facility, 
    const LogLevel a_severity, const std::string a_hostname, 
    const std::string a_appname, const std::string a_procid, 
    const int a_msgid){

    msg = a_msg;
    facility = a_facility;
    severity = a_severity;
    hostname = a_hostname;
    appname = a_appname;
    procid = a_procid;
    msgid = a_msgid;
}