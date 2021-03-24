#ifndef LOG_HANDLER_HPP_
#define LOG_HANDLER_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

class BasicLogHandler{
    public:
        virtual void output(const std::string a_msg);
        virtual std::string to_string();
};

class StreamLogHandler: public BasicLogHandler{
    public:
        virtual void output(const std::string a_msg);
        virtual std::string to_string();
};

class FileStreamLogHandler: public StreamLogHandler{
    private:
        std::string filename;
    public:
        FileStreamLogHandler(const std::string a_filename);
        virtual void output(const std::string a_msg);
        virtual std::string to_string();
};

#endif