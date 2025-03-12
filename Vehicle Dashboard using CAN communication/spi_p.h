#define mode_0 0X00
#define mode_1 0X08
#define mode_2 0X10
#define mode_3 0X18

#define mstr_bit 5
#define lsbf_bit 6
#define spif_bit 7

typedef unsigned char u8;

void init_spi(void);
u8 spi(u8);

typedef float f32;
typedef unsigned int u32;

f32 read_adc_mcp3204(u8);

f32 read_adc_mcp3204(u8 ch)
{
  u8 hbyte,lbyte;
  u32 a=0;
  IOCLR0=1<<7;
  spi(0X06);
  hbyte=spi(ch<<6);
  lbyte=spi(0X00);
  IOSET0=1<<7;
  a=((hbyte&0X0f)<<8)|lbyte;
  return ((a*3.3)/4096);
}

void init_spi(void)
{
  PINSEL0|=0X00001500;
  S0SPCCR=150;
  S0SPCR=mode_0|(1<<5);
  IODIR0|=1<<7;
}

u8 spi(u8 data)
{
  u8 stat;
  stat=S0SPSR;
  S0SPDR=data;
  while((S0SPSR>>7)==0);
  return S0SPDR;
}
