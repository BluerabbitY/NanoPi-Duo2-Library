/**
 * @file NPDIIC.h
 * @author BlueRabbit (515704389@protonmail.com)
 * @brief Linux平台IIC设备文件底层读写
 * @version 0.1
 * @date 2022-02-28 03:02:47
 *
 * @copyright Copyright (c) 2022 BlueRabbit
 *
 */

// TODO:IIC最大收发长度改变

#ifndef _NPD_IIC_H_
#define _NPD_IIC_H_

#include "NPDDevBase.h"
#include <mutex>

// [设置]IIC单次最大收发长度(字节)
#define IIC_MAX_TRANS_LENGTN (8 * 1024)

// NPD_IIC_IOCTL_MODE 选择ioctrl模式
#define NPD_IIC_IOCTL_MODE 1
// NPD_IIC_MESSG_MODE 选择msg模式
#define NPD_IIC_MESSG_MODE 0

// [设置] IIC使用ioctrl或msg选择
#define NPD_IIC_IOCTL_MSG_SELECT NPD_IIC_MESSG_MODE

#if !NPD_IIC_IOCTL_MSG_SELECT
// flag标志位
#define IIC_M_WR 0x0000
#define IIC_M_RD 0x0001

// [设置]这里是IIC设备地址(这里添加,在下面更改)
#define IIC_DEV_ADDRESS_TEST 0x51

// [设置]这里是IIC数据地址(这里添加,在下面更改)
#define IIC_DEV_ADDRESS_DATA_TEST 0x00

// [设置] IIC设备地址
#define IIC_DEV_ADDRESS_SET IIC_DEV_ADDRESS_TEST
// [设置] IIC设备数据地址
#define IIC_DEV_ADDRESS_DATA_SET IIC_DEV_ADDRESS_DATA_TEST
#endif

namespace NPD
{
    namespace NPDDEV
    {
        class NPDIIC : public NPD::NPDDEVBASE::NPDDevBase
        {
        private:
            int _IIC_fd = -1;

#if !NPD_IIC_IOCTL_MSG_SELECT
            /**
             * @brief IIC_MSG结构体
             * 
             */
            typedef struct
            {
                uint16_t addr;
                uint16_t flag;
                uint16_t leng;
                uint8_t *buff;
            } IIC_MSG;
            
            /**
             * @brief IIC_RDWR_IOCTL_DATA结构体
             * 
             */
            typedef struct
            {
                IIC_MSG *msgs;
                int nmsgs; // nmsgs这个数量决定了有多少开始信号,对于"单开始时序",取1
            } IIC_RDWR_IOCTL_DATA;

            IIC_RDWR_IOCTL_DATA _IIC_DEV_DATA;
#endif
            std::mutex _lock_Tx;
            std::mutex _lock_Rx;

            // 基类虚函数实现
        public:
            /**
             * @brief 读取数据
             *
             * @param src 要读取的数据
             * @param size 要读取数据的长度
             * @param param 额外参数
             * @return int 读取数据的长度
             */
            int read(void *src, int size, int param = 0);
            /**
             * @brief 写入数据
             *
             * @param dest 要写入的数据
             * @param size 要写入数据的长度
             * @param param 额外参数
             * @return int 写入数据的长度
             */
            int write(void *dest, int size, int param = 0);
            /**
             * @brief 读取一字节数据
             *
             * @param dest 要读取的数据
             * @param param 额外参数
             * @return int 读取数据的长度
             */
            int read_8bit(uint8_t &dest, int param = 0);
            /**
             * @brief 写入一字节数据
             *
             * @param src 要写入的数据
             * @param param 额外参数
             * @return int 写入数据的长度
             */
            int write_8bit(uint8_t src, int param = 0);

        protected:
            /**
             * @brief 用于各外设独有的配置选项
             *
             * @return int 执行状态(-1:未成功)
             */
            int dev_init_setting();
            /**
             * @brief 打开设备文件
             *
             * @param devname 设备文件路径
             * @return true 打开成功
             * @return false 打开失败
             */
            bool nfopen(std::string devname);
            /**
             * @brief 关闭设备文件
             *
             * @return true 关闭成功
             * @return false 关闭失败
             */
            bool nfclose();
            /**
             * @brief 读取函数
             *
             * @return int 读取状态
             */
            int readfromIO();
            /**
             * @brief 执行任务函数
             *
             * @return int 任务执行状态
             */
            int execute_task();
        };
    }
}

#endif /* _NPD_IIC_H_ */