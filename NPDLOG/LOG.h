#ifndef _LOG_H_
#define _LOG_H_

#include <string>
#include <cstring>
#include <cstdio>
#include "NPDDataType.h"

#define LOG_BUF_SIZE 1024

#define LOG_INFO        0x00
#define LOG_WARN        0x01
#define LOG_ERRO        0x02
#define LOG_DEBUG       0x03
#define LOG_RELEASE     0x04

#define LOG_DEFUAL      "APP"
#define LOG_FILE_NAME   "LOG.txt"

namespace NPD {

class LOG{
public:
    /*
    * @function : Get instance
    * @return static intance
    */
    static LOG* GetInstance(){
        static LOG instance;
        return &instance;
    }
    /*
    * @function : Output log
    * @param front : Log header
    * @param Errno : Error number
    * @param print : Output content
    * @param ... : Variable parameter table
    * @return Number of output characters
    */
    static int FLOG(std::string front, uint8_t Errno, std::string print, ...);
private:
    //Hide Constructor/Destructor
    LOG();
    ~LOG();

    //First use flag
    static bool flag;
    //File path
    static std::string FILEPATH;

#ifndef LDEBUG
    /*
    * @function : Determine whether the folder exists. Create a folder if it does not exist
    * @param Path : Folder path
    * @return Execution status
    */
    static int DocumentExit(const std::string& Path);
#endif

#ifndef LDEBUG
    /*
    * @function : Get the execuble file path
    * @return Execuble file path
    */
    static std::string GetCurexePath();
#endif

    /*
    * @function : Get current time
    * @return Current time
    */
    static std::string GetCurTime();
};

}

#endif