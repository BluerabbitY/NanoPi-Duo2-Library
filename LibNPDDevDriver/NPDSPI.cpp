/**
 * @file NPDSPI.cpp
 * @author BlueRabbit (515704389@protonmail.com)
 * @brief Linux平台SPI设备文件底层读写
 * @version 0.1
 * @date 2022-02-24 03:41:23 
 * 
 * @copyright Copyright (c) 2022 BlueRabbit
 * 
 */

#include "NPDSPI.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <NPDError.h>

using namespace NPD::NPDDEV;

// 读取数据
int NPDSPI::read(void *src, int size, int param)
{
    FD_IS_READY(_SPI_fd);
    int ret = -1;
    uint8_t addr = 0;
    uint8_t offset = 0;
    bool addrflag = false;
    int totleLen = 0, tempLen = 0;
    // SPI收发结构体
    struct spi_ioc_transfer SPI_Transfer = {0};

    if(param > 0)
    {/** 带寄存器地址接收 */
        addrflag = true;
        totleLen += sizeof(uint8_t) + size;
    }
    else if(param == 0)
    {/** 不带寄存器地址接收 */
        totleLen = size;
    }
    tempLen = totleLen;
    // 线程锁
    std::lock_guard<std::mutex> LOCK_RX(_lock_RX);
    while(totleLen)
    {
        // 清空结构体
        memset(&SPI_Transfer,0,sizeof(SPI_Transfer));
        // 清空接收数组
        memset(RX_Buffer,0,sizeof(RX_Buffer));
#if COMMUNITE_MODE
        // 清空发送数组
        memset(TX_Buffer,0,sizeof(TX_Buffer));
#endif
        if(totleLen > SPI_MAX_TRANS_LENGTN)
        {
            // 放置结构体
            SPI_Transfer.rx_buf = (unsigned long long)RX_Buffer;
#if COMMUNITE_MODE
            SPI_Transfer.tx_buf = (unsigned long long)TX_Buffer;
#endif
            SPI_Transfer.len = SPI_MAX_TRANS_LENGTN;
            SPI_Transfer.cs_change = 0;
#if COMMUNITE_MODE
            ret = ioctl(_SPI_fd, SPI_IOC_MESSAGE(2), &SPI_Transfer);
#else
            ret = ioctl(_SPI_fd, SPI_IOC_MESSAGE(1), &SPI_Transfer);
#endif
            if(ret < 0)
            {/** 接收失败 */
#if COMMUNITE_MODE
                NPDLOG(NPD_LOG_ERRO,"SPI全双工读取发生错误[1]");
#else
                NPDLOG(NPD_LOG_ERRO,"SPI半双工读取发生错误[1]");
#endif
                return ret;
            }
            if(addrflag)
            {
                memcpy(&addr,RX_Buffer,sizeof(addr));
                if(addr != param)
                {
#if COMMUNITE_MODE
                    NPDLOG(NPD_LOG_WARN,"SPI全双工读取地址可能错误");
#else
                    NPDLOG(NPD_LOG_WARN,"SPI半双工读取地址可能错误");
#endif
                }
                memcpy((uint8_t*)src + offset,RX_Buffer + 1,SPI_MAX_TRANS_LENGTN - 1);
                offset = offset + SPI_MAX_TRANS_LENGTN - 1;
                totleLen = totleLen - SPI_MAX_TRANS_LENGTN;
                addrflag = false;
                continue;
            }
            memcpy((uint8_t*)src + offset,RX_Buffer,SPI_MAX_TRANS_LENGTN);
            offset += SPI_MAX_TRANS_LENGTN;
            totleLen = totleLen - SPI_MAX_TRANS_LENGTN;
        }
        else
        {
            // 放置结构体
            SPI_Transfer.rx_buf = (unsigned long long)RX_Buffer;
#if COMMUNITE_MODE
            SPI_Transfer.tx_buf = (unsigned long long)TX_Buffer;
#endif
            SPI_Transfer.len = totleLen;
            SPI_Transfer.cs_change = 0;
#if COMMUNITE_MODE
            ret = ioctl(_SPI_fd, SPI_IOC_MESSAGE(2), &SPI_Transfer);
#else
            ret = ioctl(_SPI_fd, SPI_IOC_MESSAGE(1), &SPI_Transfer);
#endif
            if(ret < 0)
            {/** 接收失败 */
#if COMMUNITE_MODE
                NPDLOG(NPD_LOG_ERRO,"SPI半全工读取发生错误[2]");
#else
                NPDLOG(NPD_LOG_ERRO,"SPI半双工读取发生错误[2]");
#endif
                return ret;
            }
            if(addrflag)
            {
                memcpy(&addr,RX_Buffer,sizeof(addr));
                if(addr != param)
                {
#if COMMUNITE_MODE
                    NPDLOG(NPD_LOG_WARN,"SPI半全工读取地址可能错误");
#else
                    NPDLOG(NPD_LOG_WARN,"SPI半双工读取地址可能错误");
#endif
                }
                memcpy(src,RX_Buffer + 1,size - 1);
                offset = offset + size - 1;
                totleLen = 0;
                addrflag = false;
                continue;
            }
            memcpy((uint8_t*)src + offset,RX_Buffer,totleLen);
            totleLen = 0;
        }
    }
    // 成功返回接收的长度
    return tempLen;
}

// 写入数据
int NPDSPI::write(void *dest, int size, int param)
{
    FD_IS_READY(_SPI_fd);
    int ret = -1;
    uint8_t addr = param;
    uint8_t addroffset = 0;
    bool addrflag = false;
    uint8_t offset = 0;
    int totleLen = 0,tempLen = 0;
    // SPI收发结构体
    struct spi_ioc_transfer SPI_Transfer = {0};
    // 清空结构体
    memset(&SPI_Transfer,0,sizeof(SPI_Transfer));

    if(param > 0)
    {/** 带寄存器地址发送 */
        // 寄存器地址拷贝
        totleLen = size + sizeof(uint8_t);
        addrflag = true;
        offset += sizeof(uint8_t);
    }
    else if(param == 0)
    {/** 不带寄存器地址发送 */
        totleLen = size;
    }
    else
    {
        // 为了加快传输速度,这里只打印日志
        NPDLOG(NPD_LOG_ERRO,"SPI地址格式错误");
        return -1;
    }
    tempLen = totleLen;
    // 线程锁
    std::lock_guard<std::mutex> LOCK_TX(_lock_TX);
    while(totleLen)
    {
        // 清空结构体
        memset(&SPI_Transfer,0,sizeof(SPI_Transfer));
        // 清空发送数组
        memset(TX_Buffer,0,sizeof(TX_Buffer));
#if COMMUNITE_MODE
        // 清空接收数组
        memset(RX_Buffer,0,sizeof(RX_Buffer));
#endif
        if(totleLen > SPI_MAX_TRANS_LENGTN)
        {/** 大于单次发送长度,多次发送 */
            if(addrflag)
            {
                // 数据拷贝
                memcpy(TX_Buffer, dest, sizeof(addr));
                addroffset = sizeof(addr);
                totleLen -= 1;
            }
            // 数据拷贝
            memcpy(TX_Buffer + addroffset, (uint8_t*)dest + offset, SPI_MAX_TRANS_LENGTN - addroffset);
            // 数据放入发送寄存器
            SPI_Transfer.tx_buf = (unsigned long long)TX_Buffer;
#if COMMUNITE_MODE
            // 数据放入接收寄存器
            SPI_Transfer.rx_buf = (unsigned long long)RX_Buffer;
#endif
            // 缓冲长度(发送数据长度)
            SPI_Transfer.len = SPI_MAX_TRANS_LENGTN;
            // cs_change结束之后是否需要改变片选线,一般在用户空间控制
            SPI_Transfer.cs_change = 0;
#if COMMUNITE_MODE
            ret = ioctl(_SPI_fd,SPI_IOC_MESSAGE(2),&SPI_Transfer);
#else
            ret = ioctl(_SPI_fd,SPI_IOC_MESSAGE(1),&SPI_Transfer);
#endif
            if(-1 == ret)
            {/** 发送失败 */
#if COMMUNITE_MODE
                NPDLOG(NPD_LOG_ERRO,"SPI全双工发送发生错误[2]");
#else
                NPDLOG(NPD_LOG_ERRO,"SPI半双工发送发生错误[2]");
#endif
                return ret;
            }
            offset += SPI_MAX_TRANS_LENGTN;
            totleLen = totleLen - SPI_MAX_TRANS_LENGTN - addroffset;
            if(addrflag)
            {
                addroffset = 0;
                addrflag = false;
            }
        }
        else
        {/** 小于单次发送长度,单次发送 */
            if(addrflag)
            {
                // 数据拷贝
                memcpy(TX_Buffer, dest, sizeof(addr));
                addroffset += sizeof(addr);
                totleLen -= 1;
                addrflag = false;
            }
            // 数据拷贝
            memcpy(TX_Buffer + addroffset, (uint8_t*)dest + offset, totleLen);
            // 数据放入发送寄存器
            SPI_Transfer.tx_buf = (unsigned long long)TX_Buffer;
#if COMMUNITE_MODE
            // 数据放入接收寄存器
            SPI_Transfer.rx_buf = (unsigned long long)RX_Buffer;
#endif
            // 缓冲长度(发送数据长度)
            SPI_Transfer.len = totleLen;
            // cs_change结束之后是否需要改变片选线,一般在用户空间控制
            SPI_Transfer.cs_change = 0;
#if COMMUNITE_MODE
            ret = ioctl(_SPI_fd,SPI_IOC_MESSAGE(2),&SPI_Transfer);
#else
            ret = ioctl(_SPI_fd,SPI_IOC_MESSAGE(1),&SPI_Transfer);
#endif
            if(-1 == ret)
            {/** 发送失败 */
#if COMMUNITE_MODE
                NPDLOG(NPD_LOG_ERRO,"SPI半全工发送发生错误[2]");
#else
                NPDLOG(NPD_LOG_ERRO,"SPI半双工发送发生错误[2]");
#endif
                return ret;
            }
            offset += totleLen;
            totleLen = 0;
        }
    }
    // 成功返回发送的长度
    return tempLen;
}

// 读取一字节数据
int NPDSPI::read_8bit(uint8_t &dest, int param)
{
    uint8_t temp = dest;
    return read(&temp,sizeof(uint8_t),param);
}

// 写入一字节数据
int NPDSPI::write_8bit(uint8_t src, int param)
{
    uint8_t temp = src;
    return write(&temp,sizeof(uint8_t),param);
}

// 用于各外设独有的配置选项
int NPDSPI::dev_init_setting()
{
    eSPIDevMode spi_mode;
    const uint32_t spi_speed = 0;
    const uint8_t spi_bits = 0;
    FD_IS_READY(_SPI_fd);

    // 设置SPI写模式
    auto ret = ioctl(_SPI_fd, SPI_IOC_WR_MODE, &spi_mode);
    AUTO_FAIL_CLOSE(ret<0,_SPI_fd,AUTO_RETARB_ERROR_HANDLER(-1,NPD_ERROR_DEV_SPI,false,NPD_ERROR_DEV_SPI_PERROR));

    // 设置SPI读模式
    ret = ioctl(_SPI_fd, SPI_IOC_RD_MODE, &spi_mode);
    AUTO_FAIL_CLOSE(ret<0,_SPI_fd,AUTO_RETARB_ERROR_HANDLER(NPD_ERROR_DEV_SPI,false,NPD_ERROR_DEV_SPI_PERROR));

    // 设置SPI写最大速率
    ret = ioctl(_SPI_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
    AUTO_FAIL_CLOSE(ret<0,_SPI_fd,AUTO_RETARB_ERROR_HANDLER(NPD_ERROR_DEV_SPI,false,NPD_ERROR_DEV_SPI_PERROR));

    // 设置SPI读最大速率
    ret = ioctl(_SPI_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
    AUTO_FAIL_CLOSE(ret<0,_SPI_fd,AUTO_RETARB_ERROR_HANDLER(NPD_ERROR_DEV_SPI,false,NPD_ERROR_DEV_SPI_PERROR));

    // 设置写SPI写字长
    ret = ioctl(_SPI_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bits);
    AUTO_FAIL_CLOSE(ret<0,_SPI_fd,AUTO_RETARB_ERROR_HANDLER(NPD_ERROR_DEV_SPI,false,NPD_ERROR_DEV_SPI_PERROR));

    // 设置读SPI读字长
    ret = ioctl(_SPI_fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bits);
    AUTO_FAIL_CLOSE(ret<0,_SPI_fd,AUTO_RETARB_ERROR_HANDLER(NPD_ERROR_DEV_SPI,false,NPD_ERROR_DEV_SPI_PERROR));
    
    return NPD_ERROR_NOERROR;
}

// 打开设备文件
bool NPDSPI::nfopen(std::string devname)
{
    // 打开SPI设备
    _SPI_fd = open(devname.c_str(), O_RDWR);
    if (_SPI_fd < 0)
    {
        AUTO_RETURN_ERROR_HANDLER(NPD_ERROR_DEV_SPI,false,NPD_ERROR_DEV_SPI_PERROR);
    }
    set_fd(_SPI_fd);
    return true;
}

// 关闭设备文件
bool NPDSPI::nfclose()
{
    if(_SPI_fd > 0)
    {
        close(_SPI_fd);
        _SPI_fd = -1;
    }
    return true;
}

// 读取函数
int NPDSPI::readfromIO()
{
    return NPD_ERROR_NOERROR;
}

// 执行任务函数
int NPDSPI::execute_task()
{
    return NPD_ERROR_NOERROR;
}