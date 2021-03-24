#ifndef LOG_HANDLER_HPP_
#define LOG_HANDLER_HPP_

#include <iostream>
#include <fstream>
#include <string>

class BasicLogHandler{
    public:
        virtual void output(const std::string a_msg);
};

class StreamLogHandler: public BasicLogHandler{
    public:
        virtual void output(const std::string a_msg);
};

class FileStreamLogHandler: public StreamLogHandler{
    private:
        std::string filename;
    public:
        FileStreamLogHandler(const std::string a_filename);
        virtual void output(const std::string a_msg);
};

#endif