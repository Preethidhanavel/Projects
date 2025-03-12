#include <lpc21xx.h>
#include <stdint.h>	   //uart1 gsm
#include <stdio.h>		//uart0 gps
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
//#include<strings.h>

char Latitude_Buffer[15], Longitude_Buffer[15], Time_Buffer[15], Altitude_Buffer[8];
char GGA_String[150];
uint8_t GGA_Comma_Pointers[20];
char GGA[3];
volatile uint16_t GGA_Index, CommaCounter;
bool IsItGGAString = false;

// Switch pin P0.14
#define SWITCH_PIN (1 << 14) // P0.14

__irq void UART0_Interrupt(void);

void delay_ms(int second) {
    T0PR = 15000-1;          // Set prescaler for 1ms delay
    T0TCR = 0X01;            // Enable Timer
    while (T0TC < second);   // Wait until the desired delay has passed
    T0TCR = 0X03;            // Stop Timer
    T0TCR = 0X00;            // Reset Timer
}

void UART0_init(void) {
    PINSEL0 |= 0x00000005;  // Enable UART0 Rx0 and Tx0 pins
    U0LCR = 0x83;           // DLAB = 1, 1 stop bit, 8-bit character length
    U0DLM = 0x00;           // Set for baud rate of 9600 with Pclk = 15MHz
    U0DLL = 0x61;           // Set divisor for 9600 baud rate
    U0LCR = 0x03;           // DLAB = 0, 8-bit data, no parity
    U0IER = 0x00000001;     // Enable RDA interrupts
}

void UART1_init(void) {
    PINSEL0 |= 0x00050000;  // Enable UART1 Rx1 and Tx1 pins
    U1LCR = 0x83;           // DLAB = 1, 1 stop bit, 8-bit character length
    U1DLM = 0x00;           // Set for baud rate of 9600 with Pclk = 15MHz
    U1DLL = 0x08;           // Set divisor for 9600 baud rate
    U1LCR = 0x03;           // DLAB = 0, 8-bit data, no parity
    U1IER = 0x00000001;     // Enable RDA interrupts
}

void UART0_TxChar(char ch) {  
    U0THR = ch;
    while ((U0LSR & 0x40) == 0);  // Wait till transmission is completed
}

void UART1_TxChar(char ch) {  
    U1THR = ch;
    while ((U1LSR & 0x40) == 0);  // Wait till transmission is completed
}

void UART0_SendString(char* str) {  
    uint8_t i = 0;
    while (str[i] != '\0') {
        UART0_TxChar(str[i]);
        i++;
    }
}

void UART1_SendString(char* str) {  
    uint8_t i = 0;
    while (str[i] != '\0') {
        UART1_TxChar(str[i]);
        i++;
    }
}

__irq void UART0_Interrupt(void) {
    int iir_value;
    char received_char;
    iir_value = U0IIR;
    received_char = U0RBR;
    
    if (received_char == '$') {
        GGA_Index = 0;
        CommaCounter = 0;
        IsItGGAString = false;
    }
    else if (IsItGGAString == true) {
        if (received_char == ',') {
            GGA_Comma_Pointers[CommaCounter++] = GGA_Index;  // Store commas in buffer
        }
        GGA_String[GGA_Index++] = received_char;  // Store the $GPGGA string in buffer
    }
    else if ((GGA[0] == 'G') && (GGA[1] == 'G') && (GGA[2] == 'A')) {
        IsItGGAString = true;
        GGA[0] = GGA[1] = GGA[2] = 0;
    }
    else {
        GGA[0] = GGA[1];
        GGA[1] = GGA[2];
        GGA[2] = received_char;
    }
    VICVectAddr = 0x00;
}

void get_Latitude(uint16_t Latitude_Pointer) {
    uint8_t lat_index = 0;
    float lat_decimal_value, lat_degrees_value;
    int32_t lat_degrees;
    uint8_t index = (Latitude_Pointer + 1);
    
    for (; GGA_String[index] != ','; index++) {
        Latitude_Buffer[lat_index] = GGA_String[index];
        lat_index++;
    }

    lat_decimal_value = atof(Latitude_Buffer);  
    lat_decimal_value = (lat_decimal_value / 100);  
    lat_degrees = (int)(lat_decimal_value);  
    lat_decimal_value = (lat_decimal_value - lat_degrees) / 0.6;  
    lat_degrees_value = (float)(lat_degrees + lat_decimal_value);  

    sprintf(Latitude_Buffer, "%f", lat_degrees_value);
}

void get_Longitude(uint16_t Longitude_Pointer) {
    float long_decimal_value, long_degrees_value;
    int32_t long_degrees;
    uint8_t long_index = 0;
    uint8_t index = (Longitude_Pointer + 1);
    
    for (; GGA_String[index] != ','; index++) {
        Longitude_Buffer[long_index] = GGA_String[index];
        long_index++;
    }

    long_decimal_value = atof(Longitude_Buffer);
    long_decimal_value = (long_decimal_value / 100);  
    long_degrees = (int)(long_decimal_value);  
    long_decimal_value = (long_decimal_value - long_degrees) / 0.6;  
    long_degrees_value = (float)(long_degrees + long_decimal_value);  

    sprintf(Longitude_Buffer, "%f", long_degrees_value);
}

void send_SMS(char* phone_number, char* message) {
    UART1_init();
    
    // Send AT commands to GSM module
    UART1_SendString("AT\r\n");
    delay_ms(500);
    UART1_SendString("AT+CMGF=1\r\n");  // Set SMS mode to text
    delay_ms(500);
    UART1_SendString("AT+CMGS=\"");
    UART1_SendString(phone_number);
    UART1_SendString("\"\r\n");
    delay_ms(500);
	 UART1_SendString("SOMEONE IS TRYING TO ACCESS YOUR VEHICLE\r\n");
	 delay_ms(500); 
    UART1_SendString(message);
	 // Send the location message
    UART1_TxChar(0x1A); // Send Ctrl+Z to send SMS
    delay_ms(500);
}

int main() {
    char message[100];
	char alert[100]="BIKE STOLEN";
	 char url[100];
	 memset(url,0,100);
//	char Latitude_Buffer[20];
//		char Longitude_Buffer[20];
    GGA_Index = 0;
    memset(GGA_String, 0, 150);
    memset(Latitude_Buffer, 0, 15);
    memset(Longitude_Buffer, 0, 15);

    memset(Time_Buffer, 0, 15);
    memset(Altitude_Buffer, 0, 8);

    // Configure the switch pin P0.14 as input
    IODIR0 &= ~SWITCH_PIN;  

    VICVectAddr0 = (unsigned) UART0_Interrupt;
    VICVectCntl0 = 0x00000026;
    VICIntEnable = 0x00000040;
    VICIntSelect = 0x00000000;
    UART0_init();

    while (1) {
        if (!(IOPIN0 & SWITCH_PIN)) {
            // Get latitude and longitude
            get_Latitude(GGA_Comma_Pointers[0]);
            get_Longitude(GGA_Comma_Pointers[2]);
								 
            // Create the message containing the latitude and longitude
           // snprintf(message, sizeof(message), "Latitude: %s, Longitude: %s", Latitude_Buffer, Longitude_Buffer);
		   memset(message,0,sizeof(message));
			// sprintf(message,"latitude: %s, longitude: %s",Latitude_Buffer,Longitude_Buffer);
			  snprintf(url, sizeof(url), "https://www.google.com/maps?q=%s,%s", Latitude_Buffer, Longitude_Buffer);
            // Send the message via SMS (replace with actual phone number)
           send_SMS("+919344817327", url); 
		//	send_SMS("+919344817327",alert);
        }

        delay_ms(1000);
    }
}
