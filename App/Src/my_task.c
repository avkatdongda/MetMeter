/*
=================================================================================================================
 	模块名称 : 主程序模块。
 	文件名称 : my_task.c
 	版    本 : V1.0.0
 	说    明 : 
               
 	修改记录 :
 		版本号    日期         作者     说明
        V1.0.0   2025-06-10   Robin    第1版，即正式版
        V1.0.1   2025-07-05   Robin    
 							1.完善采集流程
 							2.按键功能定义：
								KEY1:模式切换键
									短按：显示界面切换 0.工作界面 1.历史记录界面  2:数据传输界面
									长按：工作界面下：工作模式切换 0.手动模式 1.自动模式
									 
								KEY2:采集键
									短按: 手动模式下：开始采集
										  数据传输界面下：连接服务器
									长按：历史记录界面下： 删除数据
									  数据传输界面下：上传数据
								KEY3:翻页键 -
									短按：历史记录界面下：向前翻页键 
									  数据传输界面下：切换通信模式:蓝牙/wifi/通信关闭,
									  				选择完通信方式后，还需要单击KEY2，连接服务器；然后长按KEY2上传数据
									  所有界面下：取消告警

								KEY4:翻页键 +
									短按：历史记录界面下：向后翻页键 
									  所有界面下：取消告警
				2025-07-15   Robin    	1.完善存储机制
									2.完善UI
									3.增加蓝牙设置时间功能
									4.增加定时轮询功能
									5.增加蓝牙、wifi切换功能
									6.增加条纹显示，只有手动模式下，才显示条纹
									7.完善蜂鸣器，报警灯功能
									8.气泵由高低电平控制改为PWM控制,可调输出电压
		V1.1.0   2025-08-22   Robin 
									1.修改版本号 V1.1.0
									2.修改CCD采集流程，累加采集N次数据，然后平均
									3.添加CCD初始化设置积分时间的功能x
									4.添加上电LED、蜂鸣器测试
				2025-11-12   Robin    	1.把LINE_MAP_START设置从gui.c里移到my_tastk.h里，并改成从1500个点取值，方便用户
				2025-11-28   Robin    	1.增加按键音
									2.修复wifi/ble图标关闭状态不显示bug		
		V1.2.0 	2025-12-08   Robin 
									1.修改版本号 V1.2.0	
				2025-12-20   Robin
									1.寻找最小数算法，改成寻找最小数所在下标
				2025-12-28   Robin
									1.整理源文件,归档
=================================================================================================================
*/

#define MY_TASK_GLOBAL

#include "my_task.h"

#include "bsp.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

void ResetCcdSumData(void);

RTC_TimeTypeDef nTime;
float Sample_GetMethaneConcentration(void); //得到甲烷浓度
uint16_t Sample_GetOneData(void); //得到一次数据

#define STA_MAX 10
#define DLY_NORMAL 10

uint64_t CcdSumData[TCD1103_DATA_SIZE16]; 
#define COUNT_CCD 10 //每轮采集次数
uint8_t counter_cdd = 0;
uint16_t key_counter = 0;

typedef enum
{
    CHG_IN=0, 	
    CHG_DONE, 	
    STA_N
}STA_ID;

int16_t StaCounter[STA_N];

static uint8_t IsChgIn(void)  	{if (HAL_GPIO_ReadPin(CHG_ON_GPIO_Port, CHG_ON_Pin)) 		return 1;else return 0;} //充电线插入检测
static uint8_t IsChgDone(void)	{if (HAL_GPIO_ReadPin(CHG_DONE_GPIO_Port, CHG_DONE_Pin)) 	return 1;else return 0;} //充电完成检测
//static uint8_t IsChgOn(void)	{if (HAL_GPIO_ReadPin(CHG_ON_GPIO_Port, CHG_ON_Pin)) 		return 1;else return 0;} //

//输入IO扫描
void InputScan(void)
{
    if(IsChgIn()) 
	{
		if(StaCounter[CHG_IN] < 0) StaCounter[CHG_IN] = 0;
		else StaCounter[CHG_IN]++;
	}else
	{
		if(StaCounter[CHG_IN] > 0) StaCounter[CHG_IN] = 0;
		else StaCounter[CHG_IN]--;
	}

    if(IsChgDone()) 
	{
		if(StaCounter[CHG_DONE] < 0) StaCounter[CHG_DONE] = 0;
		else StaCounter[CHG_DONE]++;
	}else
	{
		if(StaCounter[CHG_DONE] > 0) StaCounter[CHG_DONE] = 0;
		else StaCounter[CHG_DONE]--;
	}

    if(StaCounter[CHG_IN] > STA_MAX)
	{
		Dev.Sta.Bit.ChrgIn = 0;
		StaCounter[CHG_IN] = 0;
	}else
	if(StaCounter[CHG_IN] < -STA_MAX)
	{
		Dev.Sta.Bit.ChrgIn = 1;
		StaCounter[CHG_IN] = 0;
	}

    if(StaCounter[CHG_DONE] > 3)
	{
		Dev.Sta.Bit.ChrgDone = 1;
		StaCounter[CHG_DONE] = 0;
	}else
	if(StaCounter[CHG_DONE] < -10)
	{
		Dev.Sta.Bit.ChrgDone = 0;
		StaCounter[CHG_DONE] = 0;
	}

}

//用户参数初始化
void UsrParaInit(void)
{
	Dev.pow = 70;
	Dev.id = 0;
	Dev.InfoType = INFO_NORMAL;
	Timer.pump = TIME_PUMP;
	Timer.SaveData = TIME_SAVE_DATA;
	Timer.SendData = TIME_SEND_DATA;
	Dev.Sta.Bit.ErrExFlash = 0;
	Dev.Sta.Bit.NetBle = 0;
	Dev.Sta.Bit.SysChkOk = 0;
	Timer.SysStart = TIME_SYS_START;
	Timer.AutoSample = TIME_SAMPLE_AUTO;
	lg_BwAtMode = 0;
	ResetCcdSumData();

}

//CCD缓存清零
void ResetCcdSumData(void)
{
	counter_cdd = 0;
	memset(CcdSumData, 0, sizeof(CcdSumData));
	
}

//CCD数据求和
uint8_t GetCcdSumData(void)
{
	uint16_t i=0;

	for(i=0; i<TCD1103_DATA_LEN16; i++)
	{
		CcdSumData[i] += TCD1103.Data16[i];
	}
	counter_cdd++;
	if(counter_cdd > COUNT_CCD-1) 
	{
		for(i=0; i<TCD1103_DATA_LEN16; i++)
		{
			CcdSumData[i] = CcdSumData[i]/COUNT_CCD;
			TCD1103.Data16[i] = CcdSumData[i];
		}
		return ERR_OK;
	}
	else return ERR_ERROR;
}

//在i_start~i_end范围内求CCD数据的最小值，返回全局坐标值
//i_start 起始坐标，i_end结束坐标
uint16_t GetCcdMinData(uint16_t i_start, uint16_t i_end)
{
	uint16_t i=i_start, i_min;
	uint64_t tem1, tem2;

	i_min = i;
	tem1 = CcdSumData[i];

	if(i_end>TCD1103_DATA_LEN16) i_end = TCD1103_DATA_LEN16;
	for(; i<i_end; i++)
	{
		if(CcdSumData[i] == 0 ) continue;

		tem2 = tem1;
		tem1 = tem1<CcdSumData[i] ? tem1 : CcdSumData[i];
		i_min = tem2<CcdSumData[i] ? i_min : i;
	}
	return i_min+i_start;
}

//基于最小点左右各两个点进行拟合，返回更精确的最小点坐标
static float GetCcdMinDataFine(uint16_t i_start, uint16_t i_end)
{
	uint16_t i_min;
	float y0, y1, y2, y3, y4;
	float num, den;
	float dx;

	if(i_end > TCD1103_DATA_LEN16) i_end = TCD1103_DATA_LEN16;
	i_min = GetCcdMinData(i_start, i_end);
	if(i_min < 2 || (i_min + 2) >= TCD1103_DATA_LEN16) return (float)i_min;
	if(i_min < i_start + 2 || i_min + 2 > i_end) return (float)i_min;

	y0 = (float)CcdSumData[i_min - 2];
	y1 = (float)CcdSumData[i_min - 1];
	y2 = (float)CcdSumData[i_min];
	y3 = (float)CcdSumData[i_min + 1];
	y4 = (float)CcdSumData[i_min + 2];

	num = 2.0f * (y4 - y0) + (y3 - y1);
	den = 2.0f * (y4 + y0) - (y3 + y1) - 2.0f * y2;
	if(den > -1e-6f && den < 1e-6f) return (float)i_min;

	dx = -0.7f * (num / den);
	if(dx > 2.0f) dx = 2.0f;
	if(dx < -2.0f) dx = -2.0f;

	return (float)i_min + dx;
}

//主任务
void StartTaskMain(void const * argument)
{
	uint8_t err, key_v;
//	uint16_t i;
//	uint8_t lg_keylong = 0;

	typedef struct
	{
		uint8_t Key1   	:1;//
		uint8_t Key2	:1;//
		uint8_t Key3	:1;//
		uint8_t Key4   	:1;//
		uint8_t  		:4;//

	}TYPE_STRUCT_KEY_LONG_STA_BIT;

	typedef union
	{
		TYPE_STRUCT_KEY_LONG_STA_BIT Bit;
		uint8_t All;
	}TYPE_UNION_KEY_LONG_STA;

	TYPE_UNION_KEY_LONG_STA LongPress;

	LongPress.All = 0;
	
	task_n = TASK_IDEL;
	
	DebugPrintStr("StartTaskMain\r\n");//

	for(;;)
	{
		osDelay(10);
		if(lg_BwAtMode) continue;

		key_v = bsp_GetKey();
		
		switch( key_v ) //
		{
			//KEY1 模式切换键-----------------------------
			case KEY1_DOWN : //短按 显示模式切换 0.工作模式 1.历史记录浏览模式 2:数据传输模式
				if(Timer.SysStart) lg_BwAtMode = 1; //开机同时按模式切换键 进入BW模块AT指令模式
				DebugPrintStr("KEY1_DOWN\r\n");
				BEEP_Beep(1);
				break;
			case KEY1_UP : //抬起
				if(LongPress.Bit.Key1) //长按抬起
				{
					LongPress.Bit.Key1 = 0;
					break;
				}
				DebugPrintStr("KEY1_UP\r\n");
				//短按抬起 显示界面切换 0.工作界面 1.历史记录浏览界面 2:数据传输界面
				Dev.GuiMode++;
				if(Dev.GuiMode > 2) Dev.GuiMode = 0;

				page++;
				if(page > 2) page = 0;
				break;
			case KEY1_LONG : //长按 工作模式切换 0.手动模式 1.自动模式
				LongPress.Bit.Key1 = 1;
				if(Dev.GuiMode == GUI_MODE_WORK) //工作模式切换
				{
					if(Dev.WorkMode == WORK_MODE_MANUAL) Dev.WorkMode = WORK_MODE_AUTO;
					else					 	 Dev.WorkMode = WORK_MODE_MANUAL;
				}
				

				break;
			//KEY2 采集键-----------------------------
			case KEY2_DOWN : //短按
				BEEP_Beep(1);
				// ALARM_Start(2); //test
				break;
			case KEY2_UP : //抬起
				// if(key_counter > 100) break;
				// key_counter++;

				if(LongPress.Bit.Key2) //长按抬起
				{
					LongPress.Bit.Key2 = 0;
					break;
				}
				//短按抬起
				if(Dev.GuiMode == GUI_MODE_WORK) //工作界面下开始采集
				{
					DebugPrintStr("TASK_SAMPLE_START\r\n");
					task_n = TASK_SAMPLE_START;
				}

				if(Dev.GuiMode == GUI_MODE_DATATRAN) //数据传输界面下：模块上电，准备连接服务器
				{
					
					if(Dev.ComMode == COM_MODE_CLOSE)
					{
						Dev.Sta.Bit.NetBle = 0;
						Dev.Sta.Bit.NetWifi = 0;
						
						DebugPrintStr("BW_PoweOff\r\n");
						Ble.Sta.Bit.Pow = 0;
						Dev.InfoType = INFO_NORMAL;
						BW_POW_OFF(); //关闭BW模块
					}else
					{
						err = BW_PoweOn();
						if (err == AT_OK)
						{
							DebugPrintStr("BW_PoweOn\r\n");
							Ble.Sta.Bit.Pow = 1;
							Dev.InfoType = INFO_NORMAL;
						}else
						{
							DebugPrintStr("Communication is err!!\r\n");
							Ble.Sta.Bit.Pow = 0;
							Dev.InfoType = INFO_COM_ERROR;
							BEEP_Beep(2);
						}

					}
					
				}

				break;
			case KEY2_LONG : //长按：
				LongPress.Bit.Key2 = 1;
				
				if(Dev.GuiMode == GUI_MODE_RECORD) //历史记录模式 长按删除数据
				{
					DebugPrintStr("Delete data\r\n");

					UsrData_ClearSampleData();
					GUI_UpdataRecord(); //更新记录
					Record.pageTem = 0xFF;
				}else
				if(Dev.GuiMode == GUI_MODE_DATATRAN) //数据传输模式下：上传数据
				{
					if(Dev.Sta.Bit.NetBle || Dev.Sta.Bit.NetWifi)
					{
						DebugPrintStr("Upload data\r\n");
						Dev.InfoType = INFO_UPLOADING;
						lg_SendData = LG_SEND_SAMPLEDATA;
					}else
					{
						DebugPrintStr("Communication is err!!\r\n");
						Dev.InfoType = INFO_COM_ERROR;
						BEEP_Beep(2);
					}
				}
				break;
			//KEY3 向前翻页键-----------------------------
			case KEY3_DOWN : //短按：向前翻页
				BEEP_Beep(1);
				DebugPrintStr("ALARM_Stop\r\n");
				ALARM_Stop();
				if(Dev.GuiMode == GUI_MODE_RECORD) 
				{
					DebugPrintStr("Page up\r\n");
					if(Record.pageKey) Record.pageKey--;
					if(Record.pageKey >= Record.pageTotal) Record.pageKey = Record.pageTotal-1; //0; //
					Record.pageNow = Record.pageKey;
				}
				if(Dev.GuiMode == GUI_MODE_DATATRAN) //通信模式切换 
				{
					if(Dev.ComMode == COM_MODE_CLOSE) 	Dev.ComMode = COM_MODE_BLE;
					else
					if(Dev.ComMode == COM_MODE_BLE) 	Dev.ComMode = COM_MODE_WIFI;
					else
					if(Dev.ComMode == COM_MODE_WIFI) 	Dev.ComMode = COM_MODE_CLOSE;
					else						 	Dev.ComMode = COM_MODE_CLOSE;
					
					sprintf(DEBUG_TEM, "Set ComMode:%d\r\n", Dev.ComMode);
					DebugPrintStr(DEBUG_TEM);

				}	

				break;
			case KEY3_UP : //抬起
				DebugPrintStr("KEY3_UP\r\n");
				break;
			case KEY3_LONG : //长按
				DebugPrintStr("KEY3_LONG\r\n");
				break;
			//KEY4 向后翻页键-----------------------------
			case KEY4_DOWN : //短按：向后翻页
				BEEP_Beep(1);
				DebugPrintStr("ALARM_Stop\r\n");
				ALARM_Stop();
				if(Dev.GuiMode == GUI_MODE_RECORD) 
				{
					DebugPrintStr("Page down\r\n");
					Record.pageKey++;
					if(Record.pageKey >= Record.pageTotal) Record.pageKey = 0;//Record.pageTotal-1; //
					Record.pageNow = Record.pageKey;
				}

				break;
			case KEY4_UP : //抬起
				break;
			case KEY4_LONG : //长按
				break;
			default:
				break;
		}


		switch(task_n)
		{
			case TASK_INIT:
				timer_Preheat = TIME_Preheat;
				Dev.WorkState = STATE_PREHEAT;
				task_n = TASK_PREHEAT;
				break;
			case TASK_PREHEAT: //预热
				if(timer_Preheat == 0) //
				{
					Dev.WorkState = STATE_READY; //显示“请开始测量”
					task_n = TASK_IDEL;
				}
				break;
			case TASK_SAMPLE_START:
				DebugPrintStr("PUMP_ON：delay 2s\r\n");
				Dev.InfoType = INFO_MEASURING;
				PUMP_ON(); //气泵抽气
				Timer.pump = TIME_PUMP;
				Dev.WorkState = STATE_SAMPLE;
				task_n = TASK_AIR_OUT;
				break;
			case TASK_AIR_OUT: //气泵抽气
				if(Timer.pump) break; //延时TIME_PUMP
				PUMP_OFF(); //气泵关闭
				Timer.pump = TIME_PUMP;
				DebugPrintStr("PUMP_OFF\r\nLED_Start\r\n");
				LED_Start();//打开LED
				osDelay(10);

				task_n = TASK_SAMPLE;

				break;
			case TASK_SAMPLE:
				DebugPrintStr("TCD1103_GetData\r\n");
				err = TCD1103_GetData();
				if(err == ERR_OK)
				{
					Dev.Sta.Bit.ErrCcd = 0;
					err = GetCcdSumData();
					
					sprintf(DEBUG_TEM, "CCD sample:%d OK\r\n", counter_cdd);
					DebugPrintStr(DEBUG_TEM);
					
					if(err == ERR_OK)
					{
						DebugPrintStr("CCD sample finished\r\nLED_Stop\r\n");
						LED_Stop();//关闭LED
						task_n = TASK_SAMPLE_STOP;
					}
					

				}else
				{
					DebugPrintStr("Error\r\n");
					Dev.Sta.Bit.ErrCcd = 1;
				}
				break;
			case TASK_SAMPLE_STOP:
				//采集完后，生产一次数据
				DebugPrintStr("Sample_GetOneData\r\n");
				Sample_GetMethaneConcentration(); //得到甲烷浓度
				Sample_GetOneData(); //得到一次数据
				ResetCcdSumData(); //CCD缓存数据清零
				
				//更新显示
				lg_DisLineMap = 1;

				#if TEST_EN == 1
				// DebugPrintStrHex(&TCD1103.Data8[0], TCD1103_DATA_LEN);
				
				for(i=0; i<TCD1103_DATA_SIZE16; i++)
				{
					sprintf(DEBUG_TEM, "%04X ", TCD1103.Data16[i]);
					DebugPrintStr(DEBUG_TEM);
				}
				#endif

				Dev.InfoType = INFO_MEASURED;

				task_n = TASK_IDEL;
				break;
			case TASK_IDEL:
				
				if(sample_n)
				{
					if(Timer.SaveData == 0) //定时保存数据
					{
						DebugPrintStr("UsrData_SaveSampleData\r\n");
						Timer.SaveData = TIME_SAVE_DATA;
						UsrData_SaveSampleData(sample_n);
						sample_n = 0;
					}
				}

				if((Dev.GuiMode == GUI_MODE_WORK) //工作界面，在其它界面会暂停自动采集
				&&(Dev.WorkMode == WORK_MODE_AUTO) //自动模式
				) 
				{
					if(Timer.AutoSample == 0)
					{
						Timer.AutoSample = TIME_SAMPLE_AUTO>TIME_SAMPLE_ONCE?TIME_SAMPLE_AUTO:TIME_SAMPLE_ONCE;

						DebugPrintStr("Auto sampling...\r\n");//

						bsp_PutKey(KEY2_UP); //模拟按键，触发采集
					}
				}
				
				break;

			default:
				break;
		}
	}
}

//蓝牙wifi通信任务
void StartTaskBle(void const * argument)
{
	osDelay(TIME_SYS_START*10); //开机延时

	DebugPrintStr("StartTaskBle\r\n");
	BW_POW_OFF();

	for(;;)
	{
		osDelay(10);
		if(Ble.Sta.Bit.Pow == 0) continue;
		if(lg_BwAtMode) continue;
		
		if(lg_SendData == LG_SEND_SAMPLEDATA)
		{
			if(!Timer.SendData)
			{
				Timer.SendData = TIME_SEND_DATA;
				UsrData_SandSampleData();
				// counter_send++;
				Record.dataSend++;
			}
			
			if(Record.dataSend > Record.dataTotal)
			{
				DebugPrintStr("Upload data finished\r\n");
				Dev.InfoType = INFO_UPLOADED;
				Record.dataSend = 0;
				lg_SendData = LG_SEND_STOP;
			}
		}
		BLE_RxdTask();

	}
}

//UI任务
void StartTaskUi(void const * argument)
{
	
	DebugPrintStr("StartTaskUi\r\n");

	GUI_Init();

	osDelay(100);
	
	GUI_DisLineMapTest(); //显示测试条纹
	osDelay(100);

	ALARM_Start(1); //测试蜂鸣器和报警LED
	LED_Start(); //测试LED
	GUI_DisFrame();
	ALARM_Stop(); //测试蜂鸣器和报警LED 结束
	LED_Stop(); //测试LED 结束
	// lg_BwAtMode = 1;//test
	if(lg_BwAtMode)
	{	
		// LCD_ShowString(X_BOT+80, Y_BOT+10, "AT Mode.", COLOR_DATA, COLOR_BG,16,0);
		GUI_DisSysCheck(CHK_BW, CHK_SETMODE);
		BW_UartAtTask();
	}
	
#if 1//开机检测使能

	GUI_DisSysCheck(CHK_START, CHK_NORMAL);
	osDelay(1000); //开机延时

	for(;;) //开机检测
	{
		osDelay(1000);
		
		if(Dev.Sta.Bit.ErrExFlash)
		{
			GUI_DisSysCheck(CHK_EXFLASH, CHK_ERROR);
		}else
		{
			GUI_DisSysCheck(CHK_EXFLASH, CHK_OK);
		}
		osDelay(1000);

		if(Dev.Sta.Bit.ErrTh)
		{
			GUI_DisSysCheck(CHK_TH, CHK_ERROR);
		}else
		{
			GUI_DisSysCheck(CHK_TH, CHK_OK);
		}
		osDelay(1000);
		
		if(Dev.Sta.Bit.ErrO2)
		{
			GUI_DisSysCheck(CHK_O2, CHK_ERROR);
		}else
		{
			GUI_DisSysCheck(CHK_O2, CHK_OK);
		}
		osDelay(1000);
		if(Dev.Sta.Bit.ErrP)
		{
			GUI_DisSysCheck(CHK_P, CHK_ERROR);
		}else
		{
			GUI_DisSysCheck(CHK_P, CHK_OK);
		}
		osDelay(1000);
		if(Dev.Sta.Bit.ErrCcd)
		{
			GUI_DisSysCheck(CHK_CCD, CHK_ERROR);
		}else
		{
			GUI_DisSysCheck(CHK_CCD, CHK_OK);
		}
		osDelay(1000);
		if(Dev.Sta.Bit.ErrBw)
		{
			GUI_DisSysCheck(CHK_BW, CHK_ERROR);
		}else
		{
			GUI_DisSysCheck(CHK_BW, CHK_OK);
		}
		if((Dev.Sta.All & MASK_SYSCHK) == 0) 
		{
			Dev.Sta.Bit.SysChkOk = 1;//开机检测正常
			break; //进入工作模式
		}
	}

#endif
	
	for(;;)
	{
		osDelay(10);
	
		HAL_RTC_GetTime(&hrtc, &SysTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &SysData, RTC_FORMAT_BIN);
		
		osDelay(1000);
		
		GUI_Display();
		
	}
}

#define ERR_COUNT 2
void Sample_Init(void)
{
	sample_n = 0;
}


//CCD求最小值范围
float Sample_GetMethaneConcentration(void) //得到甲烷浓度
{
	//在此加入甲烷弄浓度算法
	// Dev.Methane = Dev.Temprature+Dev.Humidity+Dev.Pressure+Dev.Oxygen; // 甲烷浓度
	Dev.Methane = GetCcdMinDataFine(CCD_DATA_START, CCD_DATA_END);

	if(Dev.Methane > Dev.Threshold.MetLev2) Dev.AlarmLevel = 2;
	else
	if(Dev.Methane > Dev.Threshold.MetLev1) Dev.AlarmLevel = 1;
	else 						Dev.AlarmLevel = 0;

	if(Dev.AlarmLevel) Dev.Sta.Bit.MetAlarm = 1;
	else			   Dev.Sta.Bit.MetAlarm = 0;

	if(Dev.AlarmLevel) Dev.InfoType = INFO_ALARM;

	return Dev.Methane;
}

//数据处理函数，可以在此添加数据处理算法
uint8_t Sample_DataFilter(void) 
{
	Dev.Temprature = Sht20.Temprature; //温度
	Dev.Humidity = Sht20.Humidity; //湿度
	Dev.Pressure = WF5803F.Pressure; //气压
	Dev.Oxygen = JXMO2.Oxygen; //氧气浓度 JXMO2.O2; //
	return ERR_OK;
}

//得到一次数据
uint16_t Sample_GetOneData(void) 
{
	SAMPLE_DATA[sample_n].Data.n = sample_n;

	SAMPLE_DATA[sample_n].Data.TIME[0] = SysData.Year;
	SAMPLE_DATA[sample_n].Data.TIME[1] = SysData.Month;
	SAMPLE_DATA[sample_n].Data.TIME[2] = SysData.Date;
	SAMPLE_DATA[sample_n].Data.TIME[3] = SysTime.Hours;
	SAMPLE_DATA[sample_n].Data.TIME[4] = SysTime.Minutes;
	SAMPLE_DATA[sample_n].Data.TIME[5] = SysTime.Seconds;

	SAMPLE_DATA[sample_n].Data.Methane = Dev.Methane; // 甲烷浓度
	SAMPLE_DATA[sample_n].Data.Temprature = Dev.Temprature; //温度
	SAMPLE_DATA[sample_n].Data.Humidity = Dev.Humidity; //湿度
	SAMPLE_DATA[sample_n].Data.Pressure = Dev.Pressure; //气压
	SAMPLE_DATA[sample_n].Data.Oxygen = Dev.Oxygen; //氧气浓度
	//保存时间
	SAMPLE_NOW.n = SAMPLE_DATA[sample_n].Data.n;
	SAMPLE_NOW.TIME[0] = SAMPLE_DATA[sample_n].Data.TIME[0];
	SAMPLE_NOW.TIME[1] = SAMPLE_DATA[sample_n].Data.TIME[1];
	SAMPLE_NOW.TIME[2] = SAMPLE_DATA[sample_n].Data.TIME[2];
	SAMPLE_NOW.TIME[3] = SAMPLE_DATA[sample_n].Data.TIME[3];
	SAMPLE_NOW.TIME[4] = SAMPLE_DATA[sample_n].Data.TIME[4];
	SAMPLE_NOW.TIME[5] = SAMPLE_DATA[sample_n].Data.TIME[5];

	SAMPLE_NOW.Methane = SAMPLE_DATA[sample_n].Data.Methane; // 甲烷浓度
	SAMPLE_NOW.Temprature = SAMPLE_DATA[sample_n].Data.Temprature; //温度
	SAMPLE_NOW.Humidity = SAMPLE_DATA[sample_n].Data.Humidity; //湿度
	SAMPLE_NOW.Pressure = SAMPLE_DATA[sample_n].Data.Pressure; //气压
	SAMPLE_NOW.Oxygen = SAMPLE_DATA[sample_n].Data.Oxygen; //氧气浓度
	
	sample_n++;
	if(sample_n >= SAMPLE_NUM) sample_n = 0;
	return sample_n;
}

//
// uint16_t Sample_GetTestData(void)
// {
// 	;
// }

//数据采集任务
void StartTaskSample(void const * argument)
{
	uint8_t err;

	osDelay(TIME_SYS_START*10); //开机延时
	
	// BEEP_ENABLE();
	Sample_Init();
	
	POW5V_ON();

	DebugPrintStr("StartTaskSample\r\n");

	err = JXMO2_Init();
	err = TCD1103_Init();
	BSP_GetBatPow(); //采集电量

	osDelay(5000);

	err = TCD1103_GetData();
	if(err == ERR_OK)
	{
		Dev.Sta.Bit.ErrCcd = 0;
	}else
	{
		Dev.Sta.Bit.ErrCcd = 1;
	}
	
	for(;;)
	{
		osDelay(SAMPLE_FRE);
		if(lg_BwAtMode) continue;

		BSP_GetBatPow(); //采集电量
		osDelay(100);
		err = SHT20_GetValue(); //采集温湿度
		if(err == ERR_OK)
		{
			Dev.Sta.Bit.ErrTh = 0;
		}else
		{
			Dev.Sta.Bit.ErrTh = 1;
		}
		osDelay(100);
		err = WF5803F_Read(); //采集气压
		if(err == ERR_OK)
		{
			Dev.Sta.Bit.ErrP = 0;
		}else
		{
			Dev.Sta.Bit.ErrP = 1;
		}
		osDelay(100);
		err = JXMO2_GetData(); //采集氧气浓度
		if(err == ERR_OK)
		{
			Dev.Sta.Bit.ErrO2 = 0;
		}else
		{
			Dev.Sta.Bit.ErrO2 = 1;
		}

		Sample_DataFilter();

	}
}