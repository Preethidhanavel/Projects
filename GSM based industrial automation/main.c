#include<lpc21xx.h>
#include<string.h>
#include"uart0.h"
//#include"delay.h"
#include"lcd4bit.h"
void UART0_rex(char *);
void motor_rotate(void);
void motor_off(void);
//void uart0_rex(char * );
 void lcd_project(char* );
int main()
{
             char *c,*p,mes[200];
 			 IODIR0=(1<<17)|(1<<18)|(1<<19)|(1<<14)|(1<<15)|(1<<21);
			 IOSET0=(1<<17)|(1<<18)|(1<<19)|(1<<14)|(1<<15);
             UART0_config();
             in_it();
			 lcd_command(0X80);
 		     lcd_project("GSM BASED INDUSTRIAL AUTOMATION                  ");
     
	 	    uart0_str("AT\r\n");
       		delay_ms(1000);
        	uart0_str("AT+CMGF=1\r\n");
         	delay_ms(1000);

start:
      uart0_str("AT+CMGS=\"+919361120142\"\r\n");
      delay_ms(1000);
      uart0_str("waiting for input...");
      UART0_tx(26);
      delay_ms(1000);

      lcd_command(0X01);
      lcd_command(0Xc0);
      lcd_str("wait for input..");
      delay_seconds(15);
      UART0_rex(mes);
      delay_ms(1000);
       //uart0_rex(mes);
     p=strstr(mes,"MOTOR ON");
// uart0_str(mes);
//lcd_command(0X80);
//lcd_str(mes);
    delay_ms(1000);
    if(p!=NULL)
    {
	  UART0_tx('a');	
      IOCLR0=1<<17;
      motor_rotate();
	  lcd_command(0X80);	
	  lcd_str("MOTOR ON");
	   
    }
    p=NULL;
    p=strstr(mes,"MOTOROFF");
    if(p!=NULL)
	{
      IOSET0=1<<17;
      UART0_tx('b');
	  motor_off();
      lcd_command(0X80);	
	  lcd_str("MOTOR OFF"); 
	}
	p=NULL;
		
	p=strstr(mes,"ALARM ON");
	if(p!=NULL)
	{
		IOCLR0=1<<18;
		UART0_tx('c');
		IOSET0=1<<21;
		lcd_command(0X80);
		lcd_str("ALARM ON");
	}
	p=NULL;
		
	p=strstr(mes,"ALARMOFF");
	if(p!=NULL)
	{
		IOSET0=1<<18;
		UART0_tx('d');
		IOCLR0=1<<21;
		lcd_command(0X80);
		lcd_str("ALARM OFF");
	}
	p=NULL;
							
	p=strstr(mes,"PUMP ON");
	if(p!=NULL)
    {
		IOCLR0=1<<19;
		UART0_tx('e');
		lcd_command(0X80);
		lcd_str("PUMP ON ");
	}
	p=NULL;

	p=strstr(mes,"PUMPOFF");
	if(p!=NULL)
	{
		IOSET0=1<<19;
		UART0_tx('f');
		lcd_command(0X80);
		lcd_str("PUMP OFF");
	}
    goto start;
}
void motor_rotate(void)
{

IOCLR0=1<<14;
IOSET0=1<<15;
delay_ms(500);
IOSET0=1<<14;
IOCLR0=1<<15;
delay_ms(500);

}
void motor_off(void)
{
IOCLR0=1<<14;
IOCLR0=1<<15;
}
void UART0_rex(char * a)
{   int i=0;
   lcd_command(0X01);
   lcd_command(0Xc0);
   lcd_str("sending message....");
  
   while(i<60){
   a[i]=UART0_rx();
//if (a[i] == '\r' || a[i] == '\n') {
         //  break;
     // }
   i++;
}
a[i]='\0';
uart0_str(a);
}

void lcd_project(char* a)
{
int i=0,n=strlen(a),j,k=0;
while(k<32)
{
for(j=0;j<16;j++)
{
	lcd_command(0X80+j);
	lcd_data(a[(i+j)%n]);
}
i=(i+1)%n;
delay_ms(250); 

k++;
}
}
