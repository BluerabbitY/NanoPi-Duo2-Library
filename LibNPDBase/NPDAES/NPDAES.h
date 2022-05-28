/**
 * @file NPDAES.h
 * @author BlueRabbit (515704389@protonmail.com)
 * @brief AES加密解密
 * @version 0.1
 * @date 2022-03-01 09:37:10 
 * 
 * @copyright Copyright (c) 2022 BlueRabbit
 * 
 */

#ifndef _NPD_AES_H_
#define _NPD_AES_H_

#include "AES/AES.h"
#include <NPDError.h>

namespace NPD
{
    class NPDAES
    {
    public:
        /* 构造与析构函数 */
        NPDAES();
        NPDAES(std::string key, std::string iv);

    public:
        /**
         * @brief 载入密钥,除ECB外iv均需要最少16字节
         * 
         * @param key 密钥1
         * @param iv 密钥2
         */
        void load(std::string key, std::string iv);
        /**
         * @brief 加密数据
         * 
         * @param data 要加密的数据
         * @param mode 加密模式(默认ECB模式)
         * @return std::string 返回加密后的字符串
         */
        std::string encrypt(std::string data, int mode = AES::ECB);
        /**
         * @brief 解密数据
         * 
         * @param data 要解密的数据
         * @param mode 加密模式(默认ECB模式)
         * @return std::string 返回解密后的字符串
         */
        std::string decrypt(std::string data, int mode = AES::ECB);

    private:
        /** 密钥是否已经正确加载 */
        static bool isKeyLoad;
    };
}

#endif /* _NPD_AES_H_ */