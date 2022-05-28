/**
 * @file NPDTCP.cpp
 * @author BlueRabbit (515704389@protonmail.com)
 * @brief TCP网络服务器/客户端
 * @version 0.1
 * @date 2022-03-03 04:16:55 
 * 
 * @copyright Copyright (c) 2022 BlueRabbit
 * 
 */

#include "NPDTCP.h"

// #include <stdio.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <stdlib.h>
#include <arpa/inet.h>
// #include <sys/wait.h>
// #include <unistd.h>

#include <NPDCommon.h>
#include <NPDError.h>

#include "NPDNetCode.h"

#define ERROR_RETURN_TIMES 3

using namespace NPD::NPDNETBASE;
using namespace NPD::NPDNET;
using namespace NPD::NPDCOMM;
using namespace NPD::NPDBASE;

// 为了提高速度 send/recv中只会有LOG打印,不会由统一错误处理类处理
int NPDTCPClient::_task_queue_num = 0;
/*****************************************  NPDBASE  ****************************************************/

// 获取最大发送长度
int NPDTCPBase::GetMaxRxTxLength()
{
    auto ret = ExecCmd("cat /proc/sys/net/core/rmem_max");
    // 预留1024字节
    return atoi(ret.c_str()) - 1024;
}

/****************************************  NPDSERVER  ***************************************************/
// 初始化服务器
bool NPDTCPServer::init(std::string IP, int port, int N)
{
    // 客户端基数归零
    ClientCount = 0;
    // 创建套接字
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd < 0)
    {
        AUTO_RETURN_ERROR_HANDLER(NPD_ERROR_NET_TCP, false, NPD_ERROR_NET_TCP_S_ECREATE);
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = inet_addr(IP.c_str());

    socklen_t addrlen = sizeof(serveraddr);

    // 绑定套接字
    if (bind(_sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0)
    {
        AUTO_RETURN_ERROR_HANDLER(NPD_ERROR_NET_TCP, false, NPD_ERROR_NET_TCP_S_EBIND);
    }

    // 监听套接字
    if (listen(_sockfd, N) < 0)
    {
        AUTO_RETURN_ERROR_HANDLER(NPD_ERROR_NET_TCP, false, NPD_ERROR_NET_TCP_S_ELISTEN);
    }

    return false;
}

// 开启服务器
bool NPDTCPServer::start()
{
    _run = true;
    _fork = std::make_unique<std::thread>(&NPDTCPServer::fork, this);
    if(_fork != nullptr)
    {
        return true;
    }
    return false;
}

// 停止服务器
void NPDTCPServer::stop()
{
}

// 发送数据
int NPDTCPServer::send(NPD::NPDCOMM::NPDProtocol data, int flag)
{

}

// 接收数据
int NPDTCPServer::recv(NPD::NPDCOMM::NPDProtocol &data, int flag)
{

}

void NPDTCPServer::Server(sTCPinfo Chead)
{
    
    while (1)
    {
        // TODO:收发大小处理机制
        auto ret = waitForClient(Chead);
        // TEST:测试打印数据
        NPDDEBUG("recv");
        switch (ret.cmd())
        {
        case 'L':{
            // server_login(udata, acceptfd, db);
            };break;
        case 'R':
            //server_register(udata, acceptfd, db);
            break;
        case 'S':
            //server_query_words(udata, acceptfd, db);
            break;
        case 'H':
            //server_history(udata, acceptfd, db);
            break;
        case 'A':
            //server_entry_auto(udata, acceptfd, db);
            break;
        }
        //TODO:加入返回确认信息机制
    }
}

NPDProtocol NPDTCPServer::waitForClient(sTCPinfo Chead)
{
    // 首次通信 接收首次通信命令,真实数据长度
    ::recv(Chead.acceptfd, &prerecv, sizeof(prerecv), 0);
    if(prerecv.head == 0xAA && prerecv.end == 0xBB)
    {
        switch(prerecv.info.cmd)
        {
            case NPDNetCode::NPDNET_TCP_RECV: {
                // 为第二次通信动态开辟空间
                recvsec = new char[prerecv.info.size];
                // TODO:添加发送 请求二次数据

                // 第二次通信接收真实际数据
                ::recv(Chead.acceptfd, recvsec, prerecv.info.size, 0);
                // 解析数据
                NPDProtocol recvData;
                recvData.unpack(recvsec,prerecv.info.size);
                delete[] recvsec;
                recvsec = nullptr;
                return recvData;
            };break;
        }
    }
    else
    {
        NPDLOG(NPD_LOG_ERRO,"NPDTCPServer::waitForClient 通信错误 错误的头[0x%02X]或尾[0x%02X]",prerecv.head,prerecv.end);
    }
    return NPDProtocol();
}

// 客户端分支
void NPDTCPServer::fork()
{
    while (_run)
    {
        // 阻塞当有新客户端连接时会继续执行
        auto acceptfd = accept(_sockfd, NULL, NULL);
        TCPmember newconn = (TCPmember)malloc(sizeof(struct sTCPmember));
        // 先自增加再计数,也就是说client编号从1开始
        newconn->head.tcp_no = ++ClientCount;
        newconn->head.acceptfd = acceptfd;
        newconn->ptr = std::make_unique<std::thread>(&NPDTCPServer::Server, this, newconn->head);
        TCP_Client_list.push_back(newconn);
    }
}

/****************************************  NPDCLIENT  ***************************************************/
// 初始化客户端
bool NPDTCPClient::init(std::string IP, int port, int N)
{   
    _sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(_sockfd < 0)
    {
        NPD_ERROR_TYPE retno = NPD_NET_TCP_CLIENT;
        AUTO_RETURN_ERROR_HANDLER(NPD_ERROR_NET_TCP, false, NPD_ERROR_NET_TCP_SC_EINIT, &retno);
	}
    _maxLength = GetMaxRxTxLength();
}

// 发送数据
int NPDTCPClient::send(NPD::NPDCOMM::NPDProtocol data, int flag)
{
    if(_sockfd < 0 || _maxLength < 0)
    {
        NPDLOG(NPD_LOG_ERRO,"NPDTCPClient::send 参数参数未初始化");
        return -1;
    }
    // 添加任务
    _task_queue.push(++_task_queue_num);
    int ret = -1;
    // 错误计数器
    int errorCount = ERROR_RETURN_TIMES;
    // 如果单次发送长度超过了最长发送长度,则分段发送
    int totleLen = data.size();
    if(data.size() > _maxLength)
    {
        // 分段发送计数器
        int ncount = data.size() / _maxLength;
        // 分段发送计数器预保留值
        const int ncount_bak = ncount;
        int offset = 0;
        int sendLength = _maxLength;
        
        while(1)
        {
            if(ncount < 0)
            {
                // ncount计数小于0则表示最后一帧数据已经发送完成,退出
                _task_queue.pop();
                return data.size();
            }
            if(::send(_sockfd, data.const_data() + offset, sendLength, flag) < 0)
            {
                /** 发送失败 重试 */
                if(errorCount > 0)
                {
                    NPDLOG(NPD_LOG_WARN,"NPDTCPClient::send 发送失败 重试[%02d]",ERROR_RETURN_TIMES-errorCount);
                    errorCount--;
                    continue;
                }
                else
                {
                    // 失败次数过多直接返回
                    NPDLOG(NPD_LOG_ERRO,"NPDTCPClient::send 发送失败 返回[-1]");
                    _task_queue.pop();
                    return -1;
                }
            }
            else
            {
                // 分段发送计数器递减
                ncount--;
                // 数据指针偏移
                offset = offset + sendLength;
                /** 发送成功 */
                if(ncount > 0)
                {
                    // 发送计数仍然大于0,表示下次发送数据之后仍然有数据
                    sendLength = _maxLength;
                }
                else
                {
                    // 发送计数等于0,表示下次发送数据为最后一次发送
                    sendLength = data.size() - (ncount_bak * _maxLength);
                }
            }
        }
    }
    else
    {
        while(1)
        {
            // 未超长,直接发送
            if(::send(_sockfd, data.const_data(), data.size(), flag) < 0)
            {
                if(errorCount > 0)
                {
                    NPDLOG(NPD_LOG_WARN,"NPDTCPClient::send 发送失败 重试[%02d]",ERROR_RETURN_TIMES-errorCount);
                    errorCount--;
                    continue;
                }
                else
                {
                    // 失败次数过多直接返回
                    NPDLOG(NPD_LOG_ERRO,"NPDTCPClient::send 发送失败 返回[-1]");
                    _task_queue.pop();
                    return -1;
                }
            }
            else
            {
                /** 发送成功 退出 */
                _task_queue.pop();
                return data.size();
            }
        }
    }
    return -1;
}

// 接收数据
int NPDTCPClient::recv(NPD::NPDCOMM::NPDProtocol &data, int flag)
{
    if(_maxLength < 0 || _sockfd)
    {
        NPDLOG(NPD_LOG_ERRO,"NPDTCPClient::recv 参数参数未初始化");
        return -1;
    }
    _task_queue.push(++_task_queue_num);
    // ::recv(_sockfd,buf,size,flag);
    

}

// 连接服务器
bool NPDTCPClient::connect()
{
    if(_ip.empty() || _port <0)
    {
        NPD_ERROR_TYPE retno = NPD_NET_TCP_CLIENT;
        AUTO_RETURN_ERROR_HANDLER(NPD_ERROR_NET_TCP, false, NPD_ERROR_NET_TCP_SC_EINIT, &retno);
    }
	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(_port);
	serveraddr.sin_addr.s_addr = inet_addr(_ip.c_str());
	socklen_t addrlen = sizeof(serveraddr);

    // LOG打印日志:连接到服务器
    NPDLOG(NPD_LOG_INFO,"NPDTCPClient::connect() 连接到 IP:[%s] PORT:[%d]",_ip.c_str(),_port);
	if(::connect(_sockfd,(struct sockaddr *)&serveraddr,addrlen) < 0)
	{
        AUTO_RETURN_ERROR_HANDLER(NPD_ERROR_NET_TCP, false, NPD_ERROR_NET_TCP_C_ECONNECT);
	}
    return true;
}

// 断开连接
bool NPDTCPClient::disconnect()
{
    while(_task_queue.size() !=0){}
    if(_sockfd > 0)
    {
        if(close(_sockfd) < 0)
        {
            AUTO_RETURN_ERROR_HANDLER(NPD_ERROR_NET_TCP, false, NPD_ERROR_NET_TCP_C_EDCONNECT);
        }
        return true;
    }
    return false;
}

// 获取
bool NPDTCPClient::ping()
{

}

// 订阅
void NPDTCPClient::subscribe()
{

}