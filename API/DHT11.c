#include "DHT11.h"


void DHT11_Mode(DHTModeTypeDef mode)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    if(mode==DHT_IN)GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    else GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
}

uint8_t DHTReadData(DHTDataTypeDef *data)
{
    u8 Data[5]={0};
    DHT11_Mode(DHT_OUT);
    GPIO_SetBits(GPIOA,GPIO_Pin_12);
    GPIO_ResetBits(GPIOA,GPIO_Pin_12);
    Delay_ms(20);
    GPIO_SetBits(GPIOA,GPIO_Pin_12);
    DHT11_Mode(DHT_IN);

    while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==1);
    while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0);
    for(u8 i=0;i<40;i++)
    {
        while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==1);
        while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0);
        Delay_us(30);
        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0)
        {
            Data[i/8]&=~(1<<(7-i%8));
        }
        else 
        {
            Data[i/8]|=(1<<(7-i%8));
        }
    }
    if(Data[4]==Data[0]+Data[1]+Data[2]+Data[3])
    {
        data->Hum=Data[0];
        data->HumS=Data[1];
        data->Tem=Data[2];
        data->TemS=Data[3];
        return 1;
    }
    printf("data read error");
    return 0;
}
