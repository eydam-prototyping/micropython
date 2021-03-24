#include <log_logger.hpp>

BasicLogger::BasicLogger(const std::string a_hostname, 
    const std::string a_appname, const int a_facility, 
    const LogLevel a_level){

    hostname = a_hostname;
    appname = a_appname;
    facility = a_facility;
    level = a_level;
}

std::string BasicLogger::_logLevelToString(LogLevel a_ll){
    switch (a_ll)
    {
    case 0: return (std::string)"EMERGENCY"; break;
    case 1: return (std::string)"ALARM"; break;
    case 2: return (std::string)"CRITICAL"; break;
    case 3: return (std::string)"ERROR"; break;
    case 4: return (std::string)"WARNING"; break;
    case 5: return (std::string)"NOTICE"; break;
    case 6: return (std::string)"INFO"; break;
    case 7: return (std::string)"DEBUG"; break;
    default:return (std::string)"UNKNOWN"; break;
    }    
}


void BasicLogger::log(std::string a_msg, LogLevel a_severity){
    LogMessage* msg = new LogMessage(a_msg, facility, a_severity,
        hostname, appname, "0",  msgid);
    msgid += 1;

    std::string s_msg = lf->format(msg); 

    for (BasicLogHandler* lh: logHandlers){
        lh->output(s_msg);
    }
}

void BasicLogger::addHandler(BasicLogHandler* a_lh){
    logHandlers.push_back(a_lh);
}

void BasicLogger::setFormatter(LogFormatter* a_lf){
    lf = a_lf;
}

std::string BasicLogger::to_string(){
    std::string result = "Logger(";
    result += "hostname='" + hostname + "', ";
    result += "appname='" + appname + "', ";
    result += "logLevel=" + _logLevelToString(level) + ", ";
    result += "facility=" + std::to_string(facility) + ")";
    return result;
}
