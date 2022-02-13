#ifndef _NPD_SPI_H_
#define _NPD_SPI_H_

#include "NPDDevBase.h"

namespace NPDDEV
{
    class NPDSPI : public NPDDevBase
    {

        // 基类需函数实现
    public:
        // 读取数据
        int read(void *src, int size, int timeout = 0);
        // 写入数据
        int write(void *dest, int size, int timeout = 0);
        // 读取一字节数据
        int read_8bit(uint8_t &dest, int timeout = 0);
        // 写入一字节数据
        int write_8bit(uint8_t src, int timeout = 0);

    protected:
        // 用于各外设独有的配置选项
        int dev_init_setting();
        // 打开设备文件
        bool nfopen(std::string devname);
        // 关闭设备文件
        bool nfclose();
        // 读取函数
        int readfromIO();
        // 执行任务函数
        int execute_task();
    };
}

#endif /* _NPD_SPI_H_ */