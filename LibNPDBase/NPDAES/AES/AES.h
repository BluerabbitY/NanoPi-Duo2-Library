#ifndef _AES_H
#define _AES_H

/************* HEADER FILE ****************/
#include <string>
#include "Base/AESBase.h"
#include "Base/base64.h"
/******************************************/
#define PW_FileName "pw.dat"

//Define a print function.
//If you do not need this print function, just comment out the macro definition
// #define _AESPrint

/* =================================================================================================== */
#ifdef _AESPrint
#ifdef _WIN32
#include <cstdio>
#include <Windows.h>
#endif
#endif

#ifdef _AESPrint
//Define print buffer size
#define AESPrint_size 1024

#ifndef _AESPRINT
#define _AESPRINT

#define AES_INFO 0x01
#define AES_WARN 0x02
#define AES_ERRO 0x03

#define AES_DEBUG   0x04
#define AES_RELEASE 0x05

#endif
#endif


using namespace std;

class AES {
public:
    public:
    typedef enum
    {
        ECB = 0, CBC = 1, CFB = 2
    }iMod;
public:

    /*
    * @function : Get instance
    * @return static intance
    * @return AES instance
    */
    static AES* GetInstance(){
        static AES instance;
        return &instance;
    }

    /*
    * @function : Load key and IV value
    * @param key : key value
    * @param key : iv value
    * @return Is it successfully loaded
    */
    bool Load(string key, string iv);

    /*
    * @function : Get key value
    * @return Key value string
    */
    string GetKeyValue();

    /*
    * @function : Get IV value
    * @return IV value string
    */
    string GetIVValue();

    /*
    * @function : AES encryption
    * @param strSrc : String to Encrypt
    * @param iMode : Encryption mode, default ECB
    * @return Encrypted string
    */
    string EncryptionAES(const string& strSrc, int iMode = ECB);

    /*
    * @function : AES decryption
    * @param strSrc : String to decrypt
    * @param iMode : Decryption mode
    * @return Decrypted string
    */
    string DecryptionAES(const string& strSrc, int iMode = ECB);

    /*
    * @function : Writes the specified content to the file in binary form
    * @param strSrc : String to encrypt
    * @param Index : Content index
    * @param iMode : Encryption mode
    * @param OutputFile : File name to write (default pw.dat)
    * @param srcFilePath : (Default) Open File Path under current executable/pwstore
    * @return Is execution successful
    */
    bool WriteToFile(const string& strSrc, string Index, int iMode = ECB, string OutputFile = PW_FileName, const string& srcFilePath = curexeFilePath);

    /*
    * @function : Read encrypted content from binary file
    * @param Index : Content index
    * @param iMode : Decryption mode
    * @param OutputFile : File name to read (default pw.dat)
    * @param srcFilePath : (Default) Open File Path under current executable/pwstore
    * @return Decrypted string or NULL
    */
    string ReadFromFile(string Index, int iMode = ECB, string InputFile = PW_FileName, const string& srcFilePath = curexeFilePath);

    /*
    * @function : Get current executable file path
    * @return Executable file path
    */
    string GetCurexePath();

    /*
    * @function : Get program version number
    * @return Version number string
    */
    string GetVersion();

#ifdef _AESPrint
    /*
    * @function : AES information print auto line Break
    * @param Errno : Error number
    * @param print : Print string
    * @param ... : Variable parameter
    * @return Length of the printed message
    */
    static int AESPrint(uint8_t Errno, char* print, ...);
#endif
private:
    //Store key value
    string g_key;
    //Store iv value
    string g_iv;
    //Current Executable File Path
    static string curexeFilePath;
private:
    //Hide Constructor/Destructor
    AES();
    ~AES();

    /*
    * @function : Set key value
    * @param strSrc : Key value
    * @return NULL
    */
    void SetKey(string& strSrc);

    /*
    * @function : Set IV value
    * @param strSrc : IV value
    * @return NULL
    */
    void SetIV(string& strSrc);

    /*
    * @function : Check whether the value is set
    * @param iMode : Decryption/Encryption mode
    * @return Whether the value is set
    */
    bool isKeyIVSet(int iMode);

    /*
    * @function : Check whether the value is correct
    * @param iMode : Decryption/Encryption mode
    * @param dsize : Length of IV
    * @return Whether the value is correct
    */
    bool isValueCorrect(int iMode, int dsize);
};

#endif