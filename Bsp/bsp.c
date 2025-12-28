#include "bsp.h"

extern ADC_HandleTypeDef hadc1;

/*
********************************************************************************
* @function : void bsp_Init(void)
* @brief    : BSP初始化
* @param    : none
* @return   : none
********************************************************************************
*/
void bsp_Init(void)
{
	bsp_led_Int();
	bsp_usart_int();
	bsp_InitKey();
	PUMP_Int();
}

/*
********************************************************************************
* @function : void delay_ms(u16 nms)
* @brief    : ms延时
* @param    : nms毫秒
* @return   : none
********************************************************************************
*/	
void delay_ms(uint16_t nms)
{
	uint16_t i;
	
	for(i=0; i<nms; i++) 
		delay_us(1000);
}

/*
********************************************************************************
* @function : void delay_us(INT32U nus)
* @brief    : 微妙延时
* @param    : nus:微妙
* @return   : none
********************************************************************************
*/			 
void delay_us(uint32_t nus)
{
	uint32_t i = 0,j = 0;
	
	for(i = 0 ; i < nus; i++)
	{
		for(j = 0; j < 84; j++)
		{
			;
		}
	}
}

void delayMs(uint16_t nms)
{	 	
	uint16_t t;	  	  
	uint32_t temp;	

	for(t=0; t<nms; t++)
    {
		for(temp=0; temp<10000; temp++);
    }  	    
} 


#define VBAT_LOW  	3400 //最低电量
#define VBAT_FULL 	4200 //充满电量
#define VBAT_CAL	0//100 校准值

//电量采集，未经过滤波处理
uint32_t GetBatV(void)
{
	uint32_t AD_Value = 0;
	uint32_t tem32, vBat;

	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 100);
	AD_Value = HAL_ADC_GetValue(&hadc1); //

	tem32 = AD_Value*3300/4096;
	vBat = tem32*2+VBAT_CAL;
	return vBat;
}
//电量采集，有数据滤波，并转换成百分比
void BSP_GetBatPow(void)
{
    uint8_t tem8;

	uint32_t vBat;
	
	vBat = GetBatV();
	FILTER_Push(vBat);
	vBat = FILTER_Get();
	if(vBat == FILTER_DATA_ERROR) return;

	tem8 = (vBat-VBAT_LOW)*100/(VBAT_FULL-VBAT_LOW);
	if(tem8>100) tem8 = 100;

	Dev.Vbat = vBat;
	Dev.pow = tem8;
}

//声光报警启动
//lev:报警等级1~2
void ALARM_Start(uint8_t lev)
{
	if(lev == 1) BEEP_Start(TIME_BEEP_BEEP_LEV1, TIME_BEEP_STOP_LEV1, 0);
	else
	if(lev == 2) BEEP_Start(TIME_BEEP_BEEP_LEV2, TIME_BEEP_STOP_LEV2, 0);
	
}

//蜂鸣器响一声
void BEEP_Beep(uint8_t n)
{
	BEEP_Start(10, 10, n);
}

//声光报警停止
void ALARM_Stop(void)
{
	BEEP_Stop();
}
