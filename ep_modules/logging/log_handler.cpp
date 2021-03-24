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
    std::string tmp_fn = filename;

    if(filename.find("%")!=std::string::npos){
        time_t timestamp = time(nullptr);
        char buffer[32];
        std::strftime(buffer, 32, filename.c_str(), std::localtime(&timestamp));
        tmp_fn = buffer;
    }
    
    fs.open(tmp_fn, std::ios::out | std::ios::app);
    fs << a_msg << "\n";
    fs.close();
}

std::string BasicLogHandler::to_string(){
    std::string result = "BasicLogHandler";
    return result;
}

std::string StreamLogHandler::to_string(){
    std::string result = "StreamLogHandler(stream=stdout)";
    return result;
}

std::string FileStreamLogHandler::to_string(){
    std::string result = "FileStreamLogHandler(file='"+filename+"')";
    return result;
}