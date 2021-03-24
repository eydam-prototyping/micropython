#include <log_handler.hpp>

void BasicLogHandler::output(const std::string a_msg){
    std::cout << "OVERRIDE THIS FUNCTION";
}

void StreamLogHandler::output(const std::string a_msg){
    std::cout << a_msg << "\n";
}

FileStreamLogHandler::FileStreamLogHandler(const std::string a_filename){
    filename = a_filename;
}

void FileStreamLogHandler::output(const std::string a_msg){
    std::ofstream fs;
    fs.open(filename, std::ios::out | std::ios::app);
    fs << a_msg << "\n";
    fs.close();
}