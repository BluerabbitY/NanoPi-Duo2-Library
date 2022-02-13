#ifndef _NPD_IIC_H_
#define _NPD_IIC_H_

#include "NPDDevBase.h"

// IIC使用ioctrl或msg选择

// NPD_IIC_IOCTL_MODE 选择ioctrl模式
#define NPD_IIC_IOCTL_MODE 1
// NPD_IIC_MESSG_MODE 选择msg模式
#define NPD_IIC_MESSG_MODE 0

// [设置] IIC使用ioctrl或msg选择
#define NPD_IIC_IOCTL_MSG_SELECT NPD_IIC_MESSG_MODE

#if !NPD_IIC_IOCTL_MSG_SELECT
// flag标志位
#define IIC_M_WR    0x0000
#define IIC_M_RD    0x0001

// [设置]这里是IIC设备地址(这里添加,在下面更改)
#define IIC_DEV_ADDRESS_TEST 0x51

// [设置]这里是IIC数据地址(这里添加,在下面更改)
#define IIC_DEV_ADDRESS_DATA_TEST 0x00

// [设置] IIC设备地址
#define IIC_DEV_ADDRESS_SET IIC_DEV_ADDRESS_TEST
// [设置] IIC设备数据地址
#define IIC_DEV_ADDRESS_DATA_SET IIC_DEV_ADDRESS_DATA_TEST
#endif

namespace NPDDEV
{
    class NPDIIC : public NPDDevBase
    {
    private:
        int _IIC_fd = -1;

#if !NPD_IIC_IOCTL_MSG_SELECT
        typedef struct
        {
            uint16_t addr;
            uint16_t flag;
            uint16_t leng;
            uint8_t *buff;
        }IIC_MSG;

        typedef struct
        {
            IIC_MSG *msgs;
            int nmsgs;// nmsgs这个数量决定了有多少开始信号,对于"单开始时序",取1
        }IIC_RDWR_IOCTL_DATA;

        IIC_RDWR_IOCTL_DATA _IIC_DEV_DATA;
#endif

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

#endif /* _NPD_IIC_H_ */