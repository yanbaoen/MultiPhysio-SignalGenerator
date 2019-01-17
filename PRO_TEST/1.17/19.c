#include<reg52.h>							//包含51单片机寄存器定义的头文件
#include<intrins.h>	
#define uint unsigned int
#define uchar unsigned char
#define Channal_A     1    //通道A
#define Channal_B     2    //通道B
#define Channal_AB    3    //通道A&B
#define MCP42100_ENABLE()		MCP42100_CS = 0//使能
#define MCP42100_DISABLE()		MCP42100_CS = 1//使能
#define MCP42100_SCK_H() 		MCP42100_SCK = 1//
#define MCP42100_SCK_L() 		MCP42100_SCK = 0//
#define MCP42100_SI_H() 		MCP42100_SI = 1//
#define MCP42100_SI_L() 		MCP42100_SI = 0//

#define MCP42100_NONE					0x10
#define MCP42100_P0						0x11
#define MCP42100_P1						0x12
#define MCP42100_ALL					0x13			
//**************************下为函数声明修**************************************
static void DA_conver(uint Dignum);
extern void Write_A_B(uint Data_A,uint Data_B,uchar Channal,bit Model);
void InitUART  (void);
void SendByte(unsigned char dat);
void SendStr(unsigned char *s);
void MCP42100_Control(uint _ch,uint _value);

sbit DIN   = P1^0;     //数据输入端
sbit SCLK = P1^1;     //时钟信号
sbit CS    = P1^2;     //片选输入端，低电平有效
sbit MCP42100_CS  = P1^6;
sbit MCP42100_SCK = P1^5;			
sbit MCP42100_SI  = P1^4;
static uint new = 1;
uint code ECG1[1000];

//=================================================================
//*******************  串口初始化*******************
void InitUART  (void)
{
    SCON  =0x50;       // SCON: 模式 1, 8-bit UART, 使能接收 
    TMOD |= 0x20;      // TMOD:timer 1, mode 2, 8-bit 重装
    TH1 =TL1= 0xFD;   // TH1:  重装值9600 波特率 晶振 11.0592MHz  
    TR1   =1;               // TR1: timer 1 打开                     
    EA   = 1;              //打开总中断
    ES   = 1;              //打开串口中断
}   
//******************* 发送一个字节 *******************
void SendByte(unsigned char dat)
{
   SBUF = dat;
   while(!TI);
   TI = 0;  
}
//**************** 发送一个字符串 *******************
void SendStr(unsigned char *s)
{
    while(*s!='\0')   // \0表示字符串结束标志，通过检测是否字符串末尾
    {
       SendByte(*s);
       s++;
    }
}
//****************** 串口中断程序 *******************
void UART_SER (void) interrupt 4 
{
   unsigned char Temp; 
     
   if(RI)                           //判断是接收中断产生
   {
    RI=0;                      //标志位清零
    Temp=SBUF;
	new = Temp;
    /* switch(Temp){
        case 0x00:
		new = 1;
		break;
        case 0x80: 
		new = 2;
		break;
        default:
		new = 3;
		break;
	} */
    SendByte(Temp);
	}  //把接收到的值再发回电脑端
}
uint code ECG1[1000]=					
{0x4CF,
0x4C7,
0x4CF,
0x4B7,
0x4C7,
0x4BF,
0x4BF,
0x49F,
0x4AF,
0x497,
0x4AF,
0x497,
0x4AF,
0x497,
0x4A7,
0x49F,
0x49F,
0x48F,
0x4A7,
0x487,
0x49F,
0x4E7,
0x538,
0x548,
0x578,
0x629,
0x611,
0x578,
0x538,
0x4EF,
0x4A7,
0x3EE,
0x3F6,
0x3EE,
0x3EE,
0x3AD,
0x45E,
0x753,
0xC72,
0xE6D,
0x436,
0x0,
0x17A,
0x2FC,
0x436,
0x41E,
0x44E,
0x436,
0x476,
0x466,
0x49F,
0x497,
0x4C7,
0x4D7,
0x517,
0x527,
0x568,
0x578,
0x5C0,
0x5E0,
0x621,
0x651,
0x679,
0x699,
0x6A2,
0x691,
0x6AA,
0x6A2,
0x671,
0x641,
0x5F1,
0x5A8,
0x570,
0x550,
0x538,
0x527,
0x527,
0x51F,
0x517,
0x51F,
0x52F,
0x527,
0x50F,
0x50F,
0x517,
0x507,
0x4F7,
0x4E7,
0x4E7,
0x4CF,
0x4C7,
0x4BF,
0x4D7,
0x4CF,
0x4AF,
0x4B7,
0x49F,
0x4C7,
0x4B7,
0x4AF,
0x4AF,
0x4A7,
0x49F,
0x4B7,
0x4A7,
0x497,
0x487,
0x49F,
0x497,
0x4BF,
0x4E7,
0x538,
0x560,
0x5A0,
0x669,
0x619,
0x580,
0x517,
0x507,
0x476,
0x40E,
0x3FE,
0x406,
0x406,
0x3D6,
0x540,
0x8CD,
0xCA2,
0xB38,
0x42E,
0x89,
0x12A,
0x2FC,
0x416,
0x446,
0x43E,
0x446,
0x487,
0x46E,
0x49F,
0x4A7,
0x4BF,
0x4EF,
0x50F,
0x538,
0x550,
0x578,
0x5B0,
0x5E8,
0x611,
0x639,
0x681,
0x661,
0x691,
0x681,
0x689,
0x689,
0x631,
0x609,
0x5D0,
0x580,
0x568,
0x527,
0x538,
0x527,
0x517,
0x50F,
0x51F,
0x52F,
0x50F,
0x527,
0x4EF,
0x4F7,
0x4DF,
0x4EF,
0x4D7,
0x4B7,
0x4DF,
0x4B7,
0x49F,
0x4AF,
0x49F,
0x4AF,
0x4B7,
0x4A7,
0x4A7,
0x497,
0x497,
0x487,
0x4AF,
0x476,
0x48F,
0x497,
0x48F,
0x4B7,
0x527,
0x540,
0x588,
0x659,
0x619,
0x5A8,
0x527,
0x507,
0x47E,
0x416,
0x40E,
0x3CD,
0x416,
0x3AD,
0x4FF,
0x884,
0xCCA,
0xCDA,
0x487,
0x30,
0xD9,
0x2D4,
0x426,
0x436,
0x456,
0x43E,
0x46E,
0x46E,
0x49F,
0x497,
0x4DF,
0x4DF,
0x51F,
0x527,
0x560,
0x5B0,
0x5A8,
0x619,
0x629,
0x671,
0x671,
0x6B2,
0x6AA,
0x6BA,
0x6BA,
0x689,
0x651,
0x621,
0x5C0,
0x568,
0x558,
0x52F,
0x517,
0x4FF,
0x52F,
0x50F,
0x527,
0x507,
0x4FF,
0x517,
0x51F,
0x4F7,
0x4EF,
0x4DF,
0x4E7,
0x4EF,
0x4B7,
0x4C7,
0x497,
0x4A7,
0x4A7,
0x49F,
0x48F,
0x48F,
0x4AF,
0x487,
0x487,
0x48F,
0x487,
0x497,
0x48F,
0x49F,
0x4F7,
0x540,
0x568,
0x5A0,
0x661,
0x5F9,
0x578,
0x4F7,
0x4CF,
0x436,
0x3DE,
0x3C5,
0x3EE,
0x3D6,
0x385,
0x5A0,
0xA9F,
0xE95,
0x91D,
0x26B,
0x10,
0x16A,
0x33D,
0x416,
0x40E,
0x426,
0x456,
0x456,
0x476,
0x487,
0x4A7,
0x4CF,
0x4FF,
0x50F,
0x548,
0x580,
0x5A0,
0x5D8,
0x639,
0x671,
0x679,
0x6AA,
0x6AA,
0x6DA,
0x6D2,
0x6AA,
0x689,
0x641,
0x5F9,
0x5C0,
0x558,
0x517,
0x4FF,
0x4F7,
0x4EF,
0x4EF,
0x4F7,
0x4EF,
0x50F,
0x4EF,
0x4EF,
0x4FF,
0x4E7,
0x4C7,
0x4CF,
0x4CF,
0x49F,
0x4AF,
0x497,
0x4A7,
0x487,
0x49F,
0x48F,
0x487,
0x476,
0x48F,
0x476,
0x47E,
0x456,
0x497,
0x466,
0x487,
0x4A7,
0x517,
0x550,
0x560,
0x609,
0x631,
0x5D8,
0x560,
0x4DF,
0x4DF,
0x41E,
0x3E6,
0x3D6,
0x416,
0x3DE,
0x3D6,
0x649,
0xC09,
0xFDF,
0x74A,
0x152,
0xC1,
0x24B,
0x416,
0x426,
0x446,
0x436,
0x476,
0x46E,
0x487,
0x487,
0x4A7,
0x4CF,
0x517,
0x527,
0x550,
0x580,
0x5B0,
0x5D8,
0x651,
0x679,
0x6B2,
0x6AA,
0x6E2,
0x6E2,
0x6F2,
0x6D2,
0x6A2,
0x649,
0x609,
0x5C0,
0x578,
0x52F,
0x527,
0x507,
0x507,
0x507,
0x50F,
0x52F,
0x527,
0x527,
0x527,
0x527,
0x51F,
0x507,
0x50F,
0x4EF,
0x4EF,
0x4EF,
0x4DF,
0x4CF,
0x4DF,
0x4C7,
0x4CF,
0x4BF,
0x4CF,
0x49F,
0x4AF,
0x497,
0x4A7,
0x48F,
0x49F,
0x497,
0x4A7,
0x48F,
0x4B7,
0x497,
0x4AF,
0x4B7,
0x52F,
0x560,
0x568,
0x609,
0x619,
0x5A0,
0x548,
0x4E7,
0x4D7,
0x406,
0x3FE,
0x3CD,
0x40E,
0x3DE,
0x406,
0x691,
0xC6A,
0xFFF,
0x629,
0xC9,
0x109,
0x2A4,
0x43E,
0x42E,
0x456,
0x446,
0x46E,
0x466,
0x49F,
0x49F,
0x4D7,
0x4EF,
0x517,
0x52F,
0x560,
0x580,
0x5C8,
0x5E0,
0x639,
0x661,
0x6A2,
0x6BA,
0x6CA,
0x6CA,
0x6DA,
0x6CA,
0x6A2,
0x659,
0x619,
0x5D0,
0x590,
0x558,
0x558,
0x540,
0x540,
0x540,
0x548,
0x540,
0x558,
0x550,
0x550,
0x550,
0x548,
0x52F,
0x52F,
0x51F,
0x517,
0x50F,
0x4FF,
0x4F7,
0x4F7,
0x4E7,
0x4EF,
0x4E7,
0x4E7,
0x4D7,
0x4E7,
0x4D7,
0x4E7,
0x4CF,
0x4DF,
0x4CF,
0x4D7,
0x4AF,
0x4C7,
0x49F,
0x4B7,
0x49F,
0x4BF,
0x4B7,
0x4C7,
0x4AF,
0x4C7,
0x4FF,
0x558,
0x570,
0x598,
0x651,
0x619,
0x588,
0x548,
0x4F7,
0x4CF,
0x416,
0x41E,
0x3F6,
0x416,
0x3CD,
0x476,
0x76B,
0xCFB,
0xF36,
0x476,
0x10,
0x1A2,
0x34D,
0x45E,
0x42E,
0x45E,
0x456,
0x47E,
0x46E,
0x49F,
0x4A7,
0x4D7,
0x4EF,
0x527,
0x52F,
0x570,
0x580,
0x5C8,
0x5F9,
0x641,
0x661,
0x691,
0x6A2,
0x6BA,
0x6C2,
0x6D2,
0x6B2,
0x691,
0x649,
0x609,
0x5B8,
0x588,
0x550,
0x548,
0x540,
0x540,
0x52F,
0x52F,
0x52F,
0x540,
0x538,
0x550,
0x540,
0x52F,
0x51F,
0x517,
0x50F,
0x50F,
0x507,
0x4F7,
0x4FF,
0x4E7,
0x4DF,
0x4DF,
0x4D7,
0x4C7,
0x4C7,
0x4CF,
0x4C7,
0x4BF,
0x4CF,
0x4CF,
0x4D7,
0x4CF,
0x4C7,
0x4AF,
0x4C7,
0x4AF,
0x4C7,
0x4B7,
0x4BF,
0x4A7,
0x4BF,
0x4A7,
0x4B7,
0x4A7,
0x4CF,
0x4E7,
0x550,
0x570,
0x588,
0x651,
0x629,
0x590,
0x550,
0x507,
0x4D7,
0x40E,
0x42E,
0x41E,
0x426,
0x3F6,
0x48F,
0x763,
0xC32,
0xE95,
0x4EF,
0x99,
0x14A,
0x2E4,
0x45E,
0x43E,
0x456,
0x43E,
0x46E,
0x45E,
0x487,
0x49F,
0x4D7,
0x4CF,
0x4F7,
0x517,
0x550,
0x578,
0x598,
0x5C8,
0x619,
0x651,
0x689,
0x671,
0x699,
0x6AA,
0x6BA,
0x6AA,
0x699,
0x649,
0x601,
0x5C0,
0x580,
0x568,
0x548,
0x538,
0x52F,
0x527,
0x52F,
0x52F,
0x52F,
0x52F,
0x527,
0x527,
0x50F,
0x50F,
0x4EF,
0x4DF,
0x4CF,
0x4CF,
0x4CF,
0x4BF,
0x4C7,
0x4B7,
0x4BF,
0x4C7,
0x4B7,
0x4B7,
0x4B7,
0x49F,
0x4B7,
0x4A7,
0x4AF,
0x49F,
0x4B7,
0x49F,
0x4AF,
0x49F,
0x4AF,
0x48F,
0x4A7,
0x48F,
0x4B7,
0x4A7,
0x49F,
0x4B7,
0x517,
0x558,
0x588,
0x629,
0x631,
0x5C8,
0x548,
0x4F7,
0x4DF,
0x416,
0x41E,
0x3FE,
0x416,
0x3EE,
0x487,
0x74A,
0xC11,
0xE6D,
0x598,
0x109,
0xD9,
0x26B,
0x406,
0x42E,
0x456,
0x456,
0x476,
0x476,
0x47E,
0x497,
0x4D7,
0x4DF,
0x517,
0x527,
0x578,
0x580,
0x5C8,
0x5F1,
0x639,
0x659,
0x681,
0x699,
0x6BA,
0x6B2,
0x6B2,
0x6AA,
0x681,
0x639,
0x5F9,
0x5A0,
0x578,
0x540,
0x550,
0x538,
0x52F,
0x52F,
0x52F,
0x52F,
0x52F,
0x527,
0x51F,
0x527,
0x507,
0x527,
0x4EF,
0x4EF,
0x4E7,
0x4E7,
0x4EF,
0x4D7,
0x4C7,
0x4DF,
0x4C7,
0x4C7,
0x4C7,
0x4C7,
0x4BF,
0x4C7,
0x4BF,
0x4C7,
0x4BF,
0x4BF,
0x4A7,
0x4CF,
0x49F,
0x4C7,
0x4A7,
0x4AF,
0x4A7,
0x4DF,
0x49F,
0x4B7,
0x4D7,
0x538,
0x588,
0x590,
0x631,
0x629,
0x5B0,
0x548,
0x50F,
0x4E7,
0x41E,
0x41E,
0x416,
0x42E,
0x3EE,
0x487,
0x773,
0xC7A,
0xEED,
0x5A0,
0xD9,
0x109,
0x29C,
0x436,
0x436,
0x456,
0x456,
0x487,
0x47E,
0x4A7,
0x49F,
0x4C7,
0x4E7,
0x527,
0x52F,
0x570,
0x598,
0x5D0,
0x5F1,
0x649,
0x661,
0x6A2,
0x6CA,
0x6E2,
0x6DA,
0x6EA,
0x6BA,
0x689,
0x649,
0x611,
0x5C8,
0x570,
0x558,
0x548,
0x52F,
0x52F,
0x52F,
0x52F,
0x52F,
0x52F,
0x52F,
0x52F,
0x527,
0x51F,
0x51F,
0x517,
0x4FF,
0x4F7,
0x4E7,
0x4E7,
0x4DF,
0x4E7,
0x4C7,
0x4BF,
0x4B7,
0x4BF,
0x49F,
0x4A7,
0x49F,
0x4BF,
0x4A7,
0x4A7,
0x497,
0x4A7,
0x49F,
0x4A7,
0x49F,
0x49F,
0x48F,
0x49F,
0x487,
0x49F,
0x48F,
0x4A7,
0x4D7,
0x52F,
0x568,
0x580,
0x641,
0x609,
0x590,
0x540,
0x4F7,
0x4B7,
0x3FE,
0x406,
0x3E6,
0x40E,
0x3C5,
0x47E,
0x793,
0xCD2,
0xEE5,
0x507,
0x61,
0x14A,
0x2DC,
0x436,
0x41E,
0x446,
0x42E,
0x45E,
0x446,
0x466,
0x46E,
0x4AF,
0x4C7,
0x4F7,
0x50F,
0x540,
0x568,
0x5B0,
0x5D8,
0x629,
0x651,
0x691,
0x699,
0x6B2,
0x6AA,
0x6BA,
0x699,
0x671,
0x629,
0x5E8,
0x5A8,
0x580,
0x550,
0x52F,
0x51F,
0x517,
0x51F,
0x51F,
0x527,
0x52F,
0x52F,
0x52F,
0x52F,
0x52F,
0x527,
0x51F,
0x50F,
0x507,
0x4F7,
0x4E7,
0x4D7,
0x4D7,
0x4CF,
0x4CF,
0x4CF,
0x4D7,
0x4D7,
0x4D7,
0x4C7,
0x4CF,
0x4D7
};

void DA_conver(uint Dignum)
{
uint Dig=0;
uchar i=0;
SCLK=1;
CS=0;           //片选有效
for(i=0;i<16;i++)   //写入16为Bit的控制位和数据
{
   Dig=Dignum&0x8000;
   if(Dig)
   {
    DIN=1;
   }
   else
   {
    DIN=0;
   }
   SCLK=0;
   _nop_();
   Dignum<<=1;
   SCLK=1;
   _nop_();
}
SCLK=1;
CS=1;       //片选无效
}

void MCP42100_Control(uint _ch,uint _value)
{	 
	uint i = 0;
	MCP42100_ENABLE();	   
	for(i = 0;i<8;i++)
	{
		MCP42100_SCK_L(); 
		if(_ch & (0x80>>i)) MCP42100_SI_H();
		else MCP42100_SI_L();
		_nop_();
		MCP42100_SCK_H();
		_nop_();
	}
	for(i = 0;i<8;i++)
	{
		MCP42100_SCK_L();
		if(_value & (0x80>>i)) MCP42100_SI_H();
		else MCP42100_SI_L();
		_nop_();
		MCP42100_SCK_H();
		_nop_();
	}
	MCP42100_SCK_L();	 			
	MCP42100_DISABLE();						
}
//=================================================================
// 函数名称 ：void Write_A_B(uint Data_A,uint Data_B,uchar Channal,bit Model)
// 函数功能 ：模式、通道选择并进行DA转换
// 入口参数 ：Data_A：A通道转换的电压值
//            Data_B：B通道转换的电压值
//            Channal：通道选择，其值为Channal_A，Channal_B,或Channal_AB
//            Model：速度控制位 0：slow mode 1：fast mode
// 出口参数 ：无
// 说明：     Data_A，Data_B的范围为：0—0x0fff
//            本程序如果只需要一个通道时，另外一个通道的值可任意，但是不能缺省
//=================================================================
void Write_A_B(uint Data_A,uint Data_B,uchar Channal,bit Model)
{
uint Temp;
if(Model)
{
   Temp=0x4000;
}
else
    {
   Temp=0x0000;
}
switch(Channal)
{
    case Channal_A:         //A通道
         DA_conver(Temp|0x8000|(0x0fff&Data_A));
      break;
    case Channal_B:       //B通道
         DA_conver(Temp|0x0000|(0x0fff&Data_B));
    break;
    case Channal_AB:
         DA_conver(Temp|0x1000|(0x0fff&Data_B));        //A&B通道
         DA_conver(Temp|0x8000|(0x0fff&Data_A));
    break;
    default:
         break;
}
}
//=================================================================
// 函数名称 ：int main(void)
// 函数功能 ：主函数，无实际意义,仅测试TLV5618用
// 入口参数 ：无 1-A;2-B;3-A&B
// 出口参数 ：无
//=================================================================
int main(void)
{
	uint i; 
	InitUART();
   while(1)
   {
/* 	switch(new){
        case 1:
		for(i=0;i<999;i++)
		Write_A_B(ECG1[i],ECG1[i],Channal_AB,1);
		break;
        case 2: 
		for(i=0;i<999;i++)
		Write_A_B(ECG1[i]/10,ECG1[i]/10,Channal_AB,1);
		break;
		case 3: 
		for(i=0;i<999;i++)
		Write_A_B(ECG1[i]/new,ECG1[i]/new,Channal_AB,1);
		break;
        default:
		for(i=0;i<999;i++)
		Write_A_B(ECG1[i]/5,ECG1[i]/5,Channal_AB,1);
		break; 
    }   */
		for(i=0;i<999;i++)
		Write_A_B(ECG1[i]/new,ECG1[i]/new,Channal_AB,1);
		//i=0;
		//for(i=0;i<999;i++)	
			//Write_A_B(ECG3[i],ECG3[i],Channal_AB,1);   //测量通道
   }
}