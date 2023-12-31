#ifndef __ONEWIRE_H__
#define __ONEWIRE_H__

unsigned char OW_Init();
void OW_SendBit(bit b);

bit OW_ReceiveBit();

void OW_SendByte(unsigned char byte);

unsigned char OW_ReceiveByte();


#endif