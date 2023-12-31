#include <STC90C5xAD.H>
#include <intrins.h>

sbit OW_DQ = P3^7;

void Delay500us()		//@12.000MHz
{
	unsigned char data i;

	_nop_();
	i = 247;
	while (--i);
}
void Delay70us()		//@12.000MHz
{
	unsigned char data i;

	_nop_();
	i = 32;
	while (--i);
}
void Delay14us()		//@12.000MHz
{
	unsigned char data i;

	_nop_();
	i = 4;
	while (--i);
}
void Delay50us()		//@12.000MHz
{
	unsigned char data i;

	_nop_();
	i = 22;
	while (--i);
}




unsigned char OW_Init()
{
	unsigned char tem;
	unsigned char EAtem = EA;
	EA=0;		//注意单总线时序过程不能被定时器打断！关闭定时器（极大地影响定时器：单总线较IIC的劣势
	OW_DQ=1;
	OW_DQ=0;
	Delay500us();
	OW_DQ=1;
	Delay70us();
	tem=OW_DQ;//响应了吗
	Delay500us();
	EA=EAtem;
	return tem;
	
}

void OW_SendBit(bit b)
{
	unsigned char i;
	unsigned char EAtem = EA;
	EA=0;
	OW_DQ=0;
	i=4;while(--i);//0,1分界
	OW_DQ=b;
	i=24;while(--i);//要保证大于60
	OW_DQ=1;
	EA=EAtem;
}

bit OW_ReceiveBit()
{ 
	bit b;
	unsigned char i;
	unsigned char EAtem = EA;
	EA=0;
	OW_DQ=0;
	//-1 /2 
	i=2;while(--i);//5um
	OW_DQ=1;
	i=2;while(--i);
	b=OW_DQ;
	i=24;while(--i);
	EA=EAtem;
	return b;
}

void OW_SendByte(unsigned char byte)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		OW_SendBit(byte&(0x01<<i));//注意OW从低位开始传输，与IIC不同
	}
}

unsigned char OW_ReceiveByte()
{
	unsigned char i;
	unsigned char byte=0x00;//温度错误显示！！！
	for(i=0;i<8;i++)
	{
		if(OW_ReceiveBit())byte|=(0x01<<i);
	}
	return byte;
}
