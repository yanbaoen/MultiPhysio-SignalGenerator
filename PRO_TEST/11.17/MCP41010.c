
0sbit RES_CS = P3^6; //Ƭѡ 
sbit SCK = P3^5;
sbit DI = P3^4;


void Write_RES(uchar dat1) //�������ֵ�λ��
{uchar i,command=0x11;
//dat1=~dat1;
RES_CS = 0; //Ƭѡ 
_nop_(); 
SCK = 0;
_nop_(); 
for(i=8;i>0;i--) //д����
{command <<= 1; 
DI = CY;
_nop_(); 
SCK = 1;
_nop_(); 
SCK = 0;
_nop_(); 
}
for(i=8;i>0;i--) //д����
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