#include<stdlib.h>
void UART0_tx( unsigned char);
char UART0_rx(void);
void UART0_config(void);
int uart0_otp(void);
void uart0_str_otp(char *);
void UART0_config(void)
{		PINSEL0|=0X05;
		U0LCR=0X83;
		U0DLL=97;
		U0DLM=0;
		U0LCR=0X03;
}
void UART0_tx(unsigned char d)
{
		while(((U0LSR>>5)&1)==0);
		U0THR=d;
}
char UART0_rx(void)
{
		while((U0LSR&1)==0);	
		return U0RBR;
}
void uart0_str( char *s)
{	  
			
			
		while(*s)
		UART0_tx(*s++);
		
}
int uart0_otp(void)
{
short int ran;
while(1)
{
ran=rand();
if((ran<1000)||(ran>10000)||(ran<0))
continue;
return ran;
}
}
