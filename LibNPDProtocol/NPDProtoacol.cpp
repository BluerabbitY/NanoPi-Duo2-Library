/**
 * @file NPDProtoacol.cpp
 * @author BlueRabbit (515704389@protonmail.com)
 * @brief 通信协议
 * @version 0.1
 * @date 2022-03-03 01:46:46 
 * 
 * @copyright Copyright (c) 2022 BlueRabbit
 * 
 */

#include "NPDProtocol.h"

using namespace NPD;
using namespace NPD::NPDCOMM;

#define PRO_ERROR_RETURN 0

// 将数据转存到类中
#define SETVALUE(det,src,offset) memcpy(&(det),(uint8_t*)src + offset,sizeof(det));offset += sizeof(det);

NPDProtocol::NPDProtocol()
{
    init();
}

NPDProtocol::NPDProtocol(NPDBASE::NPDbuffer ndata)
{
    init();
    _data.set(0,ndata);
    _proto->payloadlength = ndata.size();
}

NPDProtocol::NPDProtocol(const char *ndata, int nsize)
{
    init();
    _data.set(0,ndata,nsize);
    _proto->payloadlength = nsize;
}

NPDProtocol::~NPDProtocol()
{
    if (_proto)
    {
        delete _proto;
    }
    if(pack_p)
    {
        free(pack_p);
        pack_p = nullptr;
    }
}

int NPDProtocol::addPayload(NPDBASE::NPDbuffer ndata)
{
    _data.append(ndata);
    _proto->payloadlength += ndata.size();
    return _data.size();
}

int NPDProtocol::addPayload(const char *ndata, int nsize)
{
    _data.append(ndata, nsize);
    _proto->payloadlength += nsize;
    return _data.size();
}

NPDBASE::NPDbuffer NPDProtocol::unpack(void *ndata, int nsize)
{
    // 指针为空
    if (!_proto)
    {
        return _data;
    }

    int offset = 0;
    SETVALUE(_proto->head   ,ndata,offset);
    SETVALUE(_proto->slength,ndata,offset);
    SETVALUE(_proto->type   ,ndata,offset);
    SETVALUE(_proto->count  ,ndata,offset);
    SETVALUE(_proto->payloadlength,ndata,offset);

    _data.append((const char *)ndata + offset, nsize);
    offset += nsize;

    SETVALUE(_proto->tState ,ndata,offset);
    SETVALUE(_proto->run    ,ndata,offset);
    SETVALUE(_proto->cmd    ,ndata,offset);
    SETVALUE(_proto->CRC32  ,ndata,offset);
    SETVALUE(_proto->reserve,ndata,offset);
    SETVALUE(_proto->end    ,ndata,offset);

    return _data;
}

// 静态数据
const char* NPDProtocol::const_data()
{
    return _data.const_data();
}

// 获取命令类型
uint16_t NPDProtocol::type()
{
    if(_proto != nullptr)
    {
        return _proto->type;
    }
    return PRO_ERROR_RETURN;
}

// 获取cmd命令
uint16_t NPDProtocol::cmd()
{
    if(_proto != nullptr)
    {
        return _proto->cmd;
    }
    return PRO_ERROR_RETURN;
}

// 总报文长度
uint32_t NPDProtocol::size()
{
    return sizeof(struct sProtocol) + _data.size();
}

// 清空所有内容
void NPDProtocol::clear()
{
    init();
}

bool NPDProtocol::init()
{
    if (_proto == nullptr)
    {
        // 初始化结构体
        _proto = new sProtocol();
    }
    memset(_proto, 0, sizeof(struct sProtocol));
    _proto->head = NPDProtocolHead;  // 数据头
    _proto->end = NPDProtocolEnd;   // 数据尾
    // 结构体长度
    _proto->slength = sizeof(struct sProtocol);
    _data.remove();
    return true;
}
