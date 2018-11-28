//=====================================================================
//                    TLV5618 DA转换驱动程序
//硬件连接: DIN ——P0^0;  
//          SCLK ——P0^1;    
//          CS   ——P0^2;      
//          VDD --逻辑电源(+5V)
//          AGND --GND(0V)

//说明   参考电压为0—3.5V，本程序为2.5V
//       输出模拟电压为:V=2*Vref*(dignum/0x1000) 注:dignum后四位为:0x0000~~0x0fff
//注意: 所需输入的数字信号值为16位数,其中D15~~D12为特殊位设置,D11~~D0为数据位000~~fff


#include<reg52.h>							//包含51单片机寄存器定义的头文件
#include<intrins.h>	
#define uint unsigned int
#define uchar unsigned char
#define Channal_A     1    //通道A
#define Channal_B     2    //通道B
#define Channal_AB    3    //通道A&B
//**************************一下为函数声明修**************************************
static void DA_conver(uint Dignum);
extern void Write_A_B(uint Data_A,uint Data_B,uchar Channal,bit Model);
void InitUART  (void);
void SendByte(unsigned char dat);
void SendStr(unsigned char *s);
//**************************修改硬件时要修改的部分********************************
sbit DIN   = P1^0;     //数据输入端
sbit SCLK = P1^1;     //时钟信号
sbit CS    = P1^2;     //片选输入端，低电平有效
uchar AABB[2] = {0x00,0x00};
int iii = 0; 
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
    RI=0;//标志位清零
                         
    Temp=SBUF;           //读入缓冲区的值
    AABB[iii] = Temp;
    iii = 0;
    if(AABB[0] == 0xFF)
    {
        iii = 1;
        AABB[0] = 0x00;
    }
    
        
    Write_A_B(AABB[0],AABB[1],Channal_AB,0);           
    SendByte(Temp);  //把接收到的值再发回电脑端
        
    }
}
//=================================================================
// 函数名称 ：void DA_conver(uint Dignum)
// 函数功能 ：进行DA转换
// 入口参数 ：Dignum:根据说明设置转化数据.头四位为特殊位用于选择转化方式,
//            以及用于通道选择.请自行设置.后12位为需要转换的值
// 出口参数 ：无

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

int main(void)
{
	InitUART();
    while(1);
   
}