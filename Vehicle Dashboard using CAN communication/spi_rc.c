#include<lpc21xx.h>
#include<stdio.h>
#include "lcd.h"
#include "spi_p.h"

typedef unsigned int u32;
u32 *ptr=(u32*)0XE0038000;


typedef struct can
{
u32 id;
u32 dlc;
u32 rtr;
u32 byteA;
u32 byteB;
}can_node;

void can_init(void);
void can_tx(can_node);
void can_rx(can_node*);
void data_hex(int);
f32 f;
u32 f1;
int main()
{ //unsigned int flag=0;
  can_node m1,m2;
  IODIR0|=(1<<18);
  IOSET0|=(1<<18);
  
  lcd_init();
  can_init();
  init_spi();
 
  //lcd_cmd(0X80);
  //lcd_str("Rx1...");
	 
  while(1) 
 {		
      	 can_rx(&m1);
       if(m1.id==0X03)
  	   {	   
	       IOCLR0=1<<18; 
	       lcd_cmd(0X80);
	       lcd_str("Rx....");
             f=read_adc_mcp3204(0);
         	
	         lcd_cmd(0Xc0);
             lcd_float(f);
          	 delay_ms(1000);
			 	 
			 if(f==0.0)
			    m1.byteA=0X00;
			 else if((f>=0.5)&&(f<=1))
			    m1.byteA=0X19;
			else if((f>=1)&&(f<=2))
			    m1.byteA=0X32;
			else if((f>2)&&(f<3))
			    m1.byteA=0X4B;
			else if(f>=3)
			   m1.byteA=0X64; 
			 //f1=f;
			// m1.byteA=f1;
			  can_tx(m1);
	          delay_s(1);
	    }
  }
 }

void can_init()
{
   PINSEL1|=0X00014000;
   VPBDIV=1;
   C2MOD=0X01;
   C2BTR=0X001c001d;	

  // ptr[0]=0X2001;
   ptr[0]=0X2003;
  // ptr[2]=0X2003;

   SFF_sa=0X00;
   SFF_GRP_sa=0Xc;
   EFF_sa=0Xc;
   EFF_GRP_sa=0XC;
   ENDofTable=0Xc;

   AFMR=0X00;
   C2MOD=0x0; 
}

void can_tx(can_node m)
{
   C2TID1=m.id;
   C2TFI1=(m.dlc<<16);	
   if(m.rtr==0) 
   {
		C2TFI1&=~(1<<30);
		C2TDA1=m.byteA;
		C2TDB1=m.byteB; 
   }
   else
   {
		C2TFI1|=(1<<30);
   }
	C2CMR=(1<<0)|(1<<5);
	while((C2GSR&(1<<3))==0);
}

void can_rx(can_node *m1)
{
	while((C2GSR&0x1)==0);
	m1->id=C2RID;
	m1->dlc=(C2RFS>>16)&0xF;
	m1->rtr=(C2RFS>>30)&0x1;
	if(m1->rtr==0)
	{
		m1->byteA=C2RDA;
		m1->byteB=C2RDB;
	}
	C2CMR=(1<<2);
}

void data_hex(int num)
{
  char a[15];
  sprintf(a,"%x",num);
  lcd_str(a);
}
