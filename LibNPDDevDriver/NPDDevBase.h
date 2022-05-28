/**
 * @file NPDDevBase.h
 * @author BlueRabbit (515704389@protonmail.com)
 * @brief 底层设备文件读写基类
 * @version 0.1
 * @date 2022-02-28 03:13:28
 *
 * @copyright Copyright (c) 2022 BlueRabbit
 *
 */

#ifndef _NPD_DEV_BASE_H_
#define _NPD_DEV_BASE_H_

#include <string>
#include <list>
#include <map>
#include <bitset>
#include <atomic>
#include <thread>
#include <mutex>
#include <memory>

// [宏定义]判断文件描述符是否已经就绪
#define FD_IS_READY(dev_fd) \
    if (dev_fd < 0)         \
    {                       \
        return -1;          \
    }
// [宏定义]遇到返回错误自动关闭文件描述符
#define AUTO_FAIL_CLOSE(condition, fd, function) \
    if (condition)                               \
    {                                            \
        close(fd);                               \
        function;                                \
    }
// [宏定义]遇到返回错误自动关闭nf文件描述符
#define AUTO_FAIL_NFCLOSE(condition, fd, function) \
    if (condition)                                 \
    {                                              \
        nfclose(fd);                               \
        function;                                  \
    }

// TODO:完善初始化过程步骤

// 每个设备文件都有最大读写长度,需要按需切割收发,目前已知:SPI单次4K IIC单次8K  在/drivers/ 下查找
// IIC i2c-dev.c下274行8192字节
// SPI spidev.c下93行4096字节
namespace NPD
{
    namespace NPDDEVBASE
    {

        class NPDDevBase
        {
        public:
            enum NPD_DEV_MODE
            {
                SERIAL_MODE = 1,
                IIC_MODE = 2,
                SPI_MODE = 3,
            };
            /*-                    外部开放函数                        -*/
        public:
            /**
             * @brief NPD外设设备初始化
             * 
             * @param mode 参数模式(串口,IIC,SPI)
             * @param deviceID 设备文件名称
             */
            void init(int mode, std::string deviceID);
            /**
             * @brief 线程开始
             * 
             * @return true 开启线程成功
             * @return false 开启线程失败
             */
            bool start();
            /**
             * @brief 线程结束
             * 
             */
            void stop();
            /**
             * @brief 获取设备类型
             * 
             * @return std::string 设备类型
             */
            std::string get_device_type();
            /**
             * @brief 获取设备类型(id)
             * 
             * @return const uint32_t 设备类型(id)
             */
            const uint32_t get_device_typeID();
            /**
             * @brief 获取设备树列表
             * 
             * @return const std::map<int, NPDDevBase *> 设备树列表
             */
            const std::map<int, NPDDevBase *> get_device_tree();
            /**
             * @brief 根据设备名获取设备类
             * 
             * @param devname 设备名
             * @return const NPDDevBase* 设备类
             */
            const NPDDevBase *get_device_by_name(std::string devname);
            /**
             * @brief 获取文件描述符
             * 
             * @return const int 文件描述符
             */
            const int get_fd();
            /**
             * @brief 获取设备名称
             * 
             * @return const std::string 设备名称
             */
            const std::string get_dev_name();
            // 外部开放函数static
        public:
            /**
             * @brief 位置1
             * 
             * @param src 8位二进制数
             * @param pos 要置位的位
             */
            static void SetBit(uint8_t &src, uint8_t pos);
            /**
             * @brief 位置0
             * 
             * @param src 8位二进制数
             * @param pos 要置位的位
             */
            static void ResetBit(uint8_t &src, uint8_t pos);
            /**
             * @brief 位取反
             * 
             * @param src 8位二进制数
             * @param pos 要置位的位
             */
            static void FlipBit(uint8_t &src, uint8_t pos);

            // 内部调用
        protected:
            // 变量
            /**
             * @brief 完全初始化标志
             * 
             */
            uint8_t _fullinit = 0x00;
            /**
             * @brief 初始化步骤标志
             * 
             */
            std::bitset<8> _initflag;
            /**
             * @brief 设备类型
             * 
             */
            uint32_t typeID = 0;
            /**
             * @brief 验证是否初始化成功
             * 
             * @return true 初始化成功
             * @return false 初始化失败
             */
            bool check_init();
            /**
             * @brief 设备任务线程函数
             * 
             */
            void NPD_run_task();
            /**
             * @brief 设置文件描述符
             * 
             * @param fdno 文件描述符
             */
            void set_fd(int fdno);
            /**
             * @brief 位置1(内部调用)
             * 
             * @param src 8位二进制数
             * @param pos 要置位的位
             */
            void SetBit(std::bitset<8> &src, uint8_t pos);
            /**
             * @brief 位置0(内部调用)
             * 
             * @param src 8位二进制数
             * @param pos 要置位的位
             */
            void ResetBit(std::bitset<8> &src, uint8_t pos);
            /**
             * @brief 位取反(内部调用)
             * 
             * @param src 8位二进制数
             * @param pos 要置位的位
             */
            void FlipBit(std::bitset<8> &src, uint8_t pos);

        private:
            /**
             * @brief 设备文件描述符
             * 
             */
            int _fd = -1;
            /**
             * @brief 当前设备名
             * 
             */
            std::string _dev_name;
            /**
             * @brief 设备列表
             * 
             */
            static std::map<int, NPDDevBase *> _npd_dev_tree_list;
            /**
             * @brief 线程指针
             * 
             */
            std::unique_ptr<std::thread> _NPDpthread;
            /**
             * @brief 是否运行
             * 
             */
            std::atomic<bool> _run;
            /**
             * @brief 设备文件存储
             * 
             */
            std::string _dev_path;

            // 虚函数
        public:
            /**
             * @brief 读取数据(纯虚函数)
             *
             * @param src 要读取的数据
             * @param size 要读取数据的长度
             * @param param 额外参数
             * @return int 读取数据的长度
             */
            virtual int read(void *src, int size, int param = 0) = 0;
            /**
             * @brief 写入数据(纯虚函数)
             *
             * @param dest 要写入的数据
             * @param size 要写入数据的长度
             * @param param 额外参数
             * @return int 写入数据的长度
             */
            virtual int write(void *dest, int size, int param = 0) = 0;
            /**
             * @brief 读取一字节数据(纯虚函数)
             *
             * @param dest 要读取的数据
             * @param param 额外参数
             * @return int 读取数据的长度
             */
            virtual int read_8bit(uint8_t &dest, int param = 0) = 0;
            /**
             * @brief 写入一字节数据(纯虚函数)
             *
             * @param src 要写入的数据
             * @param param 额外参数
             * @return int 写入数据的长度
             */
            virtual int write_8bit(uint8_t src, int param = 0) = 0;

        protected:
            /**
             * @brief 用于各外设独有的配置选项(纯虚函数)
             *
             * @return int 执行状态(-1:未成功)
             */
            virtual int dev_init_setting() = 0;
            /**
             * @brief 打开设备文件(纯虚函数)
             *
             * @param devname 设备文件路径
             * @return true 打开成功
             * @return false 打开失败
             */
            virtual bool nfopen(std::string devname) = 0;
            /**
             * @brief 关闭设备文件(纯虚函数)
             *
             * @return true 关闭成功
             * @return false 关闭失败
             */
            virtual bool nfclose() = 0;
            /**
             * @brief 读取函数(纯虚函数)
             *
             * @return int 读取状态
             */
            virtual int readfromIO() = 0;
            /**
             * @brief 执行任务函数(纯虚函数)
             *
             * @return int 任务执行状态
             */
            virtual int execute_task() = 0;
        };

    }
}

#endif /* _NPD_DEV_BASE_H_ */