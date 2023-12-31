#include <STC90C5xAD.H>
#include "DS18B02.h"
#include "LCD1602.h"
#include "Delay.h"
#include <INTRINS.H>

//#define PRC	//CL模式预编译

//P1				btn
//P2_0			bzr
//P0 P2_567	1602
//P3_456		595
//P3_7			18B02



//蜂鸣器数据
#define	Q 0
#define	F4P 7
#define	G4P 9
#define	A4 10
#define	A4P 11
#define	B4 12
#define	C 13
#define	CP 14
#define	D 15
#define	DP 16
#define	E 17
#define	F 18
#define	FP 19
#define	G 20
#define	GP 21
#define	A 22
#define	B 24
#define SPEED_BS 3

//马达
#define SPEED_MS 200
sbit MOTOA = P3^0;
sbit MOTOB = P3^1;
sbit MOTOC = P3^2;
sbit MOTOD = P3^3;
void playMoto()
{	
		MOTOA = 1;
		MOTOB = 0;
		MOTOC = 1;
		MOTOD = 1;
		Delay(SPEED_MS);

		MOTOA = 1;
		MOTOB = 1;
		MOTOC = 1;
		MOTOD = 0;
		Delay(SPEED_MS);

		MOTOA = 0;
		MOTOB = 1;
		MOTOC = 1;
		MOTOD = 1;
		Delay(SPEED_MS);

		MOTOA = 1;
		MOTOB = 1;
		MOTOC = 0;
		MOTOD = 1;
		Delay(SPEED_MS);						
}
	
	
	
//蜂鸣器数据
sbit Bzr = P2^0;
float tpr;
unsigned char mProcess,freqS;
char isPlaying = 0;

unsigned char code ALENI[]={Q,5,C,1,Q,8,C,2,
D,6,C,2,Q,1,D,3,E,2,Q,2,
FP,1,E,2,Q,2,D,2,C,3,Q,6,
Q,2,C,2,Q,8,C,2,D,6,
C,2,Q,1,D,2,C,2,Q,2,A4P,2,G4P,2,
Q,2,F4P,1,Q,12,
C,2,Q,7,C,2,D,6,
C,2,Q,2,D,2,E,2,Q,2,FP,2,E,1,Q,2,
Q,1,D,2,C,3,Q,3,G4P,2,Q,2,A4P,1,C,2,
Q,2,G4P,2,C,2,Q,2,G4P,2,C,6,
GP,5,FP,11,
Q,12,G,4,
F,2,DP,2,Q,2,CP,2,B4,4,B4,5,
Q,2,G,2,Q,2,F,2,DP,2,Q,1,CP,2,
B4,5,B4,5,Q,2,A4,2,Q,2,B4,2,
CP,2,Q,1,DP,3,F,2,Q,1,G,2,A,2,Q,2,B,2,
A,3,G,2,F,2,Q,2,DP,2,CP,5,
CP,5,Q,1,G,3,Q,1,F,2,DP,3,
Q,1,CP,2,B4,4,B4,5,Q,2,G,2,
Q,2,F,2,DP,2,Q,2,CP,2,B4,4,B4,4,
B4,2,Q,1,CP,3,Q,1,DP,2,F,2,Q,2,G,2,
A,1,Q,2,G,2,F,2,Q,2,DP,2,CP,6,
CP,6,B4,2,Q,6,0xFF};

unsigned int code bzrFreq[]={
	0,
	63628,63731,63835,63928,64021,64103,64185,64260,64331,64400,64463,64528,
	64580,64633,64684,64732,64777,64820,64860,64898,64934,64968,65000,65030,
	65058,65085,65110,65134,65157,65178,65198,65217,65235,65252,65268,65283,
};


void Timer0_Init(void)
{
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TL0 = 0x00;
	TH0 = 0x00;
	TF0 = 0;
	TR0 = 1;
	ET0 = 1;
	EA = 1;
	PT0 = 0;
}

//蜂鸣器报警
void playALENI()
{
	if(isPlaying)
	{
		if(ALENI[mProcess]!=0xFF)	
		{
			freqS=ALENI[mProcess];	
			mProcess++;
			Delay(SPEED_BS/4*ALENI[mProcess]);	
			mProcess++;
			TR0=0;
			Delay(5);
			TR0=1;
		}
		else	
		{
			TR0=0;
			isPlaying=0;
		}
	}
}

void playPre(unsigned int pr)
{
	if(isPlaying)
	{
		pr<200?(freqS=(pr-100)/3+1):(freqS=36);	
		Delay(100);	
		TR0=0;
		Delay(100);
		TR0=1;
	}
	else
	{
		TR0=0;
		isPlaying=0;
	}
	
}


//595芯片输入
sbit mRCLK=P3^5;	
sbit mSRCLK=P3^6;	
sbit mSER=P3^4;

void hc595_SendByte(unsigned char c1,unsigned char c2,unsigned char c3,unsigned char c4)
{
	unsigned char a;

	mSRCLK = 1;
	mRCLK = 1;

	for(a=0;a<8;a++)		 
	{
		mSER = c1&(0x80>>a);		
		mSRCLK = 0;		
		_nop_();
		_nop_();
		mSRCLK = 1;	
	}

	for(a=0;a<8;a++)	
	{
		mSER = c2&(0x80>>a);		 
		mSRCLK = 0;		
		_nop_();
		_nop_();
		mSRCLK = 1;	
	}

	for(a=0;a<8;a++)	
	{
		mSER = c3&(0x80>>a);		
		mSRCLK = 0;		
		_nop_();
		_nop_();
		mSRCLK = 1;	
	}

	for(a=0;a<8;a++)		
	{
		mSER = c4&(0x80>>a);		
		mSRCLK = 0;		
		_nop_();
		_nop_();
		mSRCLK = 1;	
	}

	mRCLK = 0;
	_nop_();
	_nop_();
	mRCLK = 1;
	
	mSER=0;
}

//LED点阵报警
int time=0;
unsigned int code nene1[32]={0x00,0x00,0x03,0xC0,0x0F,0xF0,0x1F,0xFC,0x3F,0xFE,0x3F,0xFF,0x7F,0xFF,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xFF,0xF8,0xEF,0xF8,0xE7,0xF0,0xE7,0xE0,0xE7,0x00,0x6F,0x80};
unsigned int code nene2[32]={0x00,0x00,0x03,0xC0,0x0F,0xF0,0x1F,0xFC,0x3F,0xFE,0x3F,0xFF,0xFF,0xFF,0xFF,0xFE,0xFF,0xFE,0xFF,0xFE,0xEF,0xF8,0xEF,0xF8,0xE3,0x10,0xE7,0x80,0xFF,0xE0,0x6F,0x80};
void playLED()
{		
	int i;
	int j;
	if(time++%2)
	{
		for(j=0;j<8;j++)
		{		
			for(i=0;i<16;i++)
			{
				hc595_SendByte(~nene1[2*i],~nene1[2*i+1],i<8?0x00:0x01<<i%8,i<8?0x01<<i:0x00);
			}
		}
		for(j=0;j<8;j++)
		{
			for(i=0;i<16;i++)
			{
				hc595_SendByte(~nene2[2*i],~nene2[2*i+1],i<8?0x00:0x01<<i%8,i<8?0x01<<i:0x00);
			}				
		}	
	}
	else
	{
		for(j=0;j<8;j++)
		{
			for(i=0;i<16;i++)
			{
				hc595_SendByte(~nene2[2*i],~nene2[2*i+1],i<8?0x00:0x01<<i%8,i<8?0x01<<i:0x00);
			}				
		}	
		for(j=0;j<8;j++)
		{		
			for(i=0;i<16;i++)
			{
				hc595_SendByte(~nene1[2*i],~nene1[2*i+1],i<8?0x00:0x01<<i%8,i<8?0x01<<i:0x00);
			}
		}
	}
}

unsigned int code OM[]={0xDC,0XC0,0XBC,0XC9,0XB5,0XC5,0XC6,0XB0,0XA6,0XD0,0XC3,0XB8,0XC0,0XDE,0XBB,0XB2};

unsigned char PCR = 0;
void main()
{
	float t0;
	float k;
	float prec;
	int i;
	char tH = 30;
	char tB = 0;
	unsigned char precH = 2;

	
	LCD_Init();
	LCD_ShowCiallo(1,1);
	Timer0_Init();
	DS_ConvertT();//先转换一次，覆盖默认值
	Delay(1000);//>750ms
	
	t0 = DS_ReadT();

	while(1)
	{
		DS_ConvertT();
		tpr = DS_ReadT();
		
		if(tpr<0)tpr=-tpr;//补码转换

		
		if(PCR)	//两种模式下LCD显示
		{
			//k = (1-0.721)/(tH-t0);
			//prec = k*(tpr-t0)+0.721;
			
			k = 1.0/(tH-tB);
			prec = k*(tpr-tB);

			
			if(prec<precH)
			{
				LCD_ShowString(1,1,"Precent:   H:   ");
				LCD_ShowNum(1,14,tH,2);
				LCD_ShowNum(2,1,prec*100,3);
				LCD_ShowChar(2,4,'.');
				LCD_ShowNum(2,5,(unsigned int)(prec*1000)%10,1);
				LCD_ShowChar(2,6,'%');
			}
			else
			{
				LCD_ShowNum(2,1,999,3);
				LCD_ShowChar(2,4,'.');
				LCD_ShowNum(2,5,9,1);
				LCD_ShowChar(2,6,'%');
				LCD_ShowString(1,1,"Too High!! H:   ");
				LCD_ShowNum(1,14,tH,2);

//				for(i=0;i<16;i++)
//				{
//					LCD_ShowChar(1,1+i,OM[i]);
//				}
			}
			for(i=prec*10;i>0;i--)
			{
				if(i<11)LCD_ShowChar(2,6+i,0xFF);
			}
			for(i=10;i>prec*10;i--)
			{
				LCD_ShowChar(2,6+i,0xFE);
			}
		}
		else
		{
			LCD_ShowString(1,1,"Temperature:    ");
			LCD_ShowString(2,10,"  H:   ");
			LCD_ShowNum(2,14,tH,2);
			LCD_ShowChar(2,1,tpr<0?'-':'+');
			LCD_ShowNum(2,2,tpr,3);
			LCD_ShowChar(2,5,'.');
			LCD_ShowNum(2,6,(unsigned long)(tpr*10000)%10000,4);//四位小数
		}
	
		
		if(tpr>tH)		//触发警报
		{
			isPlaying=1;//为了防止从头放起
			if(PCR)
			{
				playPre((unsigned int)(prec*100));
			}
			else
			{
				playALENI();//得不断循环他，（断续的……）
			}
			playLED();
			playMoto();
		}
		else
		{
			isPlaying=0;
			TR0=0;
			freqS=0;
		}
		
		//按钮检测
		if(P10==0)
		{
			Delay(20);
			while(P10==0)
			Delay(20);
			PCR=!PCR;			
		}
		if(P11==0)
		{
			Delay(20);
			while(P11==0)
			Delay(20);
			if(tH>0)tH--;			
		}
		if(P12==0)
		{
			Delay(20);
			while(P12==0)
			Delay(20);
			tH++;			
		}
		if(P13==0)
		{
			Delay(20);
			while(P13==0)
			Delay(20);
			tH=30;			
		}
	}
}

void Timer0_Routine() interrupt 1
{
	if(bzrFreq[freqS])	
	{
		TL0 = bzrFreq[freqS]%256;		
		TH0 = bzrFreq[freqS]/256;		
		Bzr=!Bzr;	
	}
}