#include "NPDOLED.h"

#include <chrono>
#include <thread>

#include "NPDMCUFont.h"
/**
 * OLED的显存
 * 存放格式如下:
 * [0]0 1 2 3 ... 127
 * [1]0 1 2 3 ... 127
 * [2]0 1 2 3 ... 127
 * [3]0 1 2 3 ... 127
 * [4]0 1 2 3 ... 127
 * [5]0 1 2 3 ... 127
 * [6]0 1 2 3 ... 127
 * [7]0 1 2 3 ... 127
**/
using namespace NPD;

void NPDOLED::OLED_Init(void)
{
	OLED_WR_Byte(0xAE, OLED_CMD); // 关闭显示
	OLED_WR_Byte(0x00, OLED_CMD); // 设置低列地址
	OLED_WR_Byte(0x10, OLED_CMD); // 设置高列地址
	OLED_WR_Byte(0x40, OLED_CMD); // 设置起始行地址
	OLED_WR_Byte(0xB0, OLED_CMD); // 设置page地址
	OLED_WR_Byte(0x81, OLED_CMD); // 协议控制
	OLED_WR_Byte(0xFF, OLED_CMD); // 128
	OLED_WR_Byte(0xA1, OLED_CMD); // 设置段重映射
	OLED_WR_Byte(0xA6, OLED_CMD); // 正常/反向
	OLED_WR_Byte(0xA8, OLED_CMD); // 设置多路复用比率(1到64)
	OLED_WR_Byte(0x3F, OLED_CMD); // 1/32 duty
	OLED_WR_Byte(0xC8, OLED_CMD); // Com扫描方向
	OLED_WR_Byte(0xD3, OLED_CMD); // 设置显示偏移
	OLED_WR_Byte(0x00, OLED_CMD); //

	OLED_WR_Byte(0xD5, OLED_CMD); // 设置OSC分区
	OLED_WR_Byte(0x80, OLED_CMD); //

	OLED_WR_Byte(0xD8, OLED_CMD); // 关闭区域颜色模式
	OLED_WR_Byte(0x05, OLED_CMD); //

	OLED_WR_Byte(0xD9, OLED_CMD); // Set Pre-Charge Period
	OLED_WR_Byte(0xF1, OLED_CMD); //

	OLED_WR_Byte(0xDA, OLED_CMD); // 设置Com引脚配置
	OLED_WR_Byte(0x12, OLED_CMD); //

	OLED_WR_Byte(0xDB, OLED_CMD); // 设置Vcomh
	OLED_WR_Byte(0x30, OLED_CMD); //

	OLED_WR_Byte(0x8D, OLED_CMD); // charge pump使能
	OLED_WR_Byte(0x14, OLED_CMD); //

	OLED_WR_Byte(0xAF, OLED_CMD); // 打开OLED面板

	OLED_Clear();// 清空屏幕
}

void NPDOLED::OLED_Clear()
{
	uint8_t i, n;
	for (i = 0; i < 8; i++)
	{
		OLED_WR_Byte(0xb0 + i, OLED_CMD); //设置页地址（0~7）
		OLED_WR_Byte(0x00, OLED_CMD);	  //设置显示位置—列低地址
		OLED_WR_Byte(0x10, OLED_CMD);	  //设置显示位置—列高地址
		for (n = 0; n < 128; n++)
			OLED_WR_Byte(0, OLED_DATA);
	} //更新显示
}

void NPDOLED::OLED_ShowChar(uint8_t x, uint8_t y, uint8_t ch, uint8_t csize)
{
	uint8_t c = 0, i = 0;
	c = ch - ' '; //得到偏移后的值
	if (x > Max_Column - 1)
	{
		x = 0;
		y = y + 2;
	}
	if (csize == 16)
	{
		OLED_Set_Pos(x, y);
		for (i = 0; i < 8; i++)
			OLED_WR_Byte(OLED_F8X16[c * 16 + i], OLED_DATA);
		OLED_Set_Pos(x, y + 1);
		for (i = 0; i < 8; i++)
			OLED_WR_Byte(OLED_F8X16[c * 16 + i + 8], OLED_DATA);
	}
	else
	{
		OLED_Set_Pos(x, y);
		for (i = 0; i < 6; i++)
			OLED_WR_Byte(OLED_F6x8[c][i], OLED_DATA);
	}
}

void NPDOLED::OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t csize)
{
	uint8_t t, temp;
	uint8_t enshow = 0;
	for (t = 0; t < len; t++)
	{
		temp = (num / OLED_Pow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				OLED_ShowChar(x + (csize / 2) * t, y, ' ', csize);
				continue;
			}
			else
				enshow = 1;
		}
		OLED_ShowChar(x + (csize / 2) * t, y, temp + '0', csize);
	}
}

void NPDOLED::OLED_ShowString(uint8_t x, uint8_t y, uint8_t *str, uint8_t csize)
{
	uint8_t i = 0;
	while (str[i] != '\0')
	{
		OLED_ShowChar(x, y, str[i], csize);
		x += 8;
		if (x > 120)
		{
			x = 0;
			y += 2;
		}
		i++;
	}
}

void NPDOLED::OLED_ShowChinese(uint8_t x, uint8_t y, uint32_t ch)
{
	uint8_t t, adder = 0;
	OLED_Set_Pos(x, y);
	for (t = 0; t < 16; t++)
	{
		OLED_WR_Byte(OLED_Hzk[2 * ch][t], OLED_DATA);
		adder += 1;
	}
	OLED_Set_Pos(x, y + 1);
	for (t = 0; t < 16; t++)
	{
		OLED_WR_Byte(OLED_Hzk[2 * ch + 1][t], OLED_DATA);
		adder += 1;
	}
}

void NPDOLED::OLED_ShowBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[])
{
	uint32_t i = 0;
	uint8_t x, y;

	if (y1 % 8 == 0)
		y = y1 / 8;
	else
		y = y1 / 8 + 1;

	for (y = y0; y < y1; y++)
	{
		OLED_Set_Pos(x0, y);
		for (x = x0; x < x1; x++)
		{
			OLED_WR_Byte(BMP[i++], OLED_DATA);
		}
	}
}

void NPDOLED::OLED_display_on(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC命令
	OLED_WR_Byte(0X14, OLED_CMD); // DCDC ON
	OLED_WR_Byte(0XAF, OLED_CMD); // DISPLAY ON
}

void NPDOLED::OLED_display_off(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC命令
	OLED_WR_Byte(0X10, OLED_CMD); // DCDC OFF
	OLED_WR_Byte(0XAE, OLED_CMD); // DISPLAY OFF
}

void NPDOLED::delay_ms(uint32_t ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

uint32_t NPDOLED::OLED_Pow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;
	while (n--)
	{
		result *= m;
	}
	return result;
}

void NPDOLED::IIC_Start()
{

	OLED_SCLK_Set();
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

void NPDOLED::IIC_Stop()
{
	OLED_SCLK_Set();
	// OLED_SCLK_Clr();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
}

void NPDOLED::IIC_Wait_Ack()
{

	// GPIOB->CRH &= 0XFFF0FFFF;	//设置PB12为上拉输入模式
	// GPIOB->CRH |= 0x00080000;
	// OLED_SDA = 1;
	// delay_us(1);
	// OLED_SCL = 1;
	// delay_us(50000);
	/*	while(1)
		{
			if(!OLED_SDA)				//判断是否接收到OLED 应答信号
			{
				//GPIOB->CRH &= 0XFFF0FFFF;	//设置PB12为通用推免输出模式
				//GPIOB->CRH |= 0x00030000;
				return;
			}
		}
	*/
	OLED_SCLK_Set();
	OLED_SCLK_Clr();
}

void NPDOLED::Write_IIC_Byte(uint8_t IIC_Byte)
{
	uint8_t i;
	uint8_t m, da;
	da = IIC_Byte;
	OLED_SCLK_Clr();
	for (i = 0; i < 8; i++)
	{
		m = da;
		//	OLED_SCLK_Clr();
		m = m & 0x80;
		if (m == 0x80)
		{
			OLED_SDIN_Set();
		}
		else
			OLED_SDIN_Clr();
		da = da << 1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
	}
}

void NPDOLED::Write_IIC_Data(uint8_t IIC_Data)
{
	IIC_Start();
	Write_IIC_Byte(0x78); // D/C#=0; R/W#=0
	IIC_Wait_Ack();
	Write_IIC_Byte(0x40); // write data
	IIC_Wait_Ack();
	Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();
	IIC_Stop();
}

void NPDOLED::Write_IIC_Command(uint8_t IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78); // Slave address,SA0=0
	IIC_Wait_Ack();
	Write_IIC_Byte(0x00); // write command
	IIC_Wait_Ack();
	Write_IIC_Byte(IIC_Command);
	IIC_Wait_Ack();
	IIC_Stop();
}

void NPDOLED::OLED_WR_Byte(uint8_t dat, uint8_t cmd)
{
	if (cmd)
	{
		Write_IIC_Data(dat);
	}
	else
	{
		Write_IIC_Command(dat);
	}
}

void NPDOLED::OLED_Set_Pos(uint8_t x, uint8_t y)
{
	OLED_WR_Byte(0xb0 + y, OLED_CMD);
	OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
	OLED_WR_Byte((x & 0x0f), OLED_CMD);
}

void NPDOLED::Fill_picture(uint8_t fill_Data)
{
	uint8_t m, n;
	for (m = 0; m < 8; m++)
	{
		OLED_WR_Byte(0xb0 + m, 0); // page0-page1
		OLED_WR_Byte(0x00, 0);	   // low column start address
		OLED_WR_Byte(0x10, 0);	   // high column start address
		for (n = 0; n < 128; n++)
		{
			OLED_WR_Byte(fill_Data, 1);
		}
	}
}
