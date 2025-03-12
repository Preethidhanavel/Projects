#include<lpc21xx.h>
#include<stdio.h>
#include "lcd.h"
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

int main()
{ unsigned int flag=0;
  can_node m1;
  IODIR0|=(1<<17);
  IOSET0|=(1<<17);

  lcd_init();
  can_init();
  //lcd_cmd(0X80);
  //lcd_str("Rx1...");
	 
  while(1) 
 {
      	 can_rx(&m1);
     if((m1.id==0X02)&&(flag==0))
	 {	    
	  	 IOCLR0=1<<17;
		 flag=1;
	  // delay_s(3);
	   }
	   //else if(m1.id==0X05)
	   else	if((m1.id==0X02)&&(flag==1))
	   {  IOSET0=1<<17;
	 	  flag=0;
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
   ptr[0]=0X2002;
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
