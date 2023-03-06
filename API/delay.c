#include "delay.h"

uint32_t I2C_Time;
uint32_t DHT11_Time;


void SysTick_Init(uint16_t tick)
{
    SysTick_Config(SystemCoreClock/tick);
}


void SysTick_Handler(void)
{
    I2C_Time++;
    DHT11_Time++;
}


void Delay_us(uint32_t us)
{
    while(us--)delay_us();
}

void Delay_ms(uint32_t ms)
{
    while(ms--)Delay_us(1000);

}


void Delay_s(uint8_t s)
{
    while(s--)Delay_ms(1000);
}








