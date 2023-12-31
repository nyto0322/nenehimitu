#include <STC90C5xAD.H>
#include "Delay.h"

sbit mRD=P2^6;
sbit mWR=P2^5;
sbit mLCDE=P2^7;

void LCD_mWriteCommand(unsigned char c)
{
	mRD=0;
	mWR=0;
	P0=c;
	mLCDE=1;
	Delay(1);
	mLCDE=0;
	Delay(1);
}

void LCD_mWriteData(unsigned char d)
{
	mRD=1;
	mWR=0;
	P0=d;
	mLCDE=1;
	Delay(1);
	mLCDE=0;
	Delay(1);
}

void LCD_mSetCursor(unsigned char Line,unsigned char Column)
{
	if(Line==1)
	{
		LCD_mWriteCommand(0x80|(Column-1));
	}
	else if(Line==2)
	{
		LCD_mWriteCommand(0x80|(Column-1+0x40));
	}
}

void LCD_Init()
{
	LCD_mWriteCommand(0x38);
	LCD_mWriteCommand(0x0c);
	LCD_mWriteCommand(0x06);
	LCD_mWriteCommand(0x01);
}

void LCD_ShowChar(unsigned char Line,unsigned char Column,char Char)
{
	LCD_mSetCursor(Line,Column);
	LCD_mWriteData(Char);
}

void LCD_ShowString(unsigned char Line,unsigned char Column,char *String)
{
	unsigned char i;
	LCD_mSetCursor(Line,Column);
	for(i=0;String[i]!='\0';i++)
	{
		LCD_mWriteData(String[i]);
	}
}

void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	unsigned char j;

	LCD_mSetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		int pow=1;
		for(j=0;j<i-1;j++)
		{
			pow*=10;
		}

		LCD_mWriteData(Number/pow%10+'0');
	}
}





void LCD_ShowCiallo(unsigned char Line,unsigned char Column)
{
	unsigned char i;
	unsigned char code ciallo[] = {
		0x00,0x00,0x00,0x08,0x15,0x02,0x00,0x00,
		0x00,0x00,0x02,0x04,0x08,0x10,0x1F,0x00,
		0x00,0x00,0x00,0x06,0x06,0x00,0x00,0x00,
		0x00,0x00,0x11,0x11,0x15,0x0A,0x00,0x00,
		0x00,0x02,0x04,0x08,0x04,0x02,0x00,0x00,
		0x00,0x0E,0x11,0x00,0x00,0x00,0x00,0x00,
		0x00,0x04,0x0E,0x1F,0x0E,0x0A,0x11,0x00
	};
	unsigned char code cTable[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06};  //自定义字符数据地址

	LCD_mWriteCommand(0x40);//存入CGROM
	for(i=0;i<56;i++)
  {
		LCD_mWriteData(ciallo[i]);
		Delay(1);
	}

	LCD_ShowString(Line,Column,"Ciallo");
	//LCD_mWRiteCommand(0x80|6);
	Delay(1);
	LCD_mWriteData(cTable[0]);Delay(1);
	LCD_mWriteData('(');Delay(1);
	LCD_mWriteData(cTable[1]);Delay(1);
	LCD_mWriteData(cTable[2]);Delay(1);
	LCD_mWriteData(cTable[3]);Delay(1);
	LCD_mWriteData(cTable[4]);Delay(1);
	LCD_mWriteData(')');Delay(1);
	LCD_mWriteData(cTable[5]);Delay(1);
	LCD_mWriteData(cTable[6]);Delay(1);
}

