#ifndef _NPD_OLED_H_
#define _NPD_OLED_H_

#include "NPDDataType.h"

#define OLED_CMD  0  //写命令
#define OLED_DATA 1  //写数据
#define OLED_MODE 0

// OLED端口定义
// sbit OLED_SCL=P2^0;//时钟 D0(SCLK)
// sbit OLED_SDIN=P2^1;//D1(MOSI) 数据

int OLED_SCL;
int OLED_SDIN;

#define OLED_DC_Clr() OLED_DC = 0
#define OLED_DC_Set() OLED_DC = 1

#define OLED_CS_Clr() OLED_CS = 0
#define OLED_CS_Set() OLED_CS = 1

#define OLED_RST_Clr() OLED_RST = 0
#define OLED_RST_Set() OLED_RST = 1

#define OLED_SCLK_Clr() OLED_SCL = 0
#define OLED_SCLK_Set() OLED_SCL = 1

#define OLED_SDIN_Clr() OLED_SDIN = 0
#define OLED_SDIN_Set() OLED_SDIN = 1

// OLED模式设置
// 0:4线串行模式
// 1:并行8080模式

#define SIZE 16
#define XLevelL 0x02
#define XLevelH 0x10
#define Max_Column 128
#define Max_Row 64
#define Brightness 0xFF
#define X_WIDTH 128
#define Y_WIDTH 64

namespace NPD
{
    class NPDOLED
    {
    // 外部调用函数
    public:
        /**
        *  @brief  初始化OLED屏幕(SSD1306).
        *  @return  无返回.
        */
        void OLED_Init(void);
        /**
        *  @brief  清屏,清屏后整个屏幕和没点亮一样是黑色的.
        *  @return  无返回.
        */
        void OLED_Clear(void);
        /**
        *  @brief  在指定位置显示一个字符.
        *  @param  x  起始x坐标(范围0~127).
        *  @param  y  起始y坐标(范围0~63).
        *  @param  ch 要显示的字符.
        *  @param  csize 字体大小 16/12.
        *  @return  无返回.
        */
        void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t ch, uint8_t csize);
        /**
        *  @brief  显示数字.
        *  @param  x  起始x坐标(范围0~127).
        *  @param  y  起始y坐标(范围0~63).
        *  @param  num  要显示的数字(0~4294967295).
        *  @param  len  数字的位数.
        *  @param  csize  字体大小 16/12
        *  @return  无返回.
        */
        void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t csize);
        /**
        *  @brief  显示字符串.
        *  @param  x  起始x坐标(范围0~127).
        *  @param  y  起始y坐标(范围0~63).
        *  @param  str  要显示的字符串.
        *  @param  csize  字体大小 16/12
        *  @return  无返回.
        */
        void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *str, uint8_t csize);
        /**
        *  @brief  显示一个汉字.
        *  @param  x  起始x坐标(范围0~127).
        *  @param  y  起始y坐标(范围0~63).
        *  @param  ch 要显示的汉字
        *  @return  无返回.
        */
        void OLED_ShowChinese(uint8_t x, uint8_t y, uint32_t ch);
        /**
        *  @brief  显示显示BMP(128×64)图片.
        *  @param  x0  起始x坐标(范围0~127).
        *  @param  y0  起始页(0～7).
        *  @param  x1  结束x坐标(范围0~127).
        *  @param  y1  结束页(0～7).
        *  @param  BMP 图片字表
        *  @return  无返回.
        */
        void OLED_ShowBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[]);
        /**
        *  @brief  开启OLED显示.
        *  @return  无返回.
        */
        void OLED_display_on();
        /**
        *  @brief  关闭OLED显示.
        *  @return  无返回.
        */
        void OLED_display_off(void);
        /**
        *  @brief  毫秒延时函数.
        *  @param  ms  要延时的时间.
        *  @return  无返回.
        */
        void delay_ms(uint32_t ms);
    // 内部函数
    private:
        /**
        *  @brief  IIC总线启动传输.
        *  @return  无返回.
        */
        void IIC_Start();
        /**
        *  @brief  IIC总线停止传输.
        *  @return  无返回.
        */
        void IIC_Stop();
        /**
        *  @brief  IIC总线等待传输.
        *  @return  无返回.
        */
        void IIC_Wait_Ack();
        /**
        *  @brief  向IIC总线写入一个字节的数据.
        *  @param  IIC_Byte  要写入的数据.
        *  @return  无返回.
        */
        void Write_IIC_Byte(uint8_t IIC_Byte);
        /**
        *  @brief  向IIC总线写入数据.
        *  @param  IIC_Data  要写入的数据(1字节).
        *  @return  无返回.
        */
        void Write_IIC_Data(uint8_t IIC_Data);
        /**
        *  @brief  向IIC总线写入命令.
        *  @param  IIC_Command  要写入的命令(1字节).
        *  @return  无返回.
        */
        void Write_IIC_Command(uint8_t IIC_Command);
        /**
        *  @brief  OLED写入内容.
        *  @param  dat  要写入的内容(1字节).
        *  @param  cmd  OLED控制命令.
        *  @return  无返回.
        */
        void OLED_WR_Byte(uint8_t dat, uint8_t cmd);
        /**
        *  @brief  OLED设置显示起始位置.
        *  @param  x  x坐标.
        *  @param  y  y坐标.
        *  @return  无返回.
        */
        void OLED_Set_Pos(uint8_t x, uint8_t y);
        /**
        *  @brief  填充图片.
        *  @param  fill_Data  填充的数据.
        *  @return  无返回.
        */
        void Fill_picture(uint8_t fill_Data);
        /**
        *  @brief  计算m的n次方.
        *  @param  m  底数.
        *  @param  n  指数.
        *  @return  返回计算结果.
        */
        uint32_t OLED_Pow(uint8_t m, uint8_t n);
        // void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t);
        // void OLED_Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);
    };
}

#endif /* _NPD_OLED_H_ */