//=====================================================================
//                    一种基于智能手机的多功能生理信号发生器程序
//               SIVI_Multifunction-Physiological-Signal-Generator
//                                MPSG_V1.0
//硬件连接:  TLV5618:DIN ——P1^0;  
//                  SCLK ——P1^1;    
//                  CS  ——P1^2;      
//                  VDD --逻辑电源(+5V)
//                  AGND --GND(0V)
//          HC-05:  VDD --逻辑电源(+5V)
//                  AGND --GND(0V)

//注意: 所需输入的数字信号值为16位数,其中D15~~D12为特殊位设置,D11~~D0为数据位000~~fff

/***********************************************************************************/
#include<reg52.h>							//包含51单片机寄存器定义的头文件
#include<intrins.h>	
#define uint unsigned int
#define uchar unsigned char
static void DA_convert(uint Dignum);
extern void Write_A_B(uint Data,bit Model);
void InitUART  (void);
void SendByte(unsigned char dat);
void SendStr(unsigned char *s);
sbit DIN   = P1^0;     //数据输入端
sbit SCLK = P1^1;     //时钟信号
sbit CS    = P1^2;     //片选输入端，低电平有效
void InitUART  (void)
{
    SCON  =0x50;       // SCON: 模式 1, 8-bit UART, 使能接收 
    TMOD |= 0x20;      // TMOD:timer 1, mode 1, 8-bit 重装
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
   unsigned int dat;
   unsigned char ah,al;       
   while(!RI);                           //判断是接收中断产生
      RI=0;
   al=SBUF;
   while(!RI);
      RI=0;
   ah=SBUF;
   dat=(ah<<8)|al;
    Write_A_B(dat,1);           
    SendByte(dat);  //把接收到的值再发回电脑端
}
void DA_convert(uint Dignum)
{
uint Dig=0;
uchar i=0;
SCLK=1;
CS=0;           //片选有效
for(i=0;i<16;i++)   //写入16Bit的控制位和数据
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
void Write_A_B(uint Data,bit Model)
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

   DA_convert(Temp|Data);       
}
int main(void)
{
	InitUART();
    while(1);
   
}
