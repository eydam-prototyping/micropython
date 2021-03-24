#include <log_formatter.hpp>

LogFormatter::LogFormatter(bool a_colored){
    colored = a_colored;
}

std::string LogFormatter::_logLevelToColor(LogLevel a_ll){
    switch (a_ll)
    {
    case 0: return (std::string)"\033[41m"; break;
    case 1: return (std::string)"\033[30;41m"; break;
    case 2: return (std::string)"\033[31;43m"; break;
    case 3: return (std::string)"\033[31m"; break;
    case 4: return (std::string)"\033[33m"; break;
    case 5: return (std::string)"\033[36m"; break;
    case 6: return (std::string)"\033[37m"; break;
    case 7: return (std::string)"\033[90m"; break;
    default:return (std::string)"UNKNOWN"; break;
    }    
}

std::string LogFormatter::format(const LogMessage* a_msg){
    std::string result = "";
    char buffer[32];
    std::strftime(buffer, 32, "%Y-%m-%dT%H:%M:%SZ", std::localtime(&(a_msg->timestamp)));
    switch (messageFormat)
    {
    case protocol23format:
        result += "<" + std::to_string(a_msg->facility * 8 + a_msg->severity) + ">";
        if (colored){result += "\033[34m";}
        result += " " + (std::string)buffer;
        if (colored){result += "\033[0m";}
        result += " " + (std::string)a_msg->hostname;
        if (colored){result += "\033[32m";}
        result += " " + (std::string)a_msg->appname;
        if (colored){result += "\033[0m";}
        result += " " + (std::string)a_msg->procid;
        result += " " + std::to_string(a_msg->msgid) + " ";
        if (colored){result += _logLevelToColor(a_msg->severity);}
        result += (std::string)a_msg->msg;
        if (colored){result += "\033[0m";}
        break;

    default:
        result = "unknown message format";
        break;
    }
    return result;
}