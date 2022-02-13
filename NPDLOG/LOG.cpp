#include "LOG.h"
#include <stdarg.h>
#include <iostream>
#include <string>
#include <map>
#include <tuple>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctime>

//Create new folder
#ifdef _WIN32
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#define GenerateString(front,logt,second,buf) "[" + front + "LOG][" + logt + "][" + second + "]:" + buf + "\n"

using namespace std;
using namespace NPD;

bool LOG::flag = true;
std::string LOG::FILEPATH = "";

LOG::LOG()
{

}

LOG::~LOG()
{

}

static map<uint8_t, std::tuple<std::string ,std::string>> LOGPrintMap = {
    {LOG_INFO, {"INFO", "\e[32mINFO\e[0m"}},
    {LOG_WARN, {"WARN", "\e[33mWARN\e[0m"}},
    {LOG_ERRO, {"ERRO", "\e[31mERRO\e[0m"}},

    {LOG_DEBUG  , {"DEBUG", "\e[36mDEBUG\e[0m"}},
    {LOG_RELEASE, {"RELEASE", "\e[35mRELEASE\e[0m"}},
};

/*******************************************************
*    @function : Output log                            *
*    @param front : Log header;                        *
*    @param Errno : Error number;                      *
*    @param print : Output content;                    *
*    @param ... : Variable parameter;                  *
*    @return Number of output characters;              *
*******************************************************/
int LOG::FLOG(std::string front, uint8_t Errno, std::string print, ...)
{
    int fret = -1;
    if(front.size() > 0)
    {
        char buffer[LOG_BUF_SIZE] = {};
        va_list arg_ptr;
        va_start(arg_ptr, print);
        vsprintf(buffer,print.c_str(),arg_ptr);
        va_end(arg_ptr);
        auto retfind = LOGPrintMap.find(Errno);
        if(retfind != LOGPrintMap.end())
        {
            FILE *out;
            int Dret = -1;
            static std::string Path;
#ifndef LDEBUG
            if(flag)
            {
                FILEPATH = GetCurexePath();
#ifdef _WIN32
                Path = FILEPATH + "\\LOG\\";
#else
                Path = FILEPATH + "/LOG/";
#endif
                Dret = DocumentExit(Path);
                Path = Path + LOG_FILE_NAME;
                flag = false;
            }
            else
#endif
            {
                Dret = 1;
            }
            if(Dret > 0)
            {
#ifndef LDEBUG
                out = fopen(Path.c_str(), "a+");
                if(out != NULL)
                {
#endif
                    //格式:LOG名,时间,输出
                    std::string s_buffer = GenerateString(front, GetCurTime(), std::get<0>(retfind->second), std::string(buffer));
                    std::string e_buffer = GenerateString(front, GetCurTime(), std::get<1>(retfind->second), std::string(buffer));
                    fret = s_buffer.size();
                    fret--;
#ifdef _WIN32
                    printf("%s",s_buffer.c_str());
#else
                    printf("%s",e_buffer.c_str());
#endif
#ifndef LDEBUG
                    fprintf(out,"%s",s_buffer.c_str());
                    fclose(out);
                    
                }
#endif
            }
        }
    }
    return fret;
}

/*******************************************************
*    @function : Determine whether the folder exists,  *
*                Create a folder if it does not exist  *
*    @param Path : Folder path;                        *
*    @return Execution status;                         *
*******************************************************/
#ifndef LDEBUG
int LOG::DocumentExit(const std::string& Path)
{
    int ret = 1;
    if(access(Path.c_str(), 0) == -1)
    {/* Folder does not exist */
        ret = -1;
#ifdef _WIN32
        ret = mkdir(Path.c_str());
#else
        ret = mkdir(Path.c_str(), S_IRWXU|S_IRWXO);
#endif
    }
    return ret;
}
#endif

/*******************************************************
*    @function : Get the execuble file path            *
*    @return Execuble file path;                       *
*******************************************************/
#ifndef LDEBUG
std::string LOG::GetCurexePath()
{
#ifdef _WIN32
    string exeFullPath(_pgmptr);
    char* exeName = strrchr((char*)exeFullPath.c_str(), '\\');//#include <stdio.h> #include <string.h>
    exeFullPath = exeFullPath.substr(0,exeFullPath.find(exeName));
#else
    char FullPath[256] = "";
    getcwd(FullPath,sizeof(FullPath));
    string exeFullPath(FullPath);
#endif
    return exeFullPath;
}
#endif

/*******************************************************
*    @function : Get current time                      *
*    @return Current time;                             *
*******************************************************/
std::string LOG::GetCurTime()
{
    time_t NOWTIME = time(0);
    tm *LTIME = localtime(&NOWTIME);
    char buf[64] = "";
    sprintf(buf,"%04d-%02d-%02d %02d:%02d:%02d", LTIME->tm_year+1900, \
                                                 LTIME->tm_mon,LTIME->tm_mday, \
                                                 LTIME->tm_hour,LTIME->tm_min, \
                                                 LTIME->tm_sec);
    return std::string(buf);
}