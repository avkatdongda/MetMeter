/*
*********************************************************************************************************
*
*	模块名称 : 按键驱动模块
*	文件名称 : bsp_key.c
*	版    本 : V1.0
*	说    明 : 头文件
*
*********************************************************************************************************
*/

#include "bsp.h"

//按键硬件连接
#define K1_GPIO              	KEY1_GPIO_Port
#define K1_PIN                 	KEY1_Pin
#define K2_GPIO              	KEY2_GPIO_Port
#define K2_PIN                 	KEY2_Pin
#define K3_GPIO              	KEY3_GPIO_Port
#define K3_PIN                 	KEY3_Pin
#define K4_GPIO              	KEY4_GPIO_Port
#define K4_PIN                 	KEY4_Pin


static KEY_T s_tBtn[KEY_COUNT];
static KEY_FIFO_T s_tKey;

static void bsp_InitKeyVar(void);
static void bsp_InitKeyHard(void);
static void bsp_DetectKey(uint8_t i);

//按下检测
static uint8_t IsKeyDown1(void) {if (HAL_GPIO_ReadPin(K1_GPIO, K1_PIN) == 0) return 1;else return 0;} //
static uint8_t IsKeyDown2(void) {if (HAL_GPIO_ReadPin(K2_GPIO, K2_PIN) == 0) return 1;else return 0;}
static uint8_t IsKeyDown3(void) {if (HAL_GPIO_ReadPin(K3_GPIO, K3_PIN) == 0) return 1;else return 0;}
static uint8_t IsKeyDown4(void) {if (HAL_GPIO_ReadPin(K4_GPIO, K4_PIN) == 0) return 1;else return 0;}

//按键初始化
void bsp_InitKey(void)
{
	bsp_InitKeyVar(); //初始化变量
	bsp_InitKeyHard();	//初始化硬件
}

//模拟发送一个按键值
void bsp_PutKey(uint8_t _KeyCode)
{
	s_tKey.Buf[s_tKey.Write] = _KeyCode;

	if (++s_tKey.Write  >= KEY_FIFO_SIZE)
	{
		s_tKey.Write = 0;
	}
}

//得到按键值
uint8_t bsp_GetKey(void)
{
	uint8_t ret;

	if (s_tKey.Read == s_tKey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_tKey.Buf[s_tKey.Read];

		if (++s_tKey.Read >= KEY_FIFO_SIZE)
		{
			s_tKey.Read = 0;
		}
		return ret;
	}
}

//得到按键值
uint8_t bsp_GetKey2(void)
{
	uint8_t ret;

	if (s_tKey.Read2 == s_tKey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_tKey.Buf[s_tKey.Read2];

		if (++s_tKey.Read2 >= KEY_FIFO_SIZE)
		{
			s_tKey.Read2 = 0;
		}
		return ret;
	}
}

//得到按键状态
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID)
{
	return s_tBtn[_ucKeyID].State;
}

//
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed)
{
	s_tBtn[_ucKeyID].LongTime = _LongTime;
	s_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed;
	s_tBtn[_ucKeyID].RepeatCount = 0;
}


//清楚按键值
void bsp_ClearKey(void)
{
	s_tKey.Read = s_tKey.Write;
}

//按键硬件初始化
static void bsp_InitKeyHard(void)
{
	;
}

//初始化按键变量
static void bsp_InitKeyVar(void)
{
	uint8_t i;

	s_tKey.Read = 0;
	s_tKey.Write = 0;
	s_tKey.Read2 = 0;

	for (i = 0; i < KEY_COUNT; i++)
	{
		s_tBtn[i].LongTime = KEY_LONG_TIME;	
		s_tBtn[i].Count = KEY_FILTER_TIME / 2;	
		s_tBtn[i].State = 0;					
		s_tBtn[i].RepeatSpeed = 0;				
		s_tBtn[i].RepeatCount = 0;				
	}

   
	s_tBtn[0].IsKeyDownFunc = IsKeyDown1;
	s_tBtn[1].IsKeyDownFunc = IsKeyDown2;
	s_tBtn[2].IsKeyDownFunc = IsKeyDown3;
	s_tBtn[3].IsKeyDownFunc = IsKeyDown4;
	
}

//按键检测
static void bsp_DetectKey(uint8_t i)
{
	KEY_T *pBtn;

	/*
		????????????????????????????????????????????????
		if (s_tBtn[i].IsKeyDownFunc == 0)
		{
			printf("Fault : DetectButton(), s_tBtn[i].IsKeyDownFunc undefine");
		}
	*/

	pBtn = &s_tBtn[i];
	if (pBtn->IsKeyDownFunc())
	{
		if (pBtn->Count < KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count < 2 * KEY_FILTER_TIME)
		{
			pBtn->Count++;
		}
		else
		{
			if (pBtn->State == 0)
			{
				pBtn->State = 1;

				/* ????????????????????????????? */
				bsp_PutKey((uint8_t)(3 * i + 1));
			}

			if (pBtn->LongTime > 0)
			{
				if (pBtn->LongCount < pBtn->LongTime)
				{
					/* ??????????????????????????????????? */
					if (++pBtn->LongCount == pBtn->LongTime)
					{
						/* ????????????????????FIFO */
						bsp_PutKey((uint8_t)(3 * i + 3));
					}
				}
				else
				{
					if (pBtn->RepeatSpeed > 0)
					{
						if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
						{
							pBtn->RepeatCount = 0;
							/* ???????????????????????10ms???????1?????????? */
							bsp_PutKey((uint8_t)(3 * i + 1));
						}
					}
				}
			}
		}
	}
	else
	{
		if(pBtn->Count > KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count != 0)
		{
			pBtn->Count--;
		}
		else
		{
			if (pBtn->State == 1)
			{
				pBtn->State = 0;

				/* ?????????????????????????????? */
				bsp_PutKey((uint8_t)(3 * i + 2));
			}
		}

		pBtn->LongCount = 0;
		pBtn->RepeatCount = 0;
	}
}

//按键扫描，要放到一个周期循环里，比如timer
void bsp_KeyScan(void)
{
	uint8_t i;

	for (i = 0; i < KEY_COUNT; i++)
	{
		bsp_DetectKey(i);
	}
}
