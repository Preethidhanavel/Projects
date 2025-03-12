//To receive and re-transmit through UART0 of LPC2129
#include<LPC21xx.h>
void UART0_CONFIG(void);
void UART0_TX(unsigned char);
unsigned char UART0_RX(void);
void UART0_STR(unsigned char *);
#define m1f 1<<14// motor 1
#define m1b 1<<15
#define m2f 1<<16//motor2
#define m2b 1<<17
/*int main()
{
	unsigned char ret;
	UART0_CONFIG();
	//UART0_TX('A');
	//UART0_STR("EMBEDDED");
	while(1)
	{
		ret=UART0_RX();
		UART0_TX(ret);
	}
}*/
void UART0_CONFIG(void)
{
	PINSEL0 =PINSEL0 |0X00000005;
	IODIR0=m1f|m1b|m2f| m2b;
	U0LCR=0X83;
	U0DLL=0x61;
	U0DLM=0x00;
	U0LCR=0X03;
}
void UART0_TX(unsigned char d)
{
	while(!(U0LSR&(1<<5)));
	U0THR=d;
}
void UART0_STR(unsigned char *s)
{
	while(*s)
	UART0_TX(*s++);
}
unsigned char UART0_RX(void)
{
	while(((U0LSR&(1<<0)))==0);
	return U0RBR;
}
