#include "lcd_init.h"
#include "bsp.h"

void LCD_GPIO_Init(void)
{
	// GPIO_InitTypeDef GPIO_InitStruct;
  	// __HAL_RCC_GPIOA_CLK_ENABLE();
 	// __HAL_RCC_GPIOB_CLK_ENABLE();
	// __HAL_RCC_GPIOC_CLK_ENABLE();
	
	// GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7;
	// GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	// GPIO_InitStruct.Pull = GPIO_PULLUP;
	// GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	// HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	// GPIO_InitStruct.Pin = GPIO_PIN_6;
	// GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	// GPIO_InitStruct.Pull = GPIO_PULLUP;
	// GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	// HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// GPIO_InitStruct.Pin = GPIO_PIN_13;
	// GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	// GPIO_InitStruct.Pull = GPIO_PULLUP;
	// GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	// HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


	// HAL_GPIO_WritePin(GPIOA, 	GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7,	GPIO_PIN_SET);
	// HAL_GPIO_WritePin(GPIOB, 	GPIO_PIN_6,		GPIO_PIN_SET);
	// HAL_GPIO_WritePin(GPIOC, 	GPIO_PIN_13,	GPIO_PIN_SET);

}
// void LCD_GPIO_Init(void)
// {
// 	GPIO_InitTypeDef  GPIO_InitStructure;
//  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能A端口时钟
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;	 
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
//  	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //初始化GPIOA
//  	GPIO_SetBits(GPIOA,GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7);
	
//  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能B端口时钟
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	 
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
//  	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOB
//  	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能A端口时钟
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	 
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
//  	GPIO_Init(GPIOC, &GPIO_InitStructure);	  //初始化GPIOA
//  	GPIO_SetBits(GPIOC,GPIO_Pin_13);
// }


/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
// void LCD_Writ_Bus(uint8_t dat) 
// {	
// 	uint8_t i;
// 	LCD_CS_Clr();
// 	for(i=0;i<8;i++)
// 	{			  
// 		LCD_SCLK_Clr();
// 		if(dat&0x80)
// 		{
// 		   LCD_MOSI_Set();
// 		}
// 		else
// 		{
// 		   LCD_MOSI_Clr();
// 		}
// 		LCD_SCLK_Set();
// 		dat<<=1;
// 	}	
//   LCD_CS_Set();	
// }
void LCD_Writ_Bus(uint8_t dat) 
{	
//	uint8_t i;
	LCD_CS_Clr();
	HAL_SPI_Transmit(&hspi2,&dat,1,0xfff);
  	LCD_CS_Set();	
}

/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD_WR_REG(uint8_t dat)
{
	LCD_DC_Clr();//写命令
	LCD_Writ_Bus(dat);
	LCD_DC_Set();//写数据
}


/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1+80);
		LCD_WR_DATA(y2+80);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
	else
	{
		LCD_WR_REG(0x2a);//列地址设置
		// LCD_WR_DATA(x1+80);
		// LCD_WR_DATA(x2+80);
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
}

void LCD_Init(void)
{
	LCD_GPIO_Init();//初始化GPIO
	
	// LCD_RES_Clr();//复位
	// delayMs(100);//osDelay(100);//delay_ms(100);
	// LCD_RES_Set();
	// delayMs(100);//osDelay(100);//delay_ms(100);
	
	// LCD_BLK_Set();//打开背光
  	// delayMs(100);//osDelay(100);//delay_ms(100);
	
	// //************* Start Initial Sequence **********//
	// LCD_WR_REG(0x11); //Sleep out 
	// delayMs(120);//osDelay(120);//delay_ms(120);              //Delay 120ms 

	LCD_RES_Clr();//复位
	osDelay(100);//delay_ms(100);
	LCD_RES_Set();
	osDelay(100);//delay_ms(100);
	
	LCD_BLK_Set();//打开背光
  	osDelay(100);//delay_ms(100);
	
	//************* Start Initial Sequence **********//
	LCD_WR_REG(0x11); //Sleep out 
	osDelay(120);//delay_ms(120); 


	//************* Start Initial Sequence **********// 
	LCD_WR_REG(0x36);
	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x00);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x70);
	else LCD_WR_DATA8(0xA0);

	LCD_WR_REG(0x3A);			
	LCD_WR_DATA8(0x05);

	LCD_WR_REG(0xB2);			
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x0C); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x33); 
	LCD_WR_DATA8(0x33); 			

	LCD_WR_REG(0xB7);			
	LCD_WR_DATA8(0x35);

	LCD_WR_REG(0xBB);			
	LCD_WR_DATA8(0x32); //Vcom=1.35V
					
	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x01);

	LCD_WR_REG(0xC3);			
	LCD_WR_DATA8(0x15); //GVDD=4.8V  颜色深度
				
	LCD_WR_REG(0xC4);			
	LCD_WR_DATA8(0x20); //VDV, 0x20:0v

	LCD_WR_REG(0xC6);			
	LCD_WR_DATA8(0x0F); //0x0F:60Hz        	

	LCD_WR_REG(0xD0);			
	LCD_WR_DATA8(0xA4);
	LCD_WR_DATA8(0xA1); 

	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0xD0);   
	LCD_WR_DATA8(0x08);   
	LCD_WR_DATA8(0x0E);   
	LCD_WR_DATA8(0x09);   
	LCD_WR_DATA8(0x09);   
	LCD_WR_DATA8(0x05);   
	LCD_WR_DATA8(0x31);   
	LCD_WR_DATA8(0x33);   
	LCD_WR_DATA8(0x48);   
	LCD_WR_DATA8(0x17);   
	LCD_WR_DATA8(0x14);   
	LCD_WR_DATA8(0x15);   
	LCD_WR_DATA8(0x31);   
	LCD_WR_DATA8(0x34);   

	LCD_WR_REG(0xE1);     
	LCD_WR_DATA8(0xD0);   
	LCD_WR_DATA8(0x08);   
	LCD_WR_DATA8(0x0E);   
	LCD_WR_DATA8(0x09);   
	LCD_WR_DATA8(0x09);   
	LCD_WR_DATA8(0x15);   
	LCD_WR_DATA8(0x31);   
	LCD_WR_DATA8(0x33);   
	LCD_WR_DATA8(0x48);   
	LCD_WR_DATA8(0x17);   
	LCD_WR_DATA8(0x14);   
	LCD_WR_DATA8(0x15);   
	LCD_WR_DATA8(0x31);   
	LCD_WR_DATA8(0x34);
	LCD_WR_REG(0x21); 

	LCD_WR_REG(0x29);
} 
