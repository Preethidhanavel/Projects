#include<lpc21xx.h>
#include"uart0.h"
//#include"delay.h"
#include"lcd4bit.h"
//#define reset 1<<16
unsigned char rx_buffer[100];
//unsigned char c;
unsigned char on='@',off='%',fon='!',foff='$',aon='&',aoff='_';
unsigned int buffer_size=100,rx_index=0;
unsigned char command[1];
//char ip_address[16];
//int retries=3;
//volatile reset_flag=0;
void esp_init(void);
void uart_int(void);
void uart0_isr(void) __irq
{
int temp;
temp=U0IIR;
if(temp==0X04)
{
rx_buffer[rx_index++]=U0RBR;
}
if(rx_index==buffer_size){
rx_index=0;}
VICVectAddr=0;
}
int main()
{

IODIR0=(1<<17)|(1<<18)|(1<<19);
IOSET0=(1<<17)|(1<<18)|(1<<19);
delay_ms(2000);
in_it();
UART0_config();
uart_int();
lcd_command(0X80);
lcd_str(" IOT BASED HOME");
lcd_command(0Xc0);
lcd_str("***AUTOMATION***");
delay_ms(3000);
lcd_command(0X01);
lcd_str("wait for input...");
delay_ms(2000);
esp_init();
lcd_command(0X01);
while(1){
if(rx_index!=0){
command[0]=rx_buffer[rx_index-1];
if(command[0]==on){
lcd_command(0X84);
lcd_str("LIGHT ON ");
IOCLR0=1<<17;
UART0_tx('a');
}
else if(command[0]==off){
lcd_command(0X84);
lcd_str("LIGHT OFF");
IOSET0=1<<17;
UART0_tx('c');}
   else if(command[0]==fon){
   lcd_command(0Xc0);
lcd_str("FAN ON ");
IOCLR0=1<<18;
UART0_tx('d');}
else if(command[0]==foff){
lcd_command(0Xc0);
lcd_str("FAN OFF");
IOSET0=1<<18;
UART0_tx('e');}
else if(command[0]==aon){
lcd_command(0Xc9);
lcd_str("AC ON ");
IOCLR0=1<<19;
UART0_tx('f');}
else if(command[0]==aoff){
lcd_command(0Xc9);
lcd_str("AC OFF");
IOSET0=1<<19;
UART0_tx('g');}
     }
   }
}
void uart_int(void)
{
VICIntSelect=0;
VICVectCntl0=(1<<5)|6;
VICVectAddr0=(int)uart0_isr;
VICIntEnable=1<<6;
U0IER=1<<0;
}
void esp_init(void)
{
delay_ms(5000);
uart0_str("AT\r\n");
delay_ms(3000);
uart0_str("AT+CWMODE=1\r\n");
delay_ms(3000);
uart0_str("AT+CWJAP=\"Preethi\",\"preethi@123\"\r\n");
delay_ms(3000);
uart0_str("AT+CIFSR\r\n");
delay_ms(3000);
uart0_str("AT+CIPMUX=1\r\n");
delay_ms(3000);
uart0_str("AT+CIPSERVER=1,80\r\n");
delay_ms(3000);
}
