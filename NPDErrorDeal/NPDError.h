#ifndef _NPD_ERROR_H_
#define _NPD_ERROR_H_

#include <iostream>
#include "NPDErrorno.h"
#include "LOG.h"

#ifndef NPD_LOG_FRONT
#define NPD_LOG_FRONT "NPD2"
#endif

// 打印日志
#define NPD_LOG(Errno, ...) \
    NPD::LOG::FLOG(NPD_LOG_FRONT, Errno, __VA_ARGS__);

// 异常处理
#define NPD_THROW(...) \
    char output[128]; \
    sprintf(output,(char*)__VA_ARGS__); \
    throw std::runtime_error(std::string(output));

// 带日志的异常处理
#define NPD_LOG_THROW(Errno, ...) \
    char output[128]; \
    sprintf(output,(char*)__VA_ARGS__); \
    NPD::LOG::FLOG(NPD_LOG_FRONT, Errno, __VA_ARGS__); \
    throw std::runtime_error(std::string(output));

// 自动错误处理(注意:这个宏会返回指定的任意类型的值)
#define AUTO_RETARB_ERROR_HANDLER(ret, ...) \
    auto Errorno = errorCreate(__VA_ARGS__); \
    errorHandler(Errorno); \
    return ret;

// 自动错误处理并返回(注意:这个宏会直接返回bool型的值)
#define AUTO_RETURN_ERROR_HANDLER(...) \
    auto Errorno = errorCreate(__VA_ARGS__); \
    return errorHandler(Errorno);

// 自动错误处理(注意:这个宏不会返回值)
#define AUTO_ERROR_HANDLER(...) \
    auto Errorno = errorCreate(__VA_ARGS__); \
    errorHandler(Errorno);

/** 错误处理参数 */
typedef struct sNPDError* NPDError;

/**
*  @brief  创建错误处理参数.
*  @param  etype  错误类型.
*  @param  execState  是否发生错误(默认true).
*  @param  eeno  错误识别码(默认NPD_ERROR_ENO_NULL).
*  @param  additional  附加内容指针(默认nullptr).
*  @return  错误识别参数.
*/
static NPDError errorCreate(char etype,bool execState = true, int eeno = NPD_ERROR_ENO_NULL, void *additional = nullptr);
/**
*  @brief  NPD错误处理.
*  @param  nerror  错误识别参数.
*  @return  执行有无错误.
*/
static bool errorHandler(const NPDError &nerror);

#endif /* _NPD_ERROR_H_ */