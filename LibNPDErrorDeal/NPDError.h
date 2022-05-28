/**
 * @file NPDError.h
 * @author BlueRabbit (515704389@protonmail.com)
 * @brief 基础错误处理日志打印
 * @version 0.1
 * @date 2022-03-01 08:48:40
 *
 * @copyright Copyright (c) 2022 BlueRabbit
 *
 */

#ifndef _NPD_ERROR_H_
#define _NPD_ERROR_H_

#include <iostream>
#include "NPDErrorno.h"
#include "NPDLOG.h"

#ifndef NPD_LOG_FRONT
#define NPD_LOG_FRONT "NPD2"
#endif

#define NPDDEBUG(...)    \
    printf("NPDTEST ");  \
    printf(__VA_ARGS__); \
    putchar(10);

// 单步异常处理
#define ErrorCreate(...) Error::error()->errorCreate(__VA_ARGS__)
#define ErrorHandler(errorparam) Error::error()->errorHandler(errorparam);

// 打印日志
#define NPDLOG(Errno, ...) \
    NPD::NPDBASE::NPD_LOG(Errno, __VA_ARGS__);

// 异常处理
#define NPDTHROW(...)                     \
    char output[128];                     \
    sprintf(output, (char *)__VA_ARGS__); \
    throw std::runtime_error(std::string(output));

// 带日志的异常处理
#define NPDLOGTHROW(...)                               \
    char output[128];                                  \
    sprintf(output, (char *)__VA_ARGS__);              \
    LOG::GetInstance()->FLOG(NPD_LOG_FRONT, NPD_LOG_THROW, __VA_ARGS__); \
    throw std::runtime_error(std::string(output));

// 自动错误处理(注意:这个宏会返回指定的任意类型的值)
#define AUTO_RETARB_ERROR_HANDLER(ret, ...)  \
    auto Errorno = Error::error()->errorCreate(__VA_ARGS__); \
    Error::error()->errorHandler(Errorno);                   \
    return ret;

// 自动错误处理并返回(注意:这个宏会直接返回bool型的值)
#define AUTO_RETURN_ERROR_HANDLER(...)       \
    auto Errorno = Error::error()->errorCreate(__VA_ARGS__); \
    return Error::error()->errorHandler(Errorno);

// 自动错误处理(注意:这个宏不会返回值)
#define AUTO_ERROR_HANDLER(...)              \
    auto Errorno = Error::error()->errorCreate(__VA_ARGS__); \
    Error::error()->errorHandler(Errorno);

/** 错误处理参数 */
struct sNPDError;
typedef struct sNPDError *NPDError;

class Error
{
public:
    /**
     * @brief 单例模式
     *
     * @return Error* 单例
     */
    static Error *error()
    {
        static Error instance;
        return &instance;
    }

public:
    /**
     *  @brief  创建错误处理参数.
     *  @param  etype  错误类型.
     *  @param  execState  是否发生错误(默认true).
     *  @param  eeno  错误识别码(默认NPD_ERROR_ENO_NULL).
     *  @param  additional  附加内容指针(默认nullptr).
     *  @return  错误识别参数.
     */
    NPDError errorCreate(char etype, bool execState = true, int eeno = NPD_ERROR_ENO_NULL, void *additional = nullptr);
    /**
     *  @brief  NPD错误处理.
     *  @param  nerror  错误识别参数.
     *  @return  执行有无错误.
     */
    bool errorHandler(NPDError nerror);

private:
    // 隐藏构造/析构函数
    Error() {}
    ~Error() {}
};

#endif /* _NPD_ERROR_H_ */