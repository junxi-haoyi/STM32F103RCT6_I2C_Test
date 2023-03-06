#ifndef __DHT11
#define __DHT11

#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"

typedef enum {
    DHT_IN=1,
    DHT_OUT=0
}DHTModeTypeDef;

typedef struct
{
    uint8_t Hum;
    uint8_t HumS;
    uint8_t Tem;
    uint8_t TemS;
}DHTDataTypeDef;

uint8_t DHTReadData(DHTDataTypeDef *data);

#endif 