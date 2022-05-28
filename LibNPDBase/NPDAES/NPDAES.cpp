/**
 * @file NPDAES.cpp
 * @author BlueRabbit (515704389@protonmail.com)
 * @brief AES加密解密
 * @version 0.1
 * @date 2022-03-01 09:37:32 
 * 
 * @copyright Copyright (c) 2022 BlueRabbit
 * 
 */

#include "NPDAES.h"

using namespace NPD;

// 类外初始化
bool NPDAES::isKeyLoad = false;

// 构造函数
NPDAES::NPDAES(){}

NPDAES::NPDAES(std::string key, std::string iv)
{
    isKeyLoad = AES::GetInstance()->Load(key,iv);
}

// 载入
void NPDAES::load(std::string key, std::string iv)
{
    if(isKeyLoad)
    {
        return;
    }
    isKeyLoad = AES::GetInstance()->Load(key,iv);
}

// 加密
std::string NPDAES::encrypt(std::string data, int mode)
{
    if(!isKeyLoad)
    {/* 如果未载入 */
        NPDLOG(NPD_LOG_ERRO,"NPDAES::encrypt() 未正确载入密钥");
        return std::string("");
    }
    auto ret = AES::GetInstance()->EncryptionAES(data,mode);
    if(ret == "NULL")
    {
        return std::string("");
    }
    return ret;
}

// 解密
std::string NPDAES::decrypt(std::string data, int mode)
{
    if(!isKeyLoad)
    {/* 如果未载入 */
        NPDLOG(NPD_LOG_ERRO,"NPDAES::decrypt() 未正确载入密钥");
        return std::string("");
    }
    auto ret = AES::GetInstance()->DecryptionAES(data,mode);
    if(ret == "NULL")
    {
        return std::string("");
    }
    return ret;
}
