#ifndef __I2C
#define __I2C

#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"

#define I2C_Add 0X78
#define Write 0
#define Read 1

#define I2C_SCL_1() GPIO_SetBits(GPIOA,GPIO_Pin_2);
#define I2C_SCL_0() GPIO_ResetBits(GPIOA,GPIO_Pin_2);
#define I2C_SDA_1() GPIO_SetBits(GPIOA,GPIO_Pin_3);
#define I2C_SDA_0() GPIO_ResetBits(GPIOA,GPIO_Pin_3);

#define I2C_SDA_READ() GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)


void I2C_GPIO_Config(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendByte(uint8_t _ucByte);
uint8_t I2C_ReadByte(void);
uint8_t I2C_WaitAck(void);
void I2C_MAck(void);
void I2C_MNAck(void);
uint8_t Call_Device(uint8_t _Address);
uint8_t WaitStandby(void);
void OLED_Init(void);
void I2C_WriteByte(uint8_t addr,uint8_t data);
void WriteCmd(uint8_t I2C_Command);
void WriteDat(uint8_t I2C_Data);
void OLED_Init(void);
void OLED_SetPos(uint8_t x,uint8_t y);
void OLED_Fill(uint8_t fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void setBrightness(uint8_t brightness);


#endif