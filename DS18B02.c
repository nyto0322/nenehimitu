#include <STC90C5xAD.H>
#include <intrins.h>
#include "OneWire.h"

#define DS_SKIP_ROM 0xCC
#define DS_CONVERT_T 0x44
#define DS_READ_SCR 0xBE//scratchpad


void DS_ConvertT()
{
	OW_Init();
	OW_SendByte(DS_SKIP_ROM);
	OW_SendByte(DS_CONVERT_T);
}

float DS_ReadT()	//注意float运算耗时较长
{
	unsigned char tL,tM;
	int tem;
	float t;
	OW_Init();
	OW_SendByte(DS_SKIP_ROM);
	OW_SendByte(DS_READ_SCR);
	tL=OW_ReceiveByte();
	tM=OW_ReceiveByte();
	
	tem = (tM<<8)|tL;//bit4位为2的零次，右移四位即/16		tM前五位表符号，故用有符号int强转		
	t=((double)tem)/16;	//强转float以保留小数	float占4字节，int占2字节 前方五位S符号位是否影响负数转换？
	return t;
}
