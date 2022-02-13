#include "NPDIIC.h"

// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/types.h>
// #include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include <NPDError.h>

using namespace NPDDEV;

// 读取数据
int NPDIIC::read(void *src, int size, int timeout = 0)
{
    FD_IS_READY(_IIC_fd);
#if NPD_IIC_IOCTL_MSG_SELECT
    return ::read(_IIC_fd, src, size);
#else
  	_IIC_DEV_DATA.nmsgs = 2;
    _IIC_DEV_DATA.msgs[1].addr = IIC_DEV_ADDRESS_SET;   // 设置IIC设备地址
    _IIC_DEV_DATA.msgs[1].flag = IIC_M_RD;              // 读取模式
    _IIC_DEV_DATA.msgs[1].leng = size + 1;              // 多出一位用于放数据地址
	_IIC_DEV_DATA.msgs[1].buff = (uint8_t*)src;
    // _IIC_DEV_DATA.msgs[1].buff[0] = IIC_DEV_ADDRESS_DATA_SET;   // 读取数据地址

    auto ret = ioctl(_IIC_fd,I2C_RDWR,(uint64_t)&_IIC_DEV_DATA);
    if(ret < 0)
    {
        nfclose();
        AUTO_RETARB_ERROR_HANDLER(-1,NPD_ERROR_DEV_IIC,false,NPD_ERROR_DEV_IIC_EREAD);
    }
    memset(&_IIC_DEV_DATA.msgs[1],0,sizeof(IIC_MSG));// 清空读取缓存
#endif
}

// 写入数据
int NPDIIC::write(void *dest, int size, int timeout = 0)
{
    FD_IS_READY(_IIC_fd);
#if NPD_IIC_IOCTL_MSG_SELECT
    return ::write(_IIC_fd, dest, size);
#else
    _IIC_DEV_DATA.nmsgs = 1;
    _IIC_DEV_DATA.msgs[0].addr = IIC_DEV_ADDRESS_SET;   // 设置IIC设备地址
    _IIC_DEV_DATA.msgs[0].flag = IIC_M_WR;              // 数据写入模式
    _IIC_DEV_DATA.msgs[0].leng = size + 1;              // 多出一位用于放数据地址
    _IIC_DEV_DATA.msgs[0].buff = (uint8_t*)dest;
    // _IIC_DEV_DATA.msgs[0].buff[0] = IIC_DEV_ADDRESS_DATA_SET;  // 写入目标的地址

    int ret = ioctl(_IIC_fd,I2C_RDWR,(uint64_t)&_IIC_DEV_DATA);
    if(ret < 0)
    {
        nfclose();
        AUTO_RETARB_ERROR_HANDLER(-1,NPD_ERROR_DEV_IIC,false,NPD_ERROR_DEV_IIC_EWRITE);
    }
    memset(&_IIC_DEV_DATA.msgs[0],0,sizeof(IIC_MSG));// 清空写入缓存
#endif
}

// 读取一字节数据
int NPDIIC::read_8bit(uint8_t &dest, int timeout = 0)
{
    FD_IS_READY(_IIC_fd);
#if NPD_IIC_IOCTL_MSG_SELECT
    return ::read(_IIC_fd, &dest,sizeof(uint8_t));
#else
  	_IIC_DEV_DATA.nmsgs = 2;
    _IIC_DEV_DATA.msgs[1].addr = IIC_DEV_ADDRESS_SET;   // 设置IIC设备地址
    _IIC_DEV_DATA.msgs[1].flag = IIC_M_RD;              // 读取模式
    _IIC_DEV_DATA.msgs[1].leng = 8 + 1;              // 多出一位用于放数据地址
	_IIC_DEV_DATA.msgs[1].buff = (uint8_t*)(&dest);
    // _IIC_DEV_DATA.msgs[1].buff[0] = IIC_DEV_ADDRESS_DATA_SET;   // 读取数据地址

    auto ret = ioctl(_IIC_fd,I2C_RDWR,(uint64_t)&_IIC_DEV_DATA);
    if(ret < 0)
    {
        nfclose();
        AUTO_RETARB_ERROR_HANDLER(-1,NPD_ERROR_DEV_IIC,false,NPD_ERROR_DEV_IIC_EREAD);
    }
    memset(&_IIC_DEV_DATA.msgs[1],0,sizeof(IIC_MSG));// 清空读取缓存
#endif
}

// 写入一字节数据
int NPDIIC::write_8bit(uint8_t src, int timeout = 0)
{
    FD_IS_READY(_IIC_fd);
#if NPD_IIC_IOCTL_MSG_SELECT
    return ::write(_IIC_fd, &src, sizeof(uint8_t));
#else
    _IIC_DEV_DATA.nmsgs = 1;
    _IIC_DEV_DATA.msgs[0].addr = IIC_DEV_ADDRESS_SET;   // 设置IIC设备地址
    _IIC_DEV_DATA.msgs[0].flag = IIC_M_WR;              // 数据写入模式
    _IIC_DEV_DATA.msgs[0].leng = 8 + 1;                 // 多出一位用于放数据地址
    _IIC_DEV_DATA.msgs[0].buff = (uint8_t*)(&src);
    // _IIC_DEV_DATA.msgs[0].buff[0] = IIC_DEV_ADDRESS_DATA_SET;  // 写入目标的地址

    int ret = ioctl(_IIC_fd,I2C_RDWR,(uint64_t)&_IIC_DEV_DATA);
    if(ret < 0)
    {
        nfclose();
        AUTO_RETARB_ERROR_HANDLER(-1,NPD_ERROR_DEV_IIC,false,NPD_ERROR_DEV_IIC_EWRITE);
    }
    memset(&_IIC_DEV_DATA.msgs[0],0,sizeof(IIC_MSG));// 清空写入缓存
#endif
}

// 用于各外设独有的配置选项
int NPDIIC::dev_init_setting()
{
    FD_IS_READY(_IIC_fd);
#if NPD_IIC_IOCTL_MSG_SELECT
    uint16_t start_address = 0x50;
    auto ret = ioctl(_IIC_fd,I2C_TENBIT,0);
	if(ret < 0)
    {
        nfclose();
        AUTO_RETARB_ERROR_HANDLER(-1, NPD_ERROR_DEV_IIC,false,NPD_ERROR_DEV_IIC_PERROR_I);
 	}
	ret = ioctl(_IIC_fd,I2C_SLAVE,start_address);
	if(ret < 0)
    {
        nfclose();
        AUTO_RETARB_ERROR_HANDLER(-1, NPD_ERROR_DEV_IIC,false,NPD_ERROR_DEV_IIC_PERROR_II);
 	}
#else
    // 操作时序中,最多是用到2个开始信号(字节读操作中),所以此将nmsgs配置为2
    _IIC_DEV_DATA.nmsgs = 2;
    _IIC_DEV_DATA.msgs = (IIC_MSG*)malloc(_IIC_DEV_DATA.nmsgs*sizeof(IIC_MSG));
    if(!_IIC_DEV_DATA.msgs)
    {
        nfclose();
        AUTO_RETARB_ERROR_HANDLER(-1, NPD_ERROR_DEV_IIC,false,NPD_ERROR_DEV_IIC_MALLOC);
    }
    ioctl(_IIC_fd,I2C_TIMEOUT,2); // 超时时间
    ioctl(_IIC_fd,I2C_RETRIES,2); // 重复次数
#endif
}

// 打开设备文件
bool NPDIIC::nfopen(std::string devname)
{
    // /dev/i2c-0是在注册i2c-dev.c后产生的,代表一个可操作的适配器
    // 如果不使用i2c-dev.c的方式就没有也不需要这个节点
    _IIC_fd = open(devname.c_str(),O_RDWR);
	if (_IIC_fd < 0)
    {
        AUTO_RETURN_ERROR_HANDLER(NPD_ERROR_DEV_IIC,false,NPD_ERROR_DEV_IIC_PERROR);
    }
    set_fd(_IIC_fd);
}

// 关闭设备文件
bool NPDIIC::nfclose()
{
    if(_IIC_fd >= 0)
    {
        close(_IIC_fd);
        _IIC_fd = -1;
    }
    if(_IIC_DEV_DATA.msgs)
    {/* 释放指针 */
        free(_IIC_DEV_DATA.msgs);
        _IIC_DEV_DATA.msgs = nullptr;
    }
}

// 读取函数
int NPDIIC::readfromIO()
{
    char buf[1024];
    int ret = read(buf,sizeof(buf));
    if(ret == 0)
    {/* 无数据 */
        return NPD_ERROR_DEV_IIC_NODATA;
    }
    else if(ret > 0)
    {/* 有数据 */
        return NPD_ERROR_NOERROR;
    }
#if NPD_IIC_IOCTL_MSG_SELECT
    else
    {/* 发生了错误 */
        AUTO_RETARB_ERROR_HANDLER(NPD_ERROR_DEV_IIC,NPD_ERROR_DEV_IIC,false,NPD_ERROR_DEV_IIC_READ,&ret);
    }
#endif
}

// 执行任务函数
int NPDIIC::execute_task()
{

}
