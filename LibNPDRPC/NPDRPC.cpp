/**
 * @file NPDRPC.cpp
 * @author BlueRabbit (515704389@protonmail.com)
 * @brief 基于rest_rpc的RPC远程调用
 * @version 0.1
 * @date 2022-03-20 10:29:57 
 * 
 * @copyright Copyright (c) 2022 BlueRabbit
 * 
 */
#include "NPDRPC.h"

using namespace NPD;
using namespace NPD::NPDPRC;

bool NPDRPCManager::StartPRC(uint16_t port)
{
    if(!NPDRPCBASE::NPDRPCService::instance()->init(port))
    {
        return false;
    }
    
    //NPDRPCBASE::NPDRPCService::instance()->RegisterFunction("TEST",test);

    NPDRPCBASE::NPDRPCService::instance()->run();
}

void NPDRPCManager::StopRPC()
{
    std::lock_guard<std::mutex> LOCK(_lock);
    _client_list.clear()
    NPDRPCBASE::NPDRPCService::instance()->stop();
}

void NPDRPCManager::add_client(uint64_t id, const std::string& ip, uint16_t port)
{
    // 日志
    std::lock_guard<std::mutex> LOCK(_lock);
    _client_list[id] = std::make_tuple(ip,port);
}
