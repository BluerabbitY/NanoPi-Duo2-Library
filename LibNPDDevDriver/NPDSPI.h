/**
 * @file NPDSPI.h
 * @author BlueRabbit (515704389@protonmail.com)
 * @brief Linux平台SPI设备文件底层读写
 * @version 0.1
 * @date 2022-02-24 03:34:12
 *
 * @copyright Copyright (c) 2022 BlueRabbit
 *
 */

#ifndef _NPD_SPI_H_
#define _NPD_SPI_H_

#include "NPDDevBase.h"

#include <mutex>
#include <linux/spi/spidev.h>

#define HALF_DUPLEX 0
#define FULL_DUPLEX 1

// 发送数据 MESSAGE(1)表示：半双工;MESSAGE(2)表示全双工,全双工需要给RX和TX都附上结构体

// [设置] 半双工/全双工模式选择
#define COMMUNITE_MODE HALF_DUPLEX

// [设置]SPI单次最大收发长度(字节)
#define SPI_MAX_TRANS_LENGTN (4 * 1024)

namespace NPD
{
    namespace NPDDEV
    {
        class NPDSPI : public NPD::NPDDEVBASE::NPDDevBase
        {
        private:
            /**
             * @brief SPI文件描述符
             *
             */
            int _SPI_fd = -1;
            /**
             * @brief SPI模式
             *
             */
            enum eSPIDevMode
            {
                SPI_MODE0 = SPI_MODE_0,
                SPI_MODE1 = SPI_MODE_1,
                SPI_MODE2 = SPI_MODE_2,
                SPI_MODE3 = SPI_MODE_3,
            };
            /**
             * @brief 收缓存
             *
             */
            char RX_Buffer[SPI_MAX_TRANS_LENGTN] = {0};
            /**
             * @brief 发缓存
             *
             */
            char TX_Buffer[SPI_MAX_TRANS_LENGTN] = {0};
            /**
             * @brief 发线程锁
             *
             */
            std::mutex _lock_RX;
            /**
             * @brief 收线程锁
             *
             */
            std::mutex _lock_TX;
            // 基类虚函数实现
        public:
            /**
             * @brief 读取数据
             *
             * @param src 要读取的数据
             * @param size 要读取数据的长度
             * @param param 扩充参数(这里是以寄存器地址方式传输)
             * @return int 读取数据的长度
             */
            int read(void *src, int size, int param = 0);
            /**
             * @brief 写入数据
             *
             * @param dest 要写入的数据
             * @param size 要写入数据的长度
             * @param param 扩充参数(这里是以寄存器地址方式传输)
             * @return int 写入数据的长度
             */
            int write(void *dest, int size, int param = 0);
            /**
             * @brief 读取一字节数据
             *
             * @param dest 要读取的数据
             * @param param 扩充参数(这里是以寄存器地址方式传输)
             * @return int 读取数据的长度
             */
            int read_8bit(uint8_t &dest, int param = 0);
            /**
             * @brief 写入一字节数据
             *
             * @param src 要写入的数据
             * @param param 扩充参数(这里是以寄存器地址方式传输)
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

#endif /* _NPD_SPI_H_ */