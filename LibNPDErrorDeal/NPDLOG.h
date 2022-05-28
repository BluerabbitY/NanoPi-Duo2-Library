/**
 * @file NPDLOG.h
 * @author BlueRabbit (515704389@protonmail.com)
 * @brief 基于spdlog的日志处理
 * @version 0.1
 * @date 2022-03-19 10:29:45
 *
 * @copyright Copyright (c) 2022 BlueRabbit
 *
 */
#ifndef _NPD_LOG_H_
#define _NPD_LOG_H_

#include <stdarg.h>
#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#ifndef _WIN32
#include <execinfo.h>
#endif

#include "NPDErrorno.h"

#define LOG_PREFIX "NPD2"

#define LOG_BUF_SIZE 4096
#define LOG_FILE "NPDLOG.log"

#define LOG_INFO NPD_LOG_INFO
#define LOG_WARN NPD_LOG_WARN
#define LOG_ERRO NPD_LOG_ERRO
#define LOG_DEBUG NPD_LOG_DEBUG
#define LOG_TRACE NPD_LOG_TRACE
#define LOG_CRITICAL NPD_LOG_CRITICAL

namespace NPD
{
    namespace NPDBASE
    {
        class FLOG
        {
        public:
            static FLOG *instance()
            {
                static FLOG log;
                return &log;
            }
            ~FLOG()
            {
                spdlog::drop_all();
            }
            /**
             * @brief 向日志中写入内容
             * 
             * @param level 错误标识
             * @param info 错误信息
             * @param LOGPREFIX 日志头
             */
            void write_log(int level, const std::string &info);

        private:
            FLOG();
            std::string _prefix = LOG_PREFIX;
        };
        /**
         * @brief 输出日志
         * 
         * @param level 错误编号
         * @param format 输出内容
         * @param ... 可变参数表
         */
        void NPD_LOG(uint8_t level, std::string format, ...);
    }
}

#endif /* _NPD_LOG_H_ */