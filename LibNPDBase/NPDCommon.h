/**
 * @file NPDCommon.h
 * @author BlueRabbit (515704389@protonmail.com)
 * @brief 常用的通用函数
 * @version 0.1
 * @date 2022-03-04 10:38:47
 *
 * @copyright Copyright (c) 2022 BlueRabbit
 *
 */

#ifndef _NPD_COMMON_H_
#define _NPD_COMMON_H_

#include "NPDDataType.h"

namespace NPD
{
    namespace NPDBASE
    {
        /**
         * @brief CRC32校验
         * 
         * @param data 要校验的数据
         * @param size 要校验数据的长度
         * @return uint32_t CRC32校验码
         */
        uint32_t CRC32(uint8_t *data, uint32_t size);
        /**
         * @brief 获取当前程序绝对路径
         * 
         * @return std::string 绝对路径
         * @note  程序中/proc/self/exe代表当前程序
         */
        std::string GetExePath();
        /**
         * @brief 执行命令并获取返回的内容(单行)
         * 
         * @param cmd 要执行的命令
         * @return std::string 命令执行结果
         */
        std::string ExecCmd(std::string cmd);
        /**
         * @brief 执行命令并获取返回的内容(多行)
         * 
         * @param cmd 要执行的命令
         * @return std::vector<std::string> 命令执行结果集
         */
        std::vector<std::string> ExecCmds(std::string cmd);
        /**
         * @brief 获取磁盘使用情况
         * 
         * @param diskPath 磁盘路径
         * @return const uint32_t 磁盘使用率
         */
        const int GetDiskUsage(std::string diskPath);
        /**
         * @brief 获取系统时间(us)
         * 
         * @return int64_t 系统时间
         */
        int64_t GetSystemTime();
        /**
         * @brief 获取程序编译时间
         * 
         * @return std::string 程序编译时间字符串
         */
        std::string GetCompileTime();
        /**
         * @brief 获取程序版本号
         * 
         * @note 分别是 主版本 子版本 内部修订版本 阶段版本
         * 
         * @return std::vector<uint8_t> 程序版本号
         */
        std::vector<uint8_t> GetVersion();
    }
}

#endif /* _NPD_COMMON_H_ */