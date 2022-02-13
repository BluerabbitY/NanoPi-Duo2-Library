#ifndef _NPD_SERIAL_H_
#define _NPD_SERIAL_H_
// 所需头文件件
#include "NPDDevBase/NPDDevBase.h"

#define BAUDRATE B115200

// 所需命名空间
using namespace NPDDEV;

namespace NPDDEV
{

    class NPDSerial : public NPDDevBase
    {
        // 串口特有函数
    private:
        /* 串口文件描述符 **/
        int _serial_fd = -1;
        // 设置串口波特率
        bool set_speed(int speed);
        // 设置串口内容
        bool set_parity(int databits, int stopbits, int parity);

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

#endif /* _NPD_SERIAL_H_ */