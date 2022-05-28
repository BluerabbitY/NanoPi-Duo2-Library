/**
 * @file NPDSerial.h
 * @author BlueRabbit (515704389@protonmail.com)
 * @brief Linux平台串口设备文件底层读写
 * @version 0.1
 * @date 2022-02-28 03:42:50
 *
 * @copyright Copyright (c) 2022 BlueRabbit
 *
 */

#ifndef _NPD_SERIAL_H_
#define _NPD_SERIAL_H_

#include "NPDDevBase.h"

// [设置]串口波特率
#define BAUDRATE B115200

namespace NPD
{
    namespace NPDDEV
    {
        class NPDSerial : public NPD::NPDDEVBASE::NPDDevBase
        {
            // 串口特有函数
        private:
            /**
             * @brief 串口文件描述符
             *
             */
            int _serial_fd = -1;
            /**
             * @brief 设置串口波特率
             *
             * @param speed 串口波特率
             * @return true 设置成功
             * @return false 设置失败
             */
            bool set_speed(int speed);
            /**
             * @brief 设置串口内容
             *
             * @param databits 数据位
             * @param stopbits 停止位
             * @param parity 奇偶校验
             * @return true 设置成功
             * @return false 设置失败
             */
            bool set_parity(int databits, int stopbits, int parity);
            /**
             * @brief 发线程锁
             *
             */
            std::mutex _lock_Tx;
            /**
             * @brief 收线程锁
             *
             */
            std::mutex _lock_Rx;

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

#endif /* _NPD_SERIAL_H_ */