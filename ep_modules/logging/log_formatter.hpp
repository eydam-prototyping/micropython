#ifndef LOG_FORMATTER_HPP_
#define LOG_FORMATTER_HPP_

#include <log_message.hpp>
#include <string>
#include <ctime>

#define msg_highlight_ok_blue "\033[94m"
#define msg_highlight_endc "\033[0m"


enum MessageFormat { protocol23Format, combinedLogFormat };

class LogFormatter{
    private:
        MessageFormat messageFormat;
        bool colored;
        
        std::string _logLevelToColor(LogLevel a_ll);
    public:
        LogFormatter(bool a_colored=false);
        std::string format(const LogMessage* a_msg);
};

#endif