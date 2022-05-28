/**
 * @file NPDSerial.cpp
 * @author BlueRabbit (515704389@protonmail.com)
 * @brief Linux平台串口设备文件底层读写
 * @version 0.1
 * @date 2022-02-28 03:43:23 
 * 
 * @copyright Copyright (c) 2022 BlueRabbit
 * 
 */

#include "NPDSerial.h"

#include <termios.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <NPDError.h>

using namespace NPD::NPDDEV;

const int speed_arr[] = { B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300 };
const int name_arr[] = { 115200, 38400, 19200, 9600, 4800, 2400, 1200,  300, 115200, 38400, 19200, 9600, 4800, 2400, 1200,  300 };

bool NPDSerial::set_speed(int speed)
{
    int status;
    struct termios Opt;
    tcgetattr(_serial_fd, &Opt); //读取终端参数
    for(int i= 0;i < sizeof(speed_arr) / sizeof(int);i++)
    {
        if(speed == name_arr[i])
        {
            tcflush(_serial_fd, TCIOFLUSH);
            cfsetispeed(&Opt, speed_arr[i]); //设置输入波特率
            cfsetospeed(&Opt, speed_arr[i]); //设置输出波特率
            status = tcsetattr(_serial_fd, TCSANOW, &Opt);//设置终端参数
            if(status != 0)
            {
                AUTO_RETURN_ERROR_HANDLER(NPD_ERROR_DEV_SER,false,NPD_ERROR_DEV_SER_PERROR);
            }
            tcflush(_serial_fd,TCIOFLUSH);
        }
    }
    return true;
}

bool NPDSerial::set_parity(int databits,int stopbits,int parity)
{
	struct termios options;
	if(tcgetattr(_serial_fd,&options) != 0)
    {
		AUTO_RETURN_ERROR_HANDLER(NPD_ERROR_DEV_SER,false,NPD_ERROR_DEV_SER_PARITY_I);
	}

	options.c_cflag &= ~CSIZE;
	switch (databits) /*设置数据位数*/
	{
        case 7:	options.c_cflag |= CS7;break;
        case 8: options.c_cflag |= CS8;break;
	    default:{
            AUTO_RETURN_ERROR_HANDLER(NPD_ERROR_DEV_SER,false,NPD_ERROR_DEV_SER_UNDATA);
        }
	}

	switch (parity)
	{   
		case 'n':
		case 'N':{
            options.c_cflag &= ~PARENB;    /* Clear parity enable */
			options.c_iflag &= ~INPCK;     /* Enable parity checking */
        };break;
		case 'o':
		case 'O':{
            options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/  
			options.c_iflag |= INPCK;             /* Disnable parity checking */ 
        };break;
		case 'e':
		case 'E':{
            options.c_cflag |= PARENB;     /* Enable parity */    
			options.c_cflag &= ~PARODD;    /* 转换为偶效验*/     
			options.c_iflag |= INPCK;      /* Disnable parity checking */
        };break;
		case 's':
		case 'S':{
            options.c_cflag &= ~PARENB; /*as no parity*/ 
			options.c_cflag &= ~CSTOPB;
        };break;
		default:{
            AUTO_RETURN_ERROR_HANDLER(NPD_ERROR_DEV_SER,false,NPD_ERROR_DEV_SER_UNPARITY);
        }
	}

	// 设置停止位
	switch (stopbits)
	{   
		case 1:options.c_cflag &= ~CSTOPB;break;  
		case 2:options.c_cflag |=  CSTOPB;break;
		default:{
            AUTO_RETURN_ERROR_HANDLER(NPD_ERROR_DEV_SER,false,NPD_ERROR_DEV_SER_UNSTOP);
        }
	}

	/* Set input parity option */
	if (parity != 'n')
		options.c_iflag |= INPCK;
	tcflush(_serial_fd,TCIFLUSH);

    //时间控制，原始模式有效
	options.c_cc[VTIME] = 10*10; /* 设置超时10 seconds(单位1/10s 100ms)*/
	options.c_cc[VMIN] = 0;

    // options.c_cc[VMIN] = FRAME_MAXSIZE;//阻塞条件下有效
    //如果不是开发终端之类的，只是串口传输数据，而不需要串口来处理，那么使用原始模式(Raw Mode)方式来通讯，~ICANON设置串口为原始模式,设置方式如下：
    options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
    options.c_oflag  &= ~OPOST;   /*Output*/

    /* Update the options and do it NOW */
	if (tcsetattr(_serial_fd,TCSANOW,&options) != 0)
	{
        AUTO_RETURN_ERROR_HANDLER(NPD_ERROR_DEV_SER,false,NPD_ERROR_DEV_SER_PARITY_II);
	}
	return true;
}

// 读取数据
int NPDSerial::read(void *src, int size, int param)
{
    FD_IS_READY(_serial_fd);
    std::lock_guard<std::mutex> LOCK_Rx(_lock_Rx);
    return ::read(_serial_fd, src, size);
}

// 写入数据
int NPDSerial::write(void *dest, int size, int param)
{
    FD_IS_READY(_serial_fd);
    std::lock_guard<std::mutex> LOCK_Tx(_lock_Tx);
    return ::write(_serial_fd, dest, size);
}

// 读取一字节数据
int NPDSerial::read_8bit(uint8_t &dest, int param)
{
    FD_IS_READY(_serial_fd);
    std::lock_guard<std::mutex> LOCK_Rx(_lock_Rx);
    return ::read(_serial_fd, &dest, sizeof(uint8_t));
}

// 写入一字节数据
int NPDSerial::write_8bit(uint8_t src, int param)
{
    FD_IS_READY(_serial_fd);
    std::lock_guard<std::mutex> LOCK_Tx(_lock_Tx);
    return ::write(_serial_fd, &src, sizeof(uint8_t));
}

// 用于各外设独有的配置选项
int NPDSerial::dev_init_setting()
{
    FD_IS_READY(_serial_fd);
    set_speed(BAUDRATE);  //设置波特率
    set_parity(8, 1 ,'N');// 8位数据位 1位停止位 无校验位
    return NPD_ERROR_NOERROR;
}

// 打开设备文件
bool NPDSerial::nfopen(std::string devname)
{
    // O_RDWR:可读可写
    // O_NOCTTY:该参数不会使打开的文件成为该进程的控制终端。如果没有指定这个标志，那么任何一个 输入都将会影响用户的进程
    // O_NDELAY:这个程序不关心DCD信号线所处的状态,端口的另一端是否激活或者停止。如果用户不指定了这个标志，则进程将会一直处在睡眠状态，直到DCD信号线被激活
    _serial_fd = open(devname.c_str(), O_RDWR|O_NOCTTY/*|O_NDELAY*/);
    if(_serial_fd < 0)
    {
        AUTO_RETURN_ERROR_HANDLER(NPD_ERROR_DEV_SER,false,NPD_ERROR_DEV_SER_PERROR);
    }
    set_fd(_serial_fd);
    return true;
}

// 关闭设备文件
bool NPDSerial::nfclose()
{
    if(_serial_fd >= 0)
    {
        close(_serial_fd);
        _serial_fd = -1;
    }
    return true;
}

// 读取函数
int NPDSerial::readfromIO()
{
    char buf[1024];
    int ret = read(buf,sizeof(buf));
    if(ret == 0)
    {/* 无数据 */
        return NPD_ERROR_DEV_SER_NODATA;
    }
    else if(ret > 0)
    {/* 有数据 */
        return NPD_ERROR_NOERROR;
    }
    else
    {/* 发生了错误 */
        AUTO_RETARB_ERROR_HANDLER(NPD_ERROR_DEV_SER,NPD_ERROR_DEV_SER,false,NPD_ERROR_DEV_SER_READ,&ret);
    }
}

// 执行任务函数
int NPDSerial::execute_task()
{
    return NPD_ERROR_NOERROR;
}