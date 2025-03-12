#include"vc_eeprom_header.h"
#include"delay.h"
#include"vc_i2c_typedef.h"
#include"vc_i2c_header.h"
void i2c_eeprom_write(u8 slaveaddr,u8 buffaddr,u8 data)
{
i2c_start();
i2c_write(slaveaddr<<1);
i2c_write(buffaddr);
i2c_write(data);
i2c_stop();
delay_ms(10);
}

u8 i2c_eeprom_read(u8 slaveaddr,u8 buffaddr)
{	
u8 data;
i2c_start();
i2c_write(slaveaddr<<1);
i2c_write(buffaddr);
i2c_restart();
i2c_write((slaveaddr<<1)|1);
data=i2c_nack();
i2c_stop();
return data;
}
 void i2c_eeprom_pagewrite(u8 slaveaddr,u8 buffaddr,u8 *p,u8 nbyte)
{
u8 i;
i2c_start();
i2c_write(slaveaddr<<1);
i2c_write(buffaddr);
for(i=0;i<nbyte;i++){
i2c_write(p[i]);
 		 }
i2c_stop();
delay_ms(10);
 }
void i2c_eeprom_seqread(u8 slaveaddr,u8 buffaddr,u8 *p,u8 nbyte)
{
u8 i;
i2c_start();
i2c_write(slaveaddr<<1);
i2c_write(buffaddr);
i2c_restart();
i2c_write((slaveaddr<<1)|1);
for(i=0;i<nbyte-1;i++)
{p[i]=i2c_mack();
}
		    p[i]=i2c_nack();
			i2c_stop();

}

