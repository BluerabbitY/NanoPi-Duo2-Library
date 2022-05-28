#include "AES.h"
#include <iostream>
#include <stdarg.h>
#include <map>
#include <fstream>
#include <sstream>
#include <cstring>
#include "version.h"

#ifdef _WIN32
#include <stdlib.h>
#include <io.h>     //Determine whether the folder exists
#else
#include <unistd.h>
#include <sys/io.h> //Determine whether the folder exists
#endif

//Create new folder
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

//Initialize static variable
string AES::curexeFilePath = "";

//
#define _FileBufferSize 64

//Constructor
AES::AES()
{
    //Get the current executable path when constructing
    curexeFilePath = GetCurexePath();
}

//Destructor
AES::~AES()
{

}

/*******************************************************
*    @function : Load key and IV value                 *
*    @param key : key value                            *
*    @param key : iv value                             *
*    @return Is it successfully loaded;                *
*******************************************************/
bool AES::Load(string key, string iv)
{
    SetKey(key);
    SetIV(iv);
    if(g_key.empty() && g_iv.empty())
    {
        return false;
    }
    return true;
}

/*******************************************************
*    @function : AES encryption                        *
*    @param strSrc : String to Encrypt;                *
*    @param iMode : Encryption mode;                   *
*    @return Encrypted string;                         *
*******************************************************/
string AES::EncryptionAES(const string& strSrc, int iMode)
{
    //Check whether the value is set
    if(isKeyIVSet(iMode))
    {
        if(!isValueCorrect(iMode, g_iv.size()))
        {
            return "NULL";
        }
        size_t length = strSrc.length();
        int block_num = length / BLOCK_SIZE + 1;
        //Plaintext
        char* szDataIn = new char[block_num * BLOCK_SIZE + 1];
        memset(szDataIn, 0x00, block_num * BLOCK_SIZE + 1);
        strcpy(szDataIn, strSrc.c_str());
    
        //PKCS7 Padding
        int k = length % BLOCK_SIZE;
        int j = length / BLOCK_SIZE;
        int padding = BLOCK_SIZE - k;
        for (int i = 0; i < padding; i++)
        {
            szDataIn[j * BLOCK_SIZE + k + i] = padding;
        }
        szDataIn[block_num * BLOCK_SIZE] = '\0';
    
        //Encrypted string
        char *szDataOut = new char[block_num * BLOCK_SIZE + 1];
        memset(szDataOut, 0, block_num * BLOCK_SIZE + 1);
    
        //AES encryption using iMode mode
        AESBase aes;
        aes.MakeKey((const char*)(g_key.c_str()), (const char*)(g_iv.c_str()), 16, 16);
        aes.Encrypt(szDataIn, szDataOut, block_num * BLOCK_SIZE, iMode);
        string str = base64_encode((unsigned char*) szDataOut,
                block_num * BLOCK_SIZE);
        delete[] szDataIn;
        delete[] szDataOut;
        return str;
    }
    else
    {
#ifdef _AESPrint
        AESPrint(AES_ERRO,(char*)"Value not set");
#endif
        return "NULL";
    }
}

/*******************************************************
*    @function : AES Decryption                        *
*    @param strSrc : String to decrypt;                *
*    @param iMode : Decryption mode;                   *
*    @return Decrypted string;                         *
*******************************************************/
string AES::DecryptionAES(const string& strSrc, int iMode)
{
    //Check whether the value is set
    if(isKeyIVSet(iMode))
    {
        if(!isValueCorrect(iMode, g_iv.size()))
        {
            return "NULL";
        }
        string strData = base64_decode(strSrc);
        size_t length = strData.length();
        //Ciphertext
        char *szDataIn = new char[length + 1];
        memcpy(szDataIn, strData.c_str(), length+1);
        //Plaintext
        char *szDataOut = new char[length + 1];
        memcpy(szDataOut, strData.c_str(), length+1);
    
        //AES decryption using iMode mode
        AESBase aes;
        aes.MakeKey((const char*)(g_key.c_str()), (const char*)(g_iv.c_str()), 16, 16);
        aes.Decrypt(szDataIn, szDataOut, length, iMode);
    
        //Remove PKCS7 padding
        //if (0x00 < szDataOut[length - 1] <= 0x16)
        if ((0x00 < szDataOut[length - 1]) && (szDataOut[length - 1] <= 0x10))
        {
            int tmp = szDataOut[length - 1];
            for (int i = length - 1; i >= length - tmp; i--)
            {
                if (szDataOut[i] != tmp)
                {
                    memset(szDataOut, 0, length);
#ifdef _AESPrint
                    AESPrint(AES_ERRO,(char*)"Remove PKCS7 failed,Decryption error");
#endif
                    break;
                }
                else
                    szDataOut[i] = 0;
            }
        }
        string strDest(szDataOut);
        delete[] szDataIn;
        delete[] szDataOut;
        return strDest;
    }
    else
    {
#ifdef _AESPrint
        AESPrint(AES_ERRO,(char*)"Value not set");
#endif
        return "NULL";
    }
}

/*******************************************************
*    @function : Set key value                         *
*    @param strSrc : Key value;                        *
*    @return NULL;                                     *
*******************************************************/
void AES::SetKey(string& strSrc)
{
    g_key = strSrc;
}

/*******************************************************
*    @function : Set IV value                          *
*    @param strSrc : IV value;                         *
*    @return NULL;                                     *
*******************************************************/
void AES::SetIV(string& strSrc)
{
    g_iv = strSrc;
}

/*******************************************************
*    @function : Get key value                         *
*    @return Key value string;                         *
*******************************************************/
string AES::GetKeyValue()
{
    return g_key;
}

/*******************************************************
*    @function : Get IV value                          *
*    @return IV value string;                          *
*******************************************************/
string AES::GetIVValue()
{
    return g_iv;
}

/*******************************************************
*    @function : Check whether the value is set        *
*    @param iMode : Decryption/Encryption mode;        *
*    @return Whether the value is set;                 *
*******************************************************/
bool AES::isKeyIVSet(int iMode)
{
    //ECB mode does not care whether IV has been set
    if(iMode == AES::ECB)
    {
        return !(g_key.empty());
    }
    return (!(g_key.empty()))&&(!(g_iv.empty()));
}

/*******************************************************
*    @function : Check whether the value is correct    *
*    @param iMode : Decryption/Encryption mode;        *
*    @param dsize : Length of VI;                      *
*    @return Whether the value is correct;             *
*******************************************************/
bool AES::isValueCorrect(int iMode, int dsize)
{
    if(dsize <= 0)
    {
#ifdef _AESPrint
        AESPrint(AES_ERRO,(char*)"IV Value is not correct");
#endif
        return false;
    }
    if((iMode == AES::CBC)||(iMode == AES::CFB))
    {
        if(dsize < 16)
        {
#ifdef _AESPrint
            AESPrint(AES_ERRO,(char*)"The minimum length of IV is 16 byte");
#endif
        return false;
        }
    }
    return true;
}

/*******************************************************
*    @function : Writes the specified content to the   *
*                   file in binary form                *
*    @param strSrc : String to encrypt;                *
*    @param Index : Content index;                     *
*    @param iMode : Encryption mode;                   *
*    @param OutputFile : File name to write            *
*                           (default pw.dat);          *
*    @param srcFilePath : (Default) Open File Path     *
*                  under current executable/pwstore;   *
*    @return Is execution successful;                  *
*******************************************************/
bool AES::WriteToFile(const string& strSrc, string Index, int iMode, string OutputFile, const string& srcFilePath)
{
    string exeFullPath;
#ifdef _WIN32
    exeFullPath = srcFilePath + "\\pwstore\\";
#else
    exeFullPath = srcFilePath + "/pwstore/";
#endif
    if(access(exeFullPath.c_str(), 0) == -1)
    {/* Folder does not exist */
        int ret = -1;

#ifdef _WIN32
        ret = mkdir(exeFullPath.c_str());
#else
        ret = mkdir(exeFullPath.c_str(), S_IRWXU|S_IRWXO);
#endif

#ifdef _AESPrint
        if(ret < 0)
        {
            AESPrint(AES_ERRO,(char*)"Unable to create folder");
            return false;
        }
#else
        if(ret < 0)
        {
            return false;
        }
#endif
    }
    exeFullPath += OutputFile;

    int Offset = 0;
    int is_find = 0;
    std::string InputStream = "NULL";
    std::string FileContent = "NULL";
    char ReadBufffer[_FileBufferSize] = "";
    std::ifstream Input;
    std::stringstream buffer;
    Input.open(exeFullPath, ios::in | ios::binary);
    if(Input.is_open())
    {
        FileContent.clear();
        while(Input.read(ReadBufffer, sizeof(ReadBufffer)))
        { /* Read until the end of the file */
            FileContent = std::string(ReadBufffer);
            FileContent = DecryptionAES(FileContent,iMode);
            if(FileContent == Index)
            {
                Offset += _FileBufferSize;
                is_find = 1;
                break;
            }
            memset(ReadBufffer,0,sizeof(ReadBufffer));
            Offset += _FileBufferSize;
        }
        Input.close();
    }

    {/* Save original data */
        Input.open(exeFullPath, ios::in | ios::binary);
        buffer << Input.rdbuf();
        InputStream = buffer.str();
        Input.close();
    }

    char WriteBuffer[_FileBufferSize] = "";
    std::ofstream Output;
    Output.seekp(0L,ios::beg);
    Output.open(exeFullPath, ios::out | ios::binary);
    Output << InputStream;
    if(Output.is_open())
    {
        if(is_find)
        {
            Output.seekp(Offset,ios::beg);
            strcpy(WriteBuffer,EncryptionAES(strSrc,iMode).c_str());
            Output.write((char*)&WriteBuffer, _FileBufferSize);
            memset(WriteBuffer,0,sizeof(WriteBuffer));
        }
        else
        {
            Output.seekp(0L,ios::end);
            strcpy(WriteBuffer,EncryptionAES(Index,iMode).c_str());
            Output.write((char*)&WriteBuffer, _FileBufferSize);
            memset(WriteBuffer,0,sizeof(WriteBuffer));

            strcpy(WriteBuffer,EncryptionAES(strSrc,iMode).c_str());
            Output.write((char*)&WriteBuffer, _FileBufferSize);
            memset(WriteBuffer,0,sizeof(WriteBuffer));
        }
        Output.close();
    }
    return true;
}


/*******************************************************
*    @function : Read encrypted content                *
*                 from binary file                     *
*    @param Index : Content index;                     *
*    @param iMode : Decryption mode;                   *
*    @param InputFile : File name to write             *
*                           (default pw.dat);          *
*    @param srcFilePath : (Default) Open File Path     *
*                  under current executable/Code;      *
*    @return Decrypted string or NULL;                 *
*******************************************************/
string AES::ReadFromFile(string Index, int iMode, string InputFile, const string& srcFilePath)
{
    string exeFullPath;
#ifdef _WIN32
    exeFullPath = srcFilePath + "\\pwstore\\" + InputFile;
#else
    exeFullPath = srcFilePath + "/pwstore/" + InputFile;
#endif
    int StopFlag = 0;
    string FileContent = "";
    char ReadBufffer[_FileBufferSize] = "";
    std::ifstream Input;
    Input.open(exeFullPath, ios::in | ios::binary);
    if(Input.is_open())
    {
        FileContent.clear();
        while(Input.read(ReadBufffer, sizeof(ReadBufffer)))
        { /* Read until the end of the file */
            FileContent = std::string(ReadBufffer);
            FileContent = DecryptionAES(FileContent,iMode);
            if(StopFlag)break;
            if(FileContent == Index)StopFlag = 1;
            memset(ReadBufffer,0,sizeof(ReadBufffer));
        }
        Input.close();
        if(!StopFlag)
        {
            FileContent = "";
        }
    }
    return FileContent;
}

/*******************************************************
*    @function : Get current executable file path      *
*    @return Executable file path;                     *
*******************************************************/
string AES::GetCurexePath()
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

/*******************************************************
*    @function : Get program version number            *
*    @return Version number string;                    *
*******************************************************/
string AES::GetVersion()
{
    std::map<uint32_t,string> VersionMap = {
        {0,"Alpha"},
        {1,"Beta"},
        {2,"RC"},
        {3,"Release"}
    };
    auto ret = VersionMap.find(STAGE_VERSION);
    char buf[128];
    sprintf(buf,"V%d.%d.%d.%d %s",MAIN_VERSION,SUB_VERSION,INNER_VERSION,STAGE_VERSION,ret->second.c_str());
    return string(buf);
}

/*******************************************************
*    @function : AES information print                 *
*    @param Errno : Error number;                      *
*    @param print : Print string;                      *
*    @param ... : Variable parameter;                  *
*    @return Length of the printed message;            *
*******************************************************/
#ifdef _AESPrint
#ifdef _WIN32
int AES::AESPrint(uint8_t Errno, char* print, ...)
{
    //buffer size
    char buffer[AESPrint_size] = {};
    //Define variable parameter pointers
    va_list arg_ptr;
    //i is the last fixed parameter
    va_start(arg_ptr, print);
    //Copy to buffer
    vsprintf(buffer,print,arg_ptr);
    //Clear pointer
    va_end(arg_ptr);

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    int Length = 0;
    printf("AES[");
    switch(Errno)
    {
        case AES_INFO   :SetConsoleTextAttribute(handle, FOREGROUND_GREEN);printf("INFO");SetConsoleTextAttribute(handle, 0x07);Length=9;break;
        case AES_WARN   :SetConsoleTextAttribute(handle, FOREGROUND_GREEN|FOREGROUND_RED);printf("WARN");SetConsoleTextAttribute(handle, 0x07);Length=9;break;
        case AES_ERRO   :SetConsoleTextAttribute(handle, FOREGROUND_RED);printf("ERRO");SetConsoleTextAttribute(handle, 0x07);Length=9;break;
        case AES_DEBUG  :SetConsoleTextAttribute(handle, FOREGROUND_BLUE);printf("DEBUG");  SetConsoleTextAttribute(handle, 0x07);Length=10;break;
        case AES_RELEASE:SetConsoleTextAttribute(handle, FOREGROUND_RED|FOREGROUND_BLUE);printf("RELEASE");SetConsoleTextAttribute(handle, 0x07);Length=12;break;
    }
    printf("]:%s\n",buffer);
    return strlen(buffer)+Length;
}
#else
static map<uint8_t,string> AESPrintMap = {
    {AES_INFO,"\e[32mINFO\e[0m"},/*   Green  */
    {AES_WARN,"\e[33mWARN\e[0m"},/*  Yellow  */
    {AES_ERRO,"\e[31mERRO\e[0m"},/*   RED    */

    {AES_DEBUG  ,"\e[36mDEBUG\e[0m"},  /* Blue */
    {AES_RELEASE,"\e[35mRELEASE\e[0m"} /* Purple */
};

int AES::AESPrint(uint8_t Errno, char* print, ...)
{
    //buffer size
    char buffer[AESPrint_size] = {};
    //Define variable parameter pointers
    va_list arg_ptr;
    //i is the last fixed parameter
    va_start(arg_ptr, print);
    //Copy to buffer
    vsprintf(buffer,print,arg_ptr);
    //Clear pointer
    va_end(arg_ptr);
    auto ret = AESPrintMap.find(Errno);
    printf("AES[%s]:%s\n",ret->second.c_str(),buffer);
    return strlen(buffer);
}
#endif
#endif