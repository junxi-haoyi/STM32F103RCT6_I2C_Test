#include "includes.h"

int main()
{
    uint32_t i=1;
    DHTDataTypeDef Data={0};
    I2C_GPIO_Config();
    SysTick_Init(1000);
    OLED_Init();
    USART_Config();
    OLED_Fill(0xff);
    Delay_s(1);
    printf("test\r\n");

    if(WaitStandby()==1)
    {
        printf("OLED disable");
    }
    else{
        printf("OLED enable");
    }
    while(1)
    {
        // i+=10;
        // setBrightness(0x00+i);
        // Delay_s(1);


        if(I2C_Time>100)
        {
            // printf("%d\r\n",i);
            //i+=50;
            setBrightness(0x00);
            Delay_s(1);
            I2C_Time=0;
        }

        if(DHT11_Time>3000)
        {
            setBrightness(0xff);
            Delay_s(1);
            //DHTReadData(&Data);
            printf("Hum:%d.%d%%RH\r\n",Data.Hum,Data.HumS);
            printf("Tem:%d.%d¡æ\r\n",Data.Tem,Data.TemS);
            DHT11_Time=0;
        }
    }
}
