
#include"delay.h"

#define LCD_D 0X0f<<20
#define RS 1<<17
#define RW 1<<18
#define E 1<<19
void in_it(void);
void lcd_command(unsigned char );
void lcd_data(unsigned char );
void lcd_str(char * );

	

void in_it()
{
	 //  PINSEL2=0;
	IODIR1|=LCD_D|RS|RW|E;
	lcd_command(0X01);
	lcd_command(0X02);
	lcd_command(0X0C);
	lcd_command(0X28);
	lcd_command(0X80);
}
void lcd_command(unsigned char cmd)
{
		IOCLR1=LCD_D;
		IOSET1=(cmd&0xf0)<<16;
		IOCLR1=RS;
		IOCLR1=RW;
		IOSET1=E;
		delay_ms(2);
		IOCLR1=E;

		IOCLR1=LCD_D;
		IOSET1=(cmd&0x0f)<<20;
		IOCLR1=RS;
		IOCLR1=RW;
		  IOSET1=E;
		  delay_ms(2);
		  IOCLR1=E;
}
 
		  void lcd_str(char * s)
		  {while(*s)
		  lcd_data(*s++);}

void lcd_data(unsigned char d )
{IOCLR1=LCD_D;
IOSET1=(d&0xf0)<<16;
IOSET1=RS;
IOCLR1=RW;
IOSET1=E;
delay_ms(2);
IOCLR1=E;

IOCLR1=LCD_D;
IOSET1=(d&0x0f)<<20;
IOSET1=RS;
IOCLR1=RW;
		  IOSET1=E;
		  delay_ms(2);
		  IOCLR1=E;
}
