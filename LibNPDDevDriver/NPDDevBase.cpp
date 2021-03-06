/**
 * @file NPDDevBase.cpp
 * @author BlueRabbit (515704389@protonmail.com)
 * @brief 底层设备文件读写基类
 * @version 0.1
 * @date 2022-02-28 03:14:01 
 * 
 * @copyright Copyright (c) 2022 BlueRabbit
 * 
 */

#include "NPDDevBase.h"

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <filesystem>

#include <NPDError.h>

using namespace NPD::NPDDEVBASE;

// 静态变量初始化
std::map<int, NPDDevBase *> NPDDevBase::_npd_dev_tree_list;

void NPDDevBase::init(int mode,std::string deviceID)
{
    // 检测模式是否正确
    switch (mode)
    {
        case SERIAL_MODE:_fullinit = 0xFF;break;//初始化步骤,在这里修改初始化需要多少步
        case IIC_MODE   :_fullinit = 0xFF;break;
        case SPI_MODE   :_fullinit = 0xFF;break;
        default:NPDLOG(NPD_LOG_ERRO,"错误的设备模式");break;
    }
    typeID = mode;
    // 检测设备文件是否存在
    bool _fExit = std::filesystem::exists(deviceID);
    if(_fExit)
    {/* 文件存在 */
        _dev_path = deviceID;
        // 第一步初始化完成
        SetBit(_initflag,1);
    }
    else
    {/* 文件不存在 */
        // 第一步初始化未完成
        ResetBit(_initflag,1);
    }
    // 错误处理
    AUTO_ERROR_HANDLER(NPD_ERROR_DEV,_fExit,NPD_ERROR_DEV_NOFILE);
}

// 线程开始
bool NPDDevBase::start()
{
    // 验证是否初始化成功
    if(check_init())
    {
        if(nfopen(_dev_path.c_str()))
        {
            // 设备文件打开失败 错误处理
            AUTO_RETURN_ERROR_HANDLER(NPD_ERROR_DEV,false,NPD_ERROR_DEV_OPENFAIL);
        }
        // 配置各外设独有的配置选项
        dev_init_setting();
        // 创建线程
        _NPDpthread = std::make_unique<std::thread>(&NPDDevBase::NPD_run_task,this);
        _dev_name = _dev_path;
        // 添加设备文件列表
        _npd_dev_tree_list.insert(std::make_pair(_fd,this));
        return true;
    }
    else
    {
        return false;
    }
}

void NPDDevBase::stop()
{
    _run = false;
    if(_NPDpthread)
    {
        // 停止线程
        _NPDpthread->join();
        // 关闭设备文件
        nfclose();
        // 移除设备文件列表
        _npd_dev_tree_list.erase(_fd);
    }
}

std::string NPDDevBase::get_device_type()
{
    std::string ret;
    if(typeID == 0)
    {
        ret = "空设备类型";
        return ret;
    }
    switch(typeID)
    {
        case SERIAL_MODE: ret = "串口设备";break;
        case IIC_MODE: ret = "IIC设备";break;
        case SPI_MODE: ret = "SPI设备";break;
        default:ret = "未知设备类型";
    }
    return ret;
}

const uint32_t NPDDevBase::get_device_typeID()
{
    return typeID;
}

const std::map<int, NPDDevBase*> NPDDevBase::get_device_tree()
{
    return _npd_dev_tree_list;
}

const NPDDevBase *NPDDevBase::get_device_by_name(std::string devname)
{
    for(auto item:_npd_dev_tree_list)
    {
        if(item.second->get_dev_name() == devname)
        {
            return item.second;
        }
    }
    return nullptr;
}

bool NPDDevBase::check_init()
{
    bool flag = true;
    for(int i=0;i<8;i++)
    {
        if(_fullinit & (0x01 << i))
        {
            if(!(_initflag.test(i)))
            {
                NPDLOG(NPD_LOG_ERRO,"初始化步骤[%02d]未完成,请检查",i+1);
                flag = false;
                continue;
            }
        }
    }
    return flag;
}

void NPDDevBase::NPD_run_task()
{
    while(_run)
    {
        if(NPD_ERROR_NOERROR != readfromIO())
        {
            continue;
        }
        // 执行每个设备文件的功能
        auto ret = execute_task();
        if(NPD_ERROR_NOERROR != ret)
        {
            NPDLOG(NPD_LOG_ERRO,"执行任务失败,设备编号[%d],错误编号[0x%02x]",_fd,ret);
        }
    }
}

const int NPDDevBase::get_fd()
{
    return _fd;
}

const std::string NPDDevBase::get_dev_name()
{
    return _dev_name;
}

void NPDDevBase::set_fd(const int fdno)
{
    _fd = fdno;
}

/*                      位操作                            */

void NPDDevBase::SetBit(uint8_t &src, uint8_t pos)
{
    if( (pos>=1) && (pos<=8) )
    {
        src = src | (0x01 << (pos - 1));
    }
    else
    {
        NPDLOG(NPD_LOG_ERRO,"SetBit错误的位置");
    }
}

void NPDDevBase::ResetBit(uint8_t &src, uint8_t pos)
{
    if( (pos>=1) && (pos<=8) )
    {
        src = src & ( ~ (0x01 << (pos - 1)));
    }
    else
    {
        NPDLOG(NPD_LOG_ERRO,"ResetBit错误的位置");
    }
}

void NPDDevBase::FlipBit(uint8_t &src, uint8_t pos)
{
    if( (pos>=1) && (pos<=8) )
    {
        uint8_t flag = src & (0x01 << (pos - 1));
        flag?ResetBit(src,pos):SetBit(src,pos);
    }
    else
    {
        NPDLOG(NPD_LOG_ERRO,"FlipBit错误的位置");
    }
}

void NPDDevBase::SetBit(std::bitset<8> &src, uint8_t pos)
{
    src.set(pos-1);
}

void NPDDevBase::ResetBit(std::bitset<8> &src, uint8_t pos)
{
    src.reset(pos-1);
}

void NPDDevBase::FlipBit(std::bitset<8> &src, uint8_t pos)
{
    src.flip(pos-1);
}