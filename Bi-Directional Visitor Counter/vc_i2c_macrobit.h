#define scl_pin 0X00000010
#define sda_pin 0X00000040

#define cclk 60000000
#define pclk cclk/4
#define i2c_speed 100000
#define loadval ((pclk/i2c_speed)/2)

#define i2c_en 6
#define sta_bit 5
#define si_bit 3
#define sto_bit 4
#define aa_bit 2
