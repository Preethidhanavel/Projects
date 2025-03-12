#define LCD_D 0X0f<<20 
#define RS 1<<17
#define RW 1<<18
#define E 1<<19
void lcd_init(void);
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);
void lcd_integer(int);
void lcd_str(unsigned char *);
void delay_s(unsigned int);
void delay_ms(unsigned int );
void lcd_float(float);


typedef unsigned char u8;

void lcd_init(void)
{
IODIR1=LCD_D|RS|RW|E;
lcd_cmd(0X01);
lcd_cmd(0X02);
lcd_cmd(0X0C);
lcd_cmd(0X28);
lcd_cmd(0X80);
}

void lcd_cmd(unsigned char cmd)
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

void lcd_data(unsigned char d )
{
			IOCLR1=LCD_D;
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

void lcd_integer(int a)
{
			u8 b[10];
			int i=0;
			if(a==0)
			{
			lcd_data('0');
			}
			if(a<0)
			{
			a=-a;
			}
			while(a!=0)
			{
			b[i++]=a%16;
			a=a/16;
			}
			for(--i;i>=0;i--)
			{
			lcd_data(b[i]+48);
			}
}
void lcd_str(unsigned char *s)
{
			while(*s)
			{
			lcd_data(*s++);
			}
}

void delay_s(unsigned int s)
{
T0PR=6000000-1;
T0TCR=0X1;
while(T0TC<s);
T0TCR=0X03;
T0TCR=0X00;
}

void delay_ms(unsigned int s)
{
T0PR=15000-1;
T0TCR=0X1;
while(T0TC<s);
T0TCR=0X03;
T0TCR=0X00;
}

 
void lcd_float(float f)
{
   int temp;
   temp=f;
   lcd_integer(temp);
   lcd_data('.');
   temp=(temp-f)*100;
   lcd_integer(temp);
}


