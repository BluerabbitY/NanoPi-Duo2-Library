#include "NPDTCP.h"

// #include <stdio.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <stdlib.h>
#include <arpa/inet.h>
// #include <sys/wait.h>
// #include <unistd.h>

#include <NPDError.h>

using namespace NPDNET;

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
}

// 停止服务器
void NPDTCPServer::stop()
{
}

// 读取数据
void NPDTCPServer::read()
{
}

// 写入数据
void NPDTCPServer::write()
{
}

void NPDTCPServer::Server(sTCPinfo Chead)
{
    auto curinfo = Chead;
    while (1)
    {
        // printf("wait recv.\n");//5) recv(), 接收客户端发来的数据;   read()
        // recv(Chead.acceptfd, (void *)&udata, sizeof(udata), 0);
        switch (udata.cmd)
        {
        case 'L':{
            // server_login(udata, acceptfd, db);
            udata.cmd = 0;
            };break;
        case 'R':
            //server_register(udata, acceptfd, db);
            udata.cmd = 0;
            break;
        case 'S':
            //server_query_words(udata, acceptfd, db);
            udata.cmd = 0;
            break;
        case 'H':
            //server_history(udata, acceptfd, db);
            udata.cmd = 0;
            break;
        case 'A':
            //server_entry_auto(udata, acceptfd, db);
            udata.cmd = 0;
            break;
        }
    }
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