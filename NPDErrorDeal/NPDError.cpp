#include "NPDError.h"

#include <stdlib.h>

#include "sqlite3.h"

typedef int NPDErrorno;

struct sNPDError{
    /** 错误类型 */
    char errorType;
    /** 错误编号 */
    NPDErrorno eno;
    /** 语句执行状态 */
    bool execState;
    /** 错误解释 */
    std::string ErrorExplanation;
    /** 错误数据指针 */
    void *pError;
};

std::string NPD_ERROR_DEV_handle(NPDErrorno derror);

NPDError errorCreate(char etype,bool execState,int eeno,void *additional)
{
    NPDError errorno = (NPDError)malloc(sizeof(struct sNPDError));
    errorno->errorType = etype;
    errorno->eno = eeno;
    errorno->execState = execState;
    errorno->pError = additional;
    return errorno;
}

bool errorHandler(NPDError nerror)
{
    if(nerror)
    {
        bool result = false;
        try
        {
            if(!nerror->execState)
            {/* 如果执行失败了打印失败的原因 */
                switch(nerror->errorType)
                {
                    /** 数据库出错 */
                    case NPD_ERROR_DATABASE:{
                        NPD_THROW("[DATABASE ERROR]:%s",sqlite3_errmsg((sqlite3*)(nerror->pError)));
                    };break;
                    /** 设备文件出错 */
                    case NPD_ERROR_DEV:{
                        NPD_THROW("[DEVICE ERROR]:%s",NPD_ERROR_DEV_handle(nerror->eno).c_str());
                    };break;
                    /** 串口出错 */
                    case NPD_ERROR_DEV_SER:{
                        NPD_THROW("[SERIAL PORT ERROR]:%s",NPD_ERROR_DEV_SER_handle(nerror->eno,nerror->pError).c_str());
                    };break;
                    /** IIC出错 */
                    case NPD_ERROR_DEV_IIC:{
                        NPD_THROW("[IIC ERROR]:%s",NPD_ERROR_DEV_IIC_handle(nerror->eno,nerror->pError).c_str());
                    };break;
                    /** TCP出错 */
                    case NPD_ERROR_NET_TCP:{
                        NPD_THROW("[TCP ERROR]:%s",NPD_ERROR_TCP_handle(nerror->eno,nerror->pError).c_str());
                    };break;
                }
            }
            else
            {/* 执行成功 */
                result = true;
            }

        }
        catch(std::exception& e)
        {
            NPD_LOG(NPD_LOG_ERRO,"发生错误:%s", e.what());
            return false;
        }

        free(nerror);
        nerror = nullptr;
        if(result)
        {
            return true;
        }
    }
    else
    {
        NPD_LOG(LOG_ERRO,"NPDError未正确创建");
        return false;
    }
}

std::string NPD_ERROR_DEV_handle(NPDErrorno derror)
{
    std::string ret;
    switch(derror)
    {
        case NPD_ERROR_DEV_NOFILE   : ret = "设备文件不存在";break;
        case NPD_ERRPR_DEV_EMODE    : ret = "错误的设备模式";break;
        case NPD_ERROR_DEV_OPENFAIL : ret = "打开设备文件错误";break;
        default:ret = "未知的设备错误";
    }
    return ret;
}

std::string NPD_ERROR_DEV_SER_handle(NPDErrorno derror, void *param)
{
    std::string ret;
    switch(derror)
    {
        case NPD_ERROR_DEV_SER_PERROR   : ret = strerror(errno);break;
        case NPD_ERROR_DEV_SER_PARITY_I : ret = "串口tcgetattr错误I";break;
        case NPD_ERROR_DEV_SER_PARITY_II: ret = "串口tcgetattr错误II";break;
        case NPD_ERROR_DEV_SER_UNDATA   : ret = "串口数据长度错误";break;
        case NPD_ERROR_DEV_SER_UNPARITY : ret = "串口数据校验标志错误";break;
        case NPD_ERROR_DEV_SER_UNSTOP   : ret = "串口停止位错误";break;
        case NPD_ERROR_DEV_SER_READ     : ret = "串口读数据错误 错误编号:" + std::to_string(*(int*)param);break;
        default:ret = "未知的Serial设备错误";
    }
    return ret;
}

std::string NPD_ERROR_DEV_IIC_handle(NPDErrorno derror, void *param)
{
    std::string ret;
    switch(derror)
    {
        case NPD_ERROR_DEV_IIC_PERROR   : ret = strerror(errno);break;
        case NPD_ERROR_DEV_IIC_PERROR_I : ret = "I2C_TENBIT 错误 " + std::string(strerror(errno));break;
        case NPD_ERROR_DEV_IIC_PERROR_II: ret = "I2C_SLAVE 错误 " + std::string(strerror(errno));break;
        case NPD_ERROR_DEV_IIC_MALLOC   : ret = "IIC 通信结构体分配空间错误 " + std::string(strerror(errno));break;
        case NPD_ERROR_DEV_IIC_EWRITE   : ret = "IIC 写入错误 " + std::string(strerror(errno));break;
        case NPD_ERROR_DEV_IIC_EREAD    : ret = "IIC 读取错误 " + std::string(strerror(errno));break;
        default:ret = "未知的IIC设备错误";
    }
    return ret;
}

std::string NPD_ERROR_TCP_handle(NPDErrorno derror, void *param)
{
    std::string ret;
    switch(derror)
    {
        case NPD_ERROR_NET_TCP_S_ECREATE: ret = "TCP网络创建套接字错误(服务器Server) " + std::string(strerror(errno));break;
        case NPD_ERROR_NET_TCP_S_EBIND  : ret = "TCP网络绑定套接字错误(服务器Server) " + std::string(strerror(errno));break;
        case NPD_ERROR_NET_TCP_S_ELISTEN: ret = "TCP网络监听套接字错误(服务器Server) " + std::string(strerror(errno));break;
        default:ret = "未知的TCP错误";
    }
    return ret;
}