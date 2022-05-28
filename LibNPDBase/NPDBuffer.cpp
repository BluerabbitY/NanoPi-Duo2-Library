/**
 * @file NPDBuffer.cpp
 * @author BlueRabbit (515704389@protonmail.com)
 * @brief 自动扩容的数据存放类
 * @version 0.1
 * @date 2022-03-04 09:34:15 
 * 
 * @copyright Copyright (c) 2022 BlueRabbit
 * 
 */

#include "NPDBuffer.h"

#include<cstring>

using namespace NPD::NPDBASE;

NPDbuffer::NPDbuffer(){}
NPDbuffer::~NPDbuffer(){}

NPDbuffer::NPDbuffer(const uint32_t size)
{
    _buffer.resize(size);
}

NPDbuffer::NPDbuffer(const char *src, size_t size)
{
    if(src == nullptr || size ==0)
    {
        NPDTHROW("NPDbuffer()使用了错误的参数[%p][%lu]", src, size);
    }
    _buffer.clear();
    _buffer.resize(size);
    memcpy(_buffer.data(), src, size);
}

NPDbuffer::NPDbuffer(const NPDbuffer &src)
{
    _buffer = src._buffer;
}

NPDbuffer::NPDbuffer(NPDbuffer &&src)
{
    _buffer = src._buffer;
}

/************************************** 运算符重载 **************************************/

NPDbuffer &NPDbuffer::operator=(const NPDbuffer &src)
{
    _buffer = src._buffer;
    return *this;
}

NPDbuffer &NPDbuffer::operator+(const NPDbuffer &src)
{
    _buffer.resize(_buffer.size() + src._buffer.size());
    _buffer.insert(_buffer.end(), src._buffer.begin(), src ._buffer.end());
    return *this;
}

NPDbuffer &NPDbuffer::operator<<(const NPDbuffer &src)
{
    _buffer.resize(_buffer.size() + src._buffer.size());
    _buffer.insert(_buffer.end(), src._buffer.begin(), src ._buffer.end());
    return *this;
}

/*************************************** 功能函数 ***************************************/

uint32_t NPDbuffer::size() const
{
    return (uint32_t)_buffer.size();
}

void NPDbuffer::resize(uint32_t size)
{
    _buffer.resize(size);
}

char *NPDbuffer::data()
{
    return _buffer.data();
}

const char *NPDbuffer::const_data() const
{
    return _buffer.data();
}

NPDbuffer NPDbuffer::get(uint32_t pos, uint32_t size)
{
    if(pos + size > _buffer.size())
    {
        NPDTHROW("NPDbuffer::get()长度[%u]大于有效长度[%lu]", size, _buffer.size());
    }
    NPDbuffer val(_buffer.data()+pos, size);
    return std::move(val);
}

const char *NPDbuffer::query(uint32_t pos, uint32_t size) const
{
    if(pos + size > _buffer.size())
    {
        NPDTHROW("NPDbuffer::query()长度[%u]大于有效长度[%lu]", size, _buffer.size());
    }
    return _buffer.data() + pos;
}

void NPDbuffer::set(uint32_t pos, const char *src, uint32_t srcsize)
{
    if(src == nullptr)
    {
        return;
    }
    if(pos + srcsize > _buffer.size())
    {
        NPDTHROW("NPDbuffer::set() 1 源长度[%u]大于当前有效长度[%lu]", pos + srcsize, _buffer.size());
    }
    memcpy(_buffer.data() + pos, src, srcsize);
}

void NPDbuffer::set(uint32_t pos, const NPDbuffer &src)
{
    if(pos + src.size() > _buffer.size())
    {
        NPDTHROW("NPDbuffer::set() 2 源长度[%u]大于当前有效长度[%lu]", pos + src.size(), _buffer.size());
    }
    memcpy(_buffer.data() + pos, src.const_data(), src.size());
}

NPDbuffer &NPDbuffer::append(const char *src, uint32_t size)
{
    _buffer.reserve(_buffer.size() + size);
    _buffer.insert(_buffer.end(), src, src + size);
    return *this;
}

NPDbuffer &NPDbuffer::append(const NPDbuffer &src)
{
    _buffer.reserve(_buffer.size() + src.size());
    _buffer.insert(_buffer.end(), src.const_data(), src.const_data() + src.size());
    return *this;
}

void NPDbuffer::insert(uint32_t pos, const char *src, uint32_t size)
{
    if(pos >= _buffer.size())
    {
        this->append(src, size);
    }
    else
    {
        _buffer.insert(_buffer.begin() + pos, src, src + size);
    }
}

void NPDbuffer::insert(uint32_t pos, const NPDbuffer &src)
{
    if(pos >= _buffer.size())
    {
        this->append(src);
    }
    else
    {
        _buffer.insert(_buffer.begin() + pos, src.const_data(), src.const_data() + src.size());
    }
}

bool NPDbuffer::empty()
{
    return _buffer.empty();
}

void NPDbuffer::dump() const
{
    if(_buffer.size() > 0)
    {
        int count = 0;
        for(int i = 0;i<_buffer.size();i++)
        {
            if(count%32 == 0)
            {
                putchar(10);
            }
            printf("0x%02X ",_buffer[i]);
            count++;
        }
        putchar(10);
    }
    else
    {
        NPDTHROW("NPDbuffer::dump 1 错误的buffer长度[%lu]", _buffer.size());
    }
}

void NPDbuffer::dump(uint32_t pos, uint32_t size) const
{
    if(_buffer.size() > 0)
    {
        int count = 0;
        if(pos + size > _buffer.size())
        {
            NPDTHROW("NPDbuffer::dump 2 源长度[%u]大于当前有效长度[%lu]", pos + size, _buffer.size());
        }
        for(int i = pos;i<pos+size;i++)
        {
            if(count%32 == 0)
            {
                putchar(10);
            }
            printf("0x%02X ",_buffer[i]);
            count++;
        }
        putchar(10);
    }
    else
    {
        NPDTHROW("NPDbuffer::dump 2 错误的buffer长度[%lu]", _buffer.size());
    }
}

void NPDbuffer::remove()
{
    _buffer.clear();
}

void NPDbuffer::remove(uint32_t pos, uint32_t size)
{
    if(size == 0)
    {
        return;
    }
    if(pos+size > _buffer.size())
    {
        NPDTHROW("NPDbuffer::remove()源长度[%u]大于当前有效长度[%lu]", pos + size, _buffer.size());
    }
    _buffer.erase(_buffer.begin() + pos, _buffer.begin() + pos + size);
}
