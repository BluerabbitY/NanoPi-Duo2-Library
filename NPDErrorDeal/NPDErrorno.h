#ifndef _NPD_ERROR_NO_H_
#define _NPD_ERROR_NO_H_

typedef enum {
    /** 日志错误 */
    NPD_LOG_INFO,
    NPD_LOG_WARN,
    NPD_LOG_ERRO,
    
    /** 没有错误 */
    NPD_ERROR_NOERROR,
    NPD_ERROR_ENO_NULL,

    /** 文件错误 */
    NPD_ERROR_FILE,
    NPD_REEOR_FILE_CSV,
    NPD_ERROR_FILE_XML,
    NPD_ERROR_FILE_JSON,

    /** 数据库错误 */
    NPD_ERROR_DATABASE,

    /** 设备错误 */
    NPD_ERROR_DEV,
    /** 设备文件不存在 */
    NPD_ERROR_DEV_NOFILE,
    /** 设备文件打开失败 */
    NPD_ERROR_DEV_OPENFAIL,
    /** 错误的设备模式 */
    NPD_ERRPR_DEV_EMODE,

    /** SPI错误 */
    NPD_ERROR_DEV_SPI,

    /** IIC错误 */
    NPD_ERROR_DEV_IIC,
    /** IIC perror错误 */
    NPD_ERROR_DEV_IIC_PERROR,
    /** I2C_TENBIT 错误 */
    NPD_ERROR_DEV_IIC_PERROR_I,
    /** I2C_SLAVE 错误 */
    NPD_ERROR_DEV_IIC_PERROR_II,
    /** IIC malloc错误 */
    NPD_ERROR_DEV_IIC_MALLOC,
    /** IIC msg模式写入错误 */
    NPD_ERROR_DEV_IIC_EWRITE,
    /** IIC msg模式读取错误 */
    NPD_ERROR_DEV_IIC_EREAD,
    /** IIC无数据 */
    NPD_ERROR_DEV_IIC_NODATA,
    /** IIC读数据错误 */
    NPD_ERROR_DEV_IIC_READ,

    /** 串口错误 */
    NPD_ERROR_DEV_SER,
    /** 串口 perror错误 */
    NPD_ERROR_DEV_SER_PERROR,
    /** 串口tcgetattr错误I */
    NPD_ERROR_DEV_SER_PARITY_I,
    /** 串口tcgetattr错误II */
    NPD_ERROR_DEV_SER_PARITY_II,
    /** 串口数据位数错误 */
    NPD_ERROR_DEV_SER_UNDATA,
    /** 串口数据校验标志错误 */
    NPD_ERROR_DEV_SER_UNPARITY,
    /** 串口停止位错误 */
    NPD_ERROR_DEV_SER_UNSTOP,
    /** 串口无数据 */
    NPD_ERROR_DEV_SER_NODATA,
    /** 串口读数据错误 */
    NPD_ERROR_DEV_SER_READ,

    /** TCP网络错误 */
    NPD_ERROR_NET_TCP,
    /** TCP网络创建套接字错误(服务器Server) */
    NPD_ERROR_NET_TCP_S_ECREATE,
    /** TCP网络绑定套接字错误(服务器Server) */
    NPD_ERROR_NET_TCP_S_EBIND,
    /** TCP网络监听套接字错误(服务器Server) */
    NPD_ERROR_NET_TCP_S_ELISTEN,

    /** 协议错误 */
    NPD_ERROR_PROTOCOL,
    
}NPD_ERROR_TYPE;

#endif /* _NPD_ERROR_NO_H_ */