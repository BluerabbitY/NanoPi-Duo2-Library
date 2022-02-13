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

// 宏定义:判断文件描述符是否已经就绪
#define FD_IS_READY(dev_fd) if(dev_fd < 0){return -1;}

namespace NPDDEV
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
         *  @brief  NPD外设设备初始化.
         *  @param  mode  参数模式(串口,IIC,SPI).
         *  @param  deviceID  设备文件名称.
         *  @return  无返回.
         */
        void init(int mode, std::string deviceID);
        /**
         *  @brief  线程开始.
         *  @return  是否成功开启线程.
         */
        bool start();
        /**
         *  @brief  线程结束.
         *  @return  是否成功关闭线程.
         */
        void stop();
        /**
         *  @brief  获取设备类型(字符串).
         *  @return  设备类型(字符串).
         */
        std::string get_device_type();
        /**
         *  @brief  获取设备类型(id).
         *  @return  设备类型(id).
         */
        const uint32_t get_device_typeID();
        /**
         *  @brief  获取设备树列表.
         *  @return  设备树列表.
         */
        const std::map<int, NPDDevBase *> get_device_tree();
        /**
         *  @brief  获取文件描述符.
         *  @return  文件描述符.
         */
        const int get_fd();

        /*-                    外部开放函数static                        -*/
    public:
        /**
         *  @brief  位置1.
         *  @param  src  8位二进制数.
         *  @param  pos  要置位的位.
         *  @return  无返回.
         */
        static void SetBit(uint8_t &src, uint8_t pos);
        /**
         *  @brief  位置0.
         *  @param  src  8位二进制数.
         *  @param  pos  要置位的位.
         *  @return  无返回.
         */
        static void ResetBit(uint8_t &src, uint8_t pos);
        /**
         *  @brief  位取反.
         *  @param  src  8位二进制数.
         *  @param  pos  要置位的位.
         *  @return  无返回.
         */
        static void FlipBit(uint8_t &src, uint8_t pos);

        /*-                    内部调用                        -*/
    protected:
        // 变量
        /** 完全初始化标志 */
        uint8_t _fullinit = 0x00;
        /** 初始化步骤标志 */
        std::bitset<8> _initflag;
        /** 设备类型 */
        uint32_t typeID = 0;
        /**
         *  @brief  验证是否初始化成功.
         *  @return  是否成功初始化.
         */
        bool check_init();
        /**
         *  @brief  设备任务线程函数.
         *  @return  无返回.
         */
        void NPD_run_task();
        /**
         *  @brief  设置文件描述符.
         *  @return  无返回.
         */
        void set_fd(int fdno);
        /**
         *  @brief  位置1(内部调用).
         *  @param  src  8位二进制数.
         *  @param  pos  要置位的位.
         *  @return  无返回.
         */
        void SetBit(std::bitset<8> &src, uint8_t pos);
        /**
         *  @brief  位置0(内部调用).
         *  @param  src  8位二进制数.
         *  @param  pos  要置位的位.
         *  @return  无返回.
         */
        void ResetBit(std::bitset<8> &src, uint8_t pos);
        /**
         *  @brief  位取反(内部调用).
         *  @param  src  8位二进制数.
         *  @param  pos  要置位的位.
         *  @return  无返回.
         */
        void FlipBit(std::bitset<8> &src, uint8_t pos);

    private:
        /** 设备文件描述符 */
        int _fd = -1;
        /** 设备列表 */
        static std::map<int, NPDDevBase *> _NPD_tree_list;
        /** 线程指针 */
        std::unique_ptr<std::thread> _NPDpthread;
        /** 是否运行 */
        std::atomic<bool> _run;
        /** 设备文件存储 */
        std::string _dev_path;

        /*-                    虚函数                        -*/
    public:
        // 读取数据
        virtual int read(void *src, int size, int timeout = 0) = 0;
        // 写入数据
        virtual int write(void *dest, int size, int timeout = 0) = 0;
        // 读取一字节数据
        virtual int read_8bit(uint8_t &dest, int timeout = 0) = 0;
        // 写入一字节数据
        virtual int write_8bit(uint8_t src, int timeout = 0) = 0;

    protected:
        // 用于各外设独有的配置选项
        virtual int dev_init_setting() = 0;
        // 打开设备文件
        virtual bool nfopen(std::string devname) = 0;
        // 关闭设备文件
        virtual bool nfclose() = 0;
        // 读取函数
        virtual int readfromIO() = 0;
        // 执行任务函数
        virtual int execute_task() = 0;
    };

}

#endif /* _NPD_DEV_BASE_H_ */