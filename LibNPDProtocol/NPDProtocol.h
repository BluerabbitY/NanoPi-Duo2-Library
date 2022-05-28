/**
 * @file NPDProtocol.h
 * @author BlueRabbit (515704389@protonmail.com)
 * @brief 通信协议
 * @version 0.1
 * @date 2022-03-03 01:46:21
 *
 * @copyright Copyright (c) 2022 BlueRabbit
 *
 */

#ifndef _NPD_PROTOCOL_H_
#define _NPD_PROTOCOL_H_

#include "NPDDataType.h"
#include "NPDBuffer.h"
#include "NPDError.h"
#include "NPDCommon.h"

#define NPDProtocolHead 0xEFAA
#define NPDProtocolEnd 0xEFBB

// [设置]最大警告长度,超过此长度会告警
#define WARN_MALLOC_SIZE (10 * 1024)

/*
┌────────┬─────────────────┬─────────────────┐
│  name  │      size       │    describe     │
├────────┼─────────────────┼─────────────────┤
├────────┼─────────────────┼─────────────────┤
│ header │ NPDProtocolHead │       head      │
├────────┼─────────────────┼─────────────────┤
│ slength│     uint8_t     │                 │
├────────┼─────────────────┼─────────────────┤
│  type  │     uint8_t     │                 │
├────────┼─────────────────┼─────────────────┤
│ count  │    uint16_t     │     Step count  │
├────────┼─────────────────┼─────────────────┤
│ plength│    uint16_t     │                 │
├────────┼─────────────────┼─────────────────┤
│  data  │   Changeable    │                 │
├────────┼─────────────────┼─────────────────┤
│ tState │      bool       │                 │
├────────┼─────────────────┼─────────────────┤
│ CRC32  │     uint32_t    │                 │
├────────┼─────────────────┼─────────────────┤
│  end   │  NPDProtocolEnd │       end       │
└────────┴─────────────────┴─────────────────┘
*/

namespace NPD
{
    namespace NPDCOMM
    {

        class NPDProtocol
        {
        private:
            struct sProtocol
            {
                uint16_t head = NPDProtocolHead; // 数据头
                uint8_t slength;                 // 结构体长度(初始化赋值并固定)
                uint8_t type = 0;                // 命令类型
                uint16_t count = 0;              // 执行计数
                uint16_t payloadlength = 0;      // 数据长度
                // NPD::NPDBASE::NPDbuffer可变结构数据
                bool tState = false;           // 状态
                bool run = false;              // 执行
                uint16_t cmd = 0;              // 命令码
                uint32_t CRC32 = 0;            // CRC32校验
                uint16_t reserve = 0;          // 保留
                uint16_t end = NPDProtocolEnd; // 数据尾
            };
            typedef struct sProtocol *Protocol;

        public:
            /* 构造与析构函数 */
            // [注意]构造函数只会添加到数据部分,不会添加到头部信息部分
            NPDProtocol();
            NPDProtocol(NPD::NPDBASE::NPDbuffer ndata);
            NPDProtocol(const char *ndata, int nsize);
            ~NPDProtocol();

        public:
            /**
             * @brief 添加数据
             *
             * @param ndata 要添加的数据
             * @return int 不包含头部信息的数据长度
             */
            int addPayload(NPD::NPDBASE::NPDbuffer ndata);
            /**
             * @brief 添加数据
             *
             * @param ndata 要添加的数据
             * @param nsize 要添加的数据的长度
             * @return int 不包含头部信息的数据长度
             */
            int addPayload(const char *ndata, int nsize);
            /**
             * @brief 报文数据解包
             *
             * @param ndata 要解包的数据
             * @param nsize 要解包的数据的长度
             * @return NPD::NPDBASE::NPDbuffer 数据,不包含头部内容
             */
            NPD::NPDBASE::NPDbuffer unpack(void *ndata, int nsize);
            /**
             * @brief 报文数据打包(模板)
             *
             * @tparam T 指定打包数据类型
             * @return T* 指定类型的参数数据指针
             */
            template <typename T>
            T *pack()
            {
                if (pack_p != nullptr)
                {
                    free(pack_p);
                    pack_p = nullptr;
                }
                if (size() >= WARN_MALLOC_SIZE)
                {
                    NPDLOG(NPD_LOG_WARN, "NPDProtocol pack() malloc空间过长[%u]", size());
                }
                T *buf = (T *)malloc(size());
                int offset = 0;
                // 拷贝前半部分数据头
                memcpy(_proto, buf, sizeof(uint16_t) * 3 + sizeof(uint8_t) * 2);
                offset += sizeof(uint16_t) * 3 + sizeof(uint8_t) * 2;
                // 拷贝数据
                memcpy(_data.data(), buf + offset, _data.size());
                offset += _data.size();
                // 拷贝后半部分数据头
                memcpy(_proto + offset - _data.size(), buf + offset, offset);
                pack_p = buf;
                return buf;
            }
            /**
             * @brief 静态数据
             * 
             * @return NPD::NPDBASE::NPDbuffer 
             */
            const char* const_data();
            /**
             * @brief 获取命令类型
             *
             * @return uint16_t 命令类型
             */
            uint16_t type();
            /**
             * @brief 获取cmd命令
             *
             * @return uint16_t cmd命令
             */
            uint16_t cmd();
            /**
             * @brief Protocol总报文长度
             *
             * @return uint32_t 总报文长度
             */
            uint32_t size();
            /**
             * @brief 清空所有内容(包括头和数据)
             *
             */
            void clear();

        private:
            /** 头指针 */
            Protocol _proto = nullptr;
            /** 数据 */
            NPD::NPDBASE::NPDbuffer _data;
            /** pack返回暂存临时指针 */
            void *pack_p = nullptr;

        private:
            /**
             * @brief 初始化(内部调用)
             *
             * @return true 初始化成功
             * @return false 初始化失败
             */
            bool init();
        };
    }
}

#endif /* _NPD_PROTOCOL_H_ */