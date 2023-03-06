#include "I2C.h"

void I2C_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    I2C_Stop();
}

static void I2C_Delay(void)
{
    uint8_t i;
    for(i=0;i<10;i++);
}

void I2C_Start(void)
{
    I2C_SDA_1();
    I2C_SCL_1();
    I2C_Delay();
    I2C_SDA_0();
    I2C_Delay();
    I2C_SCL_0();
    I2C_Delay();
}

void I2C_Stop(void)
{
    I2C_SDA_0();
    I2C_SCL_1();
    I2C_Delay();
    I2C_SDA_1();
}


void I2C_SendByte(uint8_t _ucByte)
{
    uint8_t i;

    for(i=0;i<8;i++)
    {
        if(_ucByte&0x80)
        {
            I2C_SDA_1();
        }
        else 
        {
            I2C_SDA_0();
        }
        I2C_Delay();
        I2C_SCL_1();
        I2C_Delay();
        I2C_SCL_0();  
        if(i==7)
        {
            I2C_SDA_1();
        } 
        _ucByte<<=1;
        I2C_Delay();
    }                           //attention
}


uint8_t I2C_ReadByte(void)
{
    uint8_t i;
    uint8_t value=0;

    for(i=0;i<8;i++)
    {
        value<<=1;
        I2C_SCL_1();
        I2C_Delay();
        if(I2C_SDA_READ())value++;
        I2C_SCL_0();
        I2C_Delay();
        
    }
    return value;
}

uint8_t I2C_WaitAck(void)
{
     uint8_t re;

    I2C_SDA_1();
    I2C_Delay();
    I2C_SCL_1();
    I2C_Delay();

    if(I2C_SDA_READ())
    {
        re=1;
    }
    else
    {
        re=0;
    }
    I2C_SCL_0();
    I2C_Delay();

    return re;
}

void I2C_MAck(void)
{
    I2C_SDA_0();
    I2C_Delay();
    I2C_SCL_1();
    I2C_Delay();
    I2C_SCL_0();
    I2C_Delay();
    I2C_SDA_1();
}

void I2C_MNAck(void)
{
    I2C_SDA_1();
    I2C_Delay();
    I2C_SCL_1();
    I2C_Delay();
    I2C_SCL_0();
    I2C_Delay();
}


uint8_t Call_Device(uint8_t _Address)
{
    uint8_t ucAck;

    I2C_Start();

    I2C_SendByte(_Address|Write);
    ucAck=I2C_WaitAck();

    I2C_Stop();

    return ucAck;
}


uint8_t WaitStandby(void)
{
    uint32_t i=0;

    while(Call_Device(I2C_Add))
    {
        if(i++>0xffff)return 1;

    }
    return 0;

}


void I2C_WriteByte(uint8_t addr,uint8_t data)//addr is register address
{
    I2C_Start();

    I2C_SendByte(I2C_Add|Write);

    if(I2C_WaitAck()!=0)
    {
        goto cmd_fail;
    }

    I2C_SendByte(addr);

    if(I2C_WaitAck()!=0)
    {
        goto cmd_fail;
    }

    I2C_SendByte(data);

    if(I2C_WaitAck()!=0)
    {
        goto cmd_fail;
    }
    
    I2C_Stop();

    cmd_fail:I2C_Stop();

}


void WriteCmd(uint8_t I2C_Command)
{
    I2C_WriteByte(0x00,I2C_Command);
}


void WriteDat(uint8_t I2C_Data)
{
    I2C_WriteByte(0x40,I2C_Data);
}

void OLED_Init(void)
{
	Delay_s(1);		// 1s,这里的延时很重要,上电后延时，没有错误的冗余设计
	
    WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x1F); //

    WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x02);

	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //亮度调节 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel

    OLED_CLS();
}


void OLED_SetPos(uint8_t x,uint8_t y)
{
   WriteCmd(0xb0+y);
   WriteCmd((x&0xf0)>>4|0x10);
   WriteCmd((x&0x0f)|0x01); 
}


void OLED_Fill(uint8_t fill_Data)
{
    uint8_t m,n;
    for(m=0;m<4;m++)
    {
        WriteCmd(0xb0+m);
        WriteCmd(0x00);
        WriteCmd(0x10);
        for(n=0;n<128;n++)
        {
            WriteDat(fill_Data);
        }
    }
}

void setBrightness(uint8_t brightness)
{
    WriteCmd(0x81);
    WriteCmd(brightness);
}

void OLED_CLS(void)
{
    OLED_Fill(0x00);
}

void OLED_ON(void)
{
    WriteCmd(0x8d);
    WriteCmd(0x14);
    WriteCmd(0xaf);
}

void OLED_OFF(void)
{
    WriteCmd(0x8d);
    WriteCmd(0x10);
    WriteCmd(0xae);
}

void OLED_ShowStr(uint8_t x,uint8_t y,unsigned char *ch,unsigned char TextSize)
{
    uint8_t c=0,i=0,j=0;

    switch(TextSize)
    {
        case 1:
        {
            while(ch[j]!='\0')
            {
                c= ch[j]-32;
                if(x>126)
                {
                    x=0;
                    y++;
                }
            }
        }
    }
}







