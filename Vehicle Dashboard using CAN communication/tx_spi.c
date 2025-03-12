#include<lpc21xx.h>
#include<stdio.h>
#include "lcd.h"
#include "spi_p.h"

typedef unsigned int u32;
typedef struct can
{
u32 id;
u32 dlc;
u32 rtr;
u32 byteA;
u32 byteB;
}can_node;
float f;
void can_init(void);
void can_tx(can_node);
void can_rx(can_node *m1);
void data_hex(int num);
void cgram_write(unsigned char);

unsigned char cgram_lut[]={
                            0X0e,0X1b,0X11,0X11,0X11,0X11,0X1f,0X00,
							0X0e,0X1b,0X11,0X11,0X1f,0X1f,0X1f,0X00,
							0X0e,0X1b,0X11,0X1f,0X1f,0X1f,0X1f,0X00,
							0X0e,0X1b,0X1f,0X1f,0X1f,0X1f,0X1f,0X00,
							0X0e,0X1f,0X1f,0X1f,0X1f,0X1f,0X1f,0X00
						  };

int main()
{
        can_node m,m1;
		unsigned int count1=0,count=0;
        m.id=0X01;
        m.rtr=0;
        m.dlc=0x04;
        m.byteA=0xaabbccdd;
        m.byteB=0X00000000;

        lcd_init();
        can_init();
		cgram_write(40);
		lcd_cmd(0X85);
        lcd_str("VEHICLE");
		lcd_cmd(0Xc0);
		lcd_str("    DASHBOARD");
		delay_ms(3000);
	//	lcd_cmd(0X01);
		lcd_cmd(0X80);
		lcd_str("Transmitting... "); 
		lcd_cmd(0Xc0);
		lcd_str("              "); 
        while(1)
        {	
	   
    	if(((IOPIN0>>16)&1)==0)
	 { 
	 count++; 	
	 lcd_cmd(0X80);
	 lcd_str("WIPER           ");	
	 delay_s(2);

	  m.id=0X01;
	 can_tx(m);
	 delay_s(1);

	 if(count==1)
	 {
	 lcd_cmd(0Xc0);
	 lcd_str("OFF           ");
	// count=2;	 	    
	 }
	  else if(count==2)
	 { 	
	 lcd_cmd(0Xc0);
	 lcd_str("LOW SPEED     ");
	// count=3;	  
	 }
	 else if(count==3)
	 {	
	 lcd_cmd(0Xc0);
	 lcd_str("MEDIUM SPEED         ");
	 
	 }
	  if(count==4)
	 {
	 lcd_cmd(0Xc0);
	 lcd_str("HIGH SPEED          ");
	// count=2;	
	count=0; 	    
	 }
	
	   while(((IOPIN0>>16)&1)==0); 
	 }
	  
	else if(((IOPIN0>>15)&1)==0)
	 {	
	       count1++;
           delay_ms(150);
	       lcd_cmd(0Xc0);
           lcd_str("LEFT INDICATOR  ");
	       delay_s(2);
	      
		   if((count1%2)==0)
	       {
	          	 m.id=0X02;  
	      //    can_tx(m);
	      //    delay_s(1);
	       }
	       else
	       {    m.id=0X02;
	        }
	          can_tx(m);
	          delay_s(1);			   
	       //}
	       while(((IOPIN0>>15)&1)==0);
	 }	
	   
	 else if(((IOPIN0>>14)&1)==0)
	 {	    

	 IODIR0|=(1<<19);
       IOSET0|=(1<<19);
	 lcd_cmd(0Xc0);
	 lcd_str("FUEL ");
	  delay_s(2);

	 m.id=0X03; 
	 can_tx(m);
	 delay_s(1);
	
	        can_rx(&m1);	   
	       IOCLR0=1<<19;
		   lcd_cmd(0Xc5);
		   data_hex(m1.byteA);
		   if(m1.byteA==0X00)
		   { 
             lcd_cmd(0Xc5);
			 lcd_str(" 0%    ");
			 lcd_cmd(0Xce);
			 lcd_data(0);
          	 delay_ms(1000);
			}
		
			if(m1.byteA==0X19)
		   { 
             lcd_cmd(0Xc5);
			 lcd_str("25%      ");
			 lcd_cmd(0Xce);
			 lcd_data(1);
          	 delay_ms(1000);
			}
			if(m1.byteA==0X32)
		   { 
             lcd_cmd(0Xc5);
			 lcd_str("50%      ");
			 lcd_cmd(0Xce);
			 lcd_data(2);
          	 delay_ms(1000);
			}
			if(m1.byteA==0X4b)
		   { 
             lcd_cmd(0Xc5);
			 lcd_str("75%      ");
			 lcd_cmd(0Xce);
			 lcd_data(3);
          	 delay_ms(1000);
			}
			if(m1.byteA==0X64)
		   { 
             lcd_cmd(0Xc5);
			 lcd_str("100%     ");
			 lcd_cmd(0Xce);
			 lcd_data(4);
          	 delay_ms(1000);
			}
	    while(((IOPIN0>>14)&1)==0);
	 }	
	 }
  }


void can_init()
{
   PINSEL1|=0X00014000;
   VPBDIV=1;
   C2MOD=0X01;
   AFMR=0X02;
   C2BTR=0X001c001d;	
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

void cgram_write(unsigned char nBytes)
{
  unsigned char i;
  lcd_cmd(0X40);
  for(i=0;i<nBytes;i++)
     lcd_data(cgram_lut[i]);
}
