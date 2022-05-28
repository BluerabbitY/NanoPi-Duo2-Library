/**
 * @file NPDLOG.cpp
 * @author BlueRabbit (515704389@protonmail.com)
 * @brief 基于spdlog的日志处理
 * @version 0.1
 * @date 2022-03-19 10:43:15
 *
 * @copyright Copyright (c) 2022 BlueRabbit
 *
 */
#include "NPDLOG.h"

using namespace NPD;
using namespace NPD::NPDBASE;

void FLOG::write_log(int level, const std::string &info)
{
    auto logger = spdlog::get(_prefix);
    switch (level)
    {
    case NPD_LOG_INFO:
    {
        logger->info(info);
        break;
    }
    case NPD_LOG_WARN:
    {
        logger->warn(info);
        break;
    }
    case NPD_LOG_ERRO:
    {
        logger->error(info);
        break;
    }
    case NPD_LOG_CRITICAL:
    {
        logger->critical(info);
        break;
    }
    case NPD_LOG_TRACE:
    {
        logger->trace(info);
        break;
    }
    case NPD_LOG_DEBUG:
    {
#ifdef DEBUG_LOG
        logger->debug(info);
#endif
        break;
    }
    default:
    {
        logger->error(std::string("NPDLOG错误的标识"));
        break;
    }
    }
}

FLOG::FLOG()
{
    // 初始化线程池
    spdlog::init_thread_pool(16, 2);
    // 控制台显示
    // auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto ptr_stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    // 日志文件
    auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(LOG_FILE, 1024 * 1024 * 10, 3);
    std::vector<spdlog::sink_ptr> sinks{ptr_stdout_sink, rotating_sink};
    auto logger = std::make_shared<spdlog::async_logger>(_prefix, sinks.begin(), sinks.end(),
                                                         spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    logger->flush_on(spdlog::level::info);
    // spdlog::flush_every(std::chrono::seconds(3));
    spdlog::register_logger(logger);
    spdlog::set_level(spdlog::level::debug); // Set global log level to info
}

void NPD::NPDBASE::NPD_LOG(uint8_t level, std::string format, ...)
{
    char buff[LOG_BUF_SIZE] = {};
    // 定义可变参数指针
    va_list arg_ptr;
    va_start(arg_ptr, format);
    // i为最后一个固定参数
    vsprintf(buff, format.c_str(), arg_ptr);
    // 清空参数指针
    va_end(arg_ptr);
    NPDBASE::FLOG::instance()->write_log(level, std::move(std::string(buff)));
}