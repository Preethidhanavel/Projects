#include"vc_i2c_header.h"
#include"vc_i2c_typedef.h"
#include"vc_i2c_macrobit.h"

void i2c_init(void)
{
PINSEL0=scl_pin|sda_pin;
I2SCLL=loadval;
I2SCLH=loadval;
I2CONSET=1<<i2c_en;
}

void i2c_start(void)
{
I2CONSET=1<<sta_bit;
while(((I2CONSET>>si_bit)&1)==0);
I2CONCLR= 1<<sta_bit;
}
void i2c_restart(void)
{
   I2CONSET=1<<sta_bit;
   I2CONCLR=1<<si_bit;
   while(((I2CONSET>>si_bit)&1)==0);
   I2CONCLR=1<<sta_bit;
}
void i2c_write(u8 s)
{
I2DAT=s;
I2CONCLR=1<<si_bit;
while(((I2CONSET>>si_bit)&1)==0);
}

void i2c_stop(void)
{
I2CONSET=1<<sto_bit;
I2CONCLR=1<<si_bit;
}
u8 i2c_nack(void)
{		 
 I2CONSET = 0x00; 
  I2CONCLR = 1<<si_bit;
	while(((I2CONSET>>si_bit)&1)==0);
	return I2DAT;
}
u8 i2c_mack(void)
{
				I2CONSET=1<<aa_bit;
				 I2CONCLR=1<<si_bit;
				 while(((I2CONSET>>si_bit)&1)==0);
				 I2CONCLR=1<<aa_bit;
				 return I2DAT;

}
