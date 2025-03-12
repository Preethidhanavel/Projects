void delay_seconds(int n)
{
T0PR=15000000-1;
T0TCR=0X01;
while(T0TC<n);
T0TCR=0X03;
T0TCR=0X00;
}
void delay_ms(int n)
{
T0PR=15000-1;
T0TCR=0X01;
while(T0TC<n);
T0TCR=0X03;
T0TCR=0X00;
}
void delay_microseconds(int n)
{
T0PR=15-1;
T0TCR=0X01;
while(T0TC<n);
T0TCR=0X03;
T0TCR=0X00;
}
