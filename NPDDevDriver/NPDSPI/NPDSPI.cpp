#include "NPDSPI.h"

using namespace NPDDEV;

// 读取数据
int NPDSPI::read(void *src, int size, int timeout = 0)
{

}

// 写入数据
int NPDSPI::write(void *dest, int size, int timeout = 0)
{

}

// 读取一字节数据
int NPDSPI::read_8bit(uint8_t &dest, int timeout = 0)
{

}

// 写入一字节数据
int NPDSPI::write_8bit(uint8_t src, int timeout = 0)
{

}

// 用于各外设独有的配置选项
int NPDSPI::dev_init_setting()
{

}

// 打开设备文件
bool NPDSPI::nfopen(std::string devname)
{

}

// 关闭设备文件
bool NPDSPI::nfclose()
{

}

// 读取函数
int NPDSPI::readfromIO()
{

}

// 执行任务函数
int NPDSPI::execute_task()
{
    
}