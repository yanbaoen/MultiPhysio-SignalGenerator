sbit RES_CS = P3^6; //片选 
sbit SCK = P3^5;
sbit DI = P3^4;


void Write_RES(uchar dat1) //调整数字电位器
{uchar i,command=0x11;
//dat1=~dat1;
RES_CS = 0; //片选 
_nop_(); 
SCK = 0;
_nop_(); 
for(i=8;i>0;i--) //写命令
{command <<= 1; 
DI = CY;
_nop_(); 
SCK = 1;
_nop_(); 
SCK = 0;
_nop_(); 
}
for(i=8;i>0;i--) //写数据
{ dat1 <<= 1;
DI = CY;
_nop_(); 
SCK = 1 ;
_nop_(); 
SCK = 0 ;
_nop_(); 
}
RES_CS = 1;
_nop_(); 
}