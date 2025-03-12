#include<LPC21XX.H>
#include<stdint.h>
#include"ROBOUARTLCD.C"

unsigned char ch1;
unsigned char s;


#define LCD_D 0xf<<20//p0.0 to  p0.7
#define RS 1<<17//p0.20
#define RW 1<<18//p0.21
#define E 1<<19//p0.22
typedef unsigned char U8;

void lcd_init(void);
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);
void str(unsigned char*);


unsigned char i;
	unsigned char msg1[]={"moving forward    \n"};
	unsigned char msg2[]={"moving backward    \n"};
	unsigned char msg3[]={"moving right     \n"};
	unsigned char msg4[]={"moving left     \n"};
	unsigned char msg5[]={"stop     \n"};
	
	
	void delay_millisecond(uint16_t j)
{
	uint16_t x,i;
	for(i=0;i<j;i++)
	{
		for(x=0;x<6000;x++);// loop generate 1 milli sec
	}
}


int main()
{
	
	
		lcd_init();
	
	lcd_cmd(0x80);
	str("HI!I'M VECTOR");
	lcd_cmd(0xc5);
	str("ROBO");
	delay_millisecond(5000);
	lcd_cmd(0x01);
	lcd_cmd(0x80);
	str("SPEED :60MHz");
	lcd_cmd(0xc5);
	str("MEMORY :4GB");
	UART0_CONFIG();
				
		while(1) // REPEAT FUNCTION
		{
			
			UART0_CONFIG();
			s= UART0_RX();
			
			if(s=='f')
			{
				
	lcd_init();
	delay_millisecond(5000);
				IOCLR0=m1f;
		   IOSET0=m1b;
				IOCLR0=m2f;
			  IOSET0=m2b;
				
				str("MOVING FORWARD");
				
				for(i=0;msg1[i]!='\0';i++)
				{
					 UART0_TX(msg1[i]);
				
				}
				
			}
			else if(s=='b')
			{
				
	lcd_init();
	delay_millisecond(5000);
				IOSET0=m1f;
				IOCLR0=m1b;
				IOSET0=m2f;
			  IOCLR0=m2b;
			
			str("MOVING BACKWARD");
				
				
		for(i=0;msg2[i]!='\0';i++)
				{
					 UART0_TX(msg2[i]);
					

				}
				
			}
			else if(s=='r')
			{
				
			lcd_init();
	      	   	delay_millisecond(5000);
				IOCLR0=m1f;
				IOSET0=m1b;
				IOSET0=m2f;
				IOCLR0=m2b;
			
				str("MOVING RIGHT");
								
				
					for(i=0;msg3[i]!='\0';i++)
				{
					 UART0_TX(msg3[i]);
					
					
				}
				
			}
else if(s=='l')
			{
				

	lcd_init();
		delay_millisecond(5000);
				IOSET0=m1f;
				IOCLR0=m1b;
				IOCLR0=m2f;
				IOSET0=m2b;

				
				str("MOVING LEFT");
			
				
				for(i=0;msg4[i]!='\0';i++)
				{
					 UART0_TX(msg4[i]);
					
					
				}
				
			}

else if(s=='s')
			{
				lcd_init();
				 	delay_millisecond(5000);
				IOSET0=m1f;
			  IOSET0=m1b;
				IOSET0=m2f;
				IOSET0=m2b;
				
				
			     lcd_cmd(0X84);
				  str("STOP");
				
				for(i=0;msg5[i]!='\0';i++)
				{
					 UART0_TX(msg5[i]);
					
					 
				
				}
				
			}
 UART0_TX('\n');
			
		}
	}
	
void lcd_init(void)
{
	IODIR1=LCD_D|RS|E|RW;
	lcd_cmd(0x01);
	lcd_cmd(0x02);
	 lcd_cmd(0x0c);
	lcd_cmd(0xc0);
	lcd_cmd(0x28);
  lcd_cmd(0x80);
}
void lcd_cmd(unsigned char cmd)
{
	IOCLR1=LCD_D;
	IOSET1=(cmd&0xf0)<<16;
	IOCLR1=RS;
	IOCLR1=RW;
	IOSET1=E;
	delay_millisecond(2);
	IOCLR1=E;
	
	IOCLR1=LCD_D;
	IOSET1=(cmd&0x0f)<<20;
	IOCLR1=RS;
	IOCLR1=RW;
	IOSET1=E;
	delay_millisecond(2);
	IOCLR1=E;
}
void lcd_data(unsigned char D)
{
	IOCLR1=LCD_D;
	IOSET1=(D&0xf0)<<16;
	IOSET1=RS;
	IOCLR1=RW;
	IOSET1=E;
	delay_millisecond(2);
	IOCLR1=E;
	
	IOCLR1=LCD_D;
	IOSET1=(D&0x0f)<<20;
	IOSET1=RS;
	IOCLR1=RW;
	IOSET1=E;
	delay_millisecond(2);
	IOCLR1=E;
}

void str(unsigned char *dat)
{
	while(*dat!='\0')
	{
		lcd_data(*dat);
		dat++;
	}
}
