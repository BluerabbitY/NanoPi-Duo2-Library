#ifndef _NPD_TCP_H_
#define _NPD_TCP_H_

#include <string>
#include <thread>
#include <atomic>
#include <vector>

namespace NPDNET
{
    class NPDTCPServer
    {
        public:
        // 初始化服务器  N等待连接队列的最大长度
        bool init(std::string IP,int port,int N = 8);
        // 开启服务器
        bool start();
        // 停止服务器
        void stop();
        // 读取数据
        void read();
        // 写入数据
        void write();

        private:
        typedef struct{
            int tcp_no;
            int acceptfd;
        }sTCPinfo;

        struct sTCPmember{
            sTCPinfo head;
            std::unique_ptr<std::thread> ptr;
        };
        typedef struct sTCPmember* TCPmember;

        private:
        // 其他函数
        // 服务器功能函数
        void Server(sTCPinfo Chead);
        // 客户端分支
        void fork();
        /********************* 变量区 ***********************/
        private:
        static uint32_t ClientCount;
        std::atomic<bool> _run;
        int _sockfd = -1;
        // 创建客户端连接线程
        std::unique_ptr<std::thread> _fork;
        // 客户端列表
        std::vector<TCPmember> TCP_Client_list;
    };
    class NPDTCPClient{
        public:
        // 初始化客户端
        bool init();
        // 连接服务器
        bool connect();
        // 断开连接
        bool disconnect();
        // 读取数据
        void read();
        // 写入数据
        void write();
        private:
        // 其他函数
    };
}

#endif /* _NPD_TCP_ */