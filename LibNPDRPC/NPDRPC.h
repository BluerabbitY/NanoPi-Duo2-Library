/**
 * @file NPDRPC.h
 * @author BlueRabbit (515704389@protonmail.com)
 * @brief 基于rest_rpc的RPC远程调用
 * @version 0.1
 * @date 2022-03-20 10:29:46 
 * 
 * @copyright Copyright (c) 2022 BlueRabbit
 * 
 */
#ifndef _NPD_RPC_H_
#define _NPD_RPC_H_

#include <NPDDataType.h>
#include "rest_rpc/rest_rpc.hpp"

namespace NPD
{
    namespace NPDRPCBASE
    {
        class NPDRPCService
        {
            private:
                NPDRPCService(){}
            public:
                static NPDRPCService *instance(){
                    static NPDRPCService rpc;
                    return &rpc;
                }
                bool init(uint16_t port)
                {
                    _service = std::make_unique<rest_rpc::rpc_service::rpc_server>(port,
                    std::thread::hardware_concurrency());
                    if(_service)
                    {
                        return true;
                    }
                    return false;
                }
                void run()
                {
                    if(!_service)return;
                    _service->async_run();
                }
                void stop()
                {
                    if(!_service)return;
                }
                template<typename _T>
                void RegisterFunction(const std::string& funName, _T func)
                {
                    if(!_service)return;
                    _service->register_hanlder(funName,func);
                }
                template<typename _T>
                void Publish(std::string channel, _T& data)
                {
                    if(!_service)return;
                    _service->public(channel,data);
                }
                private:
                std::unique_ptr<rest_rpc::rpc_service::rpc_server> _service;
        };
    }
    namespace NPDPRC
    {
        class NPDRPCManager{
            private:
            NPDRPCManager(){}
            NPDRPCManager(const NPDRPCManager&) = delete;
            public:
            static NPDRPCManager *instance(){
                static NPDRPCManager rpcm;
                return &rpcm;
            }
            public:
            template<typename _T>
            void RegisterFunction(const std::string& funcName, _T func)
            {
                NPDRPCBASE::NPDRPCService::instance()->RegisterFunction(funcName,func);
            }
            bool StartPRC(uint16_t port);
            void StopRPC();
            template<typename _T>
            void Notify(std::string channel, _T& data)
            {
                // 日志
                NPDRPCBASE::NPDRPCService::instance()->publish(channel,data);
            }
            void add_client(uint64_t id, const std::string& ip, uint16_t port);
            private:
            std::mutex _lock;
            std::map<uint64_t, std::tuple<std::string, uint16_t>> _client_list;
        };
    }
}

#endif /* _NPD_RPC_H_ */