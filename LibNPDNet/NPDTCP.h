/**
 * @file NPDTCP.h
 * @author BlueRabbit (515704389@protonmail.com)
 * @brief TCP网络服务器/客户端
 * @version 0.1
 * @date 2022-03-03 04:17:37 
 * 
 * @copyright Copyright (c) 2022 BlueRabbit
 * 
 */

#ifndef _NPD_TCP_H_
#define _NPD_TCP_H_

#include <string>
#include <thread>
#include <atomic>
#include <vector>

#include <NPDProtocol.h>
#include <NPDMessageQueue.hpp>

namespace NPD
{
    namespace NPDNETBASE
    {
        class NPDTCPBase
        {
            public:
            // 获取最大发送长度
            int GetMaxRxTxLength();
            public:
            // 服务器/客户端初始化
            virtual bool init(std::string IP, int port, int N = 8) = 0;
            // 发送数据
            virtual int send(NPD::NPDCOMM::NPDProtocol data, int flag = 0) = 0;
            // 接收数据
            virtual int recv(NPD::NPDCOMM::NPDProtocol &data, int flag = 0) = 0;

            /*
            服务器:
            1.开启服务器/关闭
            2.发布消息

            客户端
            1.连接/断开
            2.订阅消息
            */
        };
    }
    namespace NPDNET
    {
        class NPDTCPServer : public NPDNETBASE::NPDTCPBase
        {

        private:
            typedef struct
            {
                /** TCP编号 */
                int tcp_no;
                /** TCP服务端文件描述符 */
                int acceptfd;
            } sTCPinfo;

            struct sTCPmember
            {
                sTCPinfo head;
                std::unique_ptr<std::thread> ptr;
            };
            typedef struct sTCPmember *TCPmember;

#pragma pack(1) //设置1字节对齐
            struct sTCPTransInfo
            {
                uint8_t cmd;
                uint8_t size;
            };
            struct sTCPTrans
            {
                const uint8_t head = 0xAA;
                sTCPTransInfo info;
                const uint8_t end = 0xBB;
            };
#pragma pack()
        public:
            // 初始化服务器  N等待连接队列的最大长度
            bool init(std::string IP, int port, int N = 8);
            // 发送数据
            int send(NPD::NPDCOMM::NPDProtocol data, int flag = 0);
            // 接收数据
            int recv(NPD::NPDCOMM::NPDProtocol &data, int flag = 0);
        public:
            // 开启服务器
            bool start();
            // 停止服务器
            void stop();
        public:
            // 发布
            void release();
        private:
            // 其他函数
            // 服务器功能函数
            void Server(sTCPinfo Chead);
            // 等待消息
            NPD::NPDCOMM::NPDProtocol waitForClient(sTCPinfo Chead);
            // 客户端分支
            void fork();
            bool ping(std::string IP);
        private:
            // 客户端计数
            uint32_t ClientCount;
            std::atomic<bool> _run;
            int _sockfd = -1;
            // 创建客户端连接线程
            std::unique_ptr<std::thread> _fork;
            // 客户端列表
            std::vector<TCPmember> TCP_Client_list;
            // 接受信息变量,提前创建好防止每次创建浪费时间
            sTCPTrans prerecv;
            char *recvsec = nullptr;
        };
        

        
        class NPDTCPClient : public NPDNETBASE::NPDTCPBase
        {
        public:
            // 初始化客户端
            bool init(std::string IP, int port, int N = 8);
            // 发送数据
            int send(NPD::NPDCOMM::NPDProtocol data, int flag = 0);
            // 接收数据
            int recv(NPD::NPDCOMM::NPDProtocol &data, int flag = 0);

        public:
            // 连接服务器
            bool connect();
            // 断开连接
            bool disconnect();
            // 获取
            bool ping();
            // 订阅
            void subscribe();
        private:
            int _maxLength = -1;
            int _sockfd = -1;
            std::string _ip;
            int _port = -1;
            bool _isSubscribe = false;
            NPD::NPDBASE::NPDMessageQueue<uint8_t> _task_queue;
            static int _task_queue_num;
        };
    }
}
#endif /* _NPD_TCP_ */