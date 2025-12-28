/*
=================================================================================================================
 	模块名称 : LCD用户界面显示
 	文件名称 : gui.c
 	版    本 : V1.0.0
 	说    明 : 
=================================================================================================================
*/
#define GUI_GLOBAL

#include "bsp.h"
#include "pic.h"
#include "lcd_init.h"

#define X_TOP 		5
#define Y_TOP 		10
#define X_TOP_BAT 	X_TOP
#define Y_TOP_BAT 	Y_TOP+5

#define X_TOP_TIME 	X_TOP+60
#define Y_TOP_TIME 	Y_TOP+5

#define X_TOP_BLE 	(320-60)
#define Y_TOP_BLE 	Y_TOP+5

#define X_TOP_DATE 	10
#define Y_TOP_DATE 	Y_TOP_TIME+40

#define LINE_W 320
#define LINE_H 60
#define LINE_X 0
#define LINE_Y 112

extern RTC_TimeTypeDef nTime;
extern uint32_t GetBatV(void);


void Int2Str(uint8_t num, char *tem)
{   
	*(tem) = num/10+0x30;
	*(tem+1) = num%10+0x30;
} 

void GetDataStr(char *str)
{
	Int2Str(SysData.Year, str+0 );
	*(str+2) = '/';
	Int2Str(SysData.Month, str+3 );
	*(str+5) = '/';
	Int2Str(SysData.Date, str+6 );
	*(str+8) = 0;
}
void GetTimeStr(char *str)
{
	Int2Str(SysTime.Hours, str );
	*(str+2) = ':';
	Int2Str(SysTime.Minutes, str+3 );
	*(str+5) = 0;
}

void GetSectr(char *str)
{
	Int2Str(SysTime.Seconds, str );
	*(str+2) = 0;
}

GUI_EXT RTC_DateTypeDef SetData;
GUI_EXT RTC_TimeTypeDef SetTime;

void GetTimeStr2(char *str)
{
	Int2Str(SysTime.Hours, str );
	*(str+2) = ':';
	Int2Str(SysTime.Minutes, str+3 );
	*(str+5) = 0;
	Int2Str(SysTime.Seconds, str+6 );
	*(str+8) = 0;
}

void GUI_Init( void )
{
	lg_DisLineMap = 0;
	Gui.Sta.All = 0xFFFF;
	Gui.DevId = 0xFF;
	Gui.Minutes = 0xFF;
	Gui.pow = 0xFF;
	Gui.InfoType = 0xFF;
	Gui.ComMode = 0xFF;
	pageTem = 0xFF; 
	Record.pageNow = 0;
	Record.pageTem = 0xFF;
	Record.dataSendTem = 0xFF;
	
	GUI_UpdataRecord();

	LCD_Init();
	LCD_Fill(0, 0, LCD_W, LCD_H, COLOR_BG);
	GUI_BlkOn();//
}

void GUI_Clear( void )
{
	LCD_Fill(0, 0, LCD_W, LCD_H, COLOR_BG);
	colorBg=COLOR_BG;
}


void GUI_Black( void )
{
	LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
}

void GUI_BlkOn( void )
{
	LCD_BLK_ON();
}

void GUI_BlkOff( void )
{
	LCD_BLK_OFF();
}

void GUI_BlkFlash( uint8_t n )
{
}

void GUI_DisPow(void)
{
	static uint16_t colorFont;

	if(Dev.Sta.Bit.ChrgIn) //充电线已插入
	{
		if(Gui.Sta.Bit.ChrgDone != Dev.Sta.Bit.ChrgDone)
		{
			Gui.Sta.Bit.ChrgDone = Dev.Sta.Bit.ChrgDone;
			if(Gui.Sta.Bit.ChrgDone)
			{
				LCD_ShowPicMonocolour(X_TOP_BAT,Y_TOP_BAT,38,25,gImage_bat100, COLOR_FONT, COLOR_BG); //38 X 25
			}else
			{
				LCD_ShowPicMonocolour(X_TOP_BAT,Y_TOP_BAT,24,16,gImage_btchrg, COLOR_FONT, COLOR_BG); //LCD_ShowPicMonocolour(X_TOP_BAT,Y_TOP_BAT,38,25,gImage_btchrg, COLOR_FONT, COLOR_BG); //38 X 25
			}
		}
		return; //
	}

	if(Gui.Sta.Bit.ChrgDone != Dev.Sta.Bit.ChrgDone)
	{
		Gui.Sta.Bit.ChrgDone = Dev.Sta.Bit.ChrgDone;
	}
	
	if(Dev.pow<25) //低电量，闪烁
	{
		Gui.pow = Dev.pow;
		if(timer_BatIconToggle == 0)
		{
			timer_BatIconToggle = TIME_BatIconToggle;

			if(colorFont == COLOR_ALARM) colorFont = COLOR_FONT;
			else 						 colorFont = COLOR_ALARM;


			LCD_ShowPicMonocolour(X_TOP_BAT,Y_TOP_BAT,24,16,gImage_bat0, colorFont, COLOR_BG); //24 X 16
		}
		return; //
	}

	if(Gui.pow == Dev.pow) return; //
	
	Gui.pow = Dev.pow;

	if(Gui.pow>75)
	{
		LCD_ShowPicMonocolour(X_TOP_BAT,Y_TOP_BAT,24,16,gImage_bat100, COLOR_FONT, COLOR_BG); //24 X 16
	}else
	if(Gui.pow>50)
	{
		LCD_ShowPicMonocolour(X_TOP_BAT,Y_TOP_BAT,24,16,gImage_bat75, COLOR_FONT, COLOR_BG); //24 X 16
	}else
	if(Gui.pow>25)
	{
		LCD_ShowPicMonocolour(X_TOP_BAT,Y_TOP_BAT,24,16,gImage_bat50, COLOR_FONT, COLOR_BG); //24 X 16
	}else
	if(Gui.pow>10)
	{
		LCD_ShowPicMonocolour(X_TOP_BAT,Y_TOP_BAT,24,16,gImage_bat25, COLOR_FONT, COLOR_BG); //24 X 16
	}
	else //低电量
	{
		LCD_ShowPicMonocolour(X_TOP_BAT,Y_TOP_BAT,24,16,gImage_bat0, COLOR_ALARM, COLOR_BG); //24 X 16
	}
	
}

void GUI_ClrTimeTem(void)
{
	Gui.Date.Year = 0;
	Gui.Date.Month = 0;
	Gui.Date.Date = 0;
	Gui.Minutes = 0;
	Gui.Seconds = 0;
}


void GUI_DisTime(void)
{

	if((Gui.Hours != SysTime.Hours)
	|| (Gui.Minutes != SysTime.Minutes)
	|| (Gui.Seconds != SysTime.Seconds))
	{
		Gui.Hours = SysTime.Hours;
		Gui.Minutes = SysTime.Minutes;
		Gui.Seconds = SysTime.Seconds;
		
		sprintf(GUI_STR, "20%02d/%02d/%02d %02d:%02d:%02d",SysData.Year, SysData.Month, SysData.Date, SysTime.Hours, SysTime.Minutes, SysTime.Seconds);
		LCD_ShowString(X_TOP_TIME, Y_TOP_TIME, GUI_STR, COLOR_DATA, COLOR_BG,16,0);
	}

}

void GUI_DisBleIcon(void)
{
	uint16_t colorFont = COLOR_OFF;
	
	if(Gui.Sta.Bit.NetBle != Dev.Sta.Bit.NetBle)
	{
		Gui.Sta.Bit.NetBle = Dev.Sta.Bit.NetBle;
		if(Gui.Sta.Bit.NetBle)
		{
			colorFont = COLOR_LINK;
		}else
		{
			colorFont = COLOR_OFF;
		}
		
		LCD_ShowPicMonocolour(X_TOP_BLE,Y_TOP_BLE,24,16,gImage_ble, colorFont, COLOR_BG); //24 X 16
	}
}

void GUI_DisWifiIcon(void)
{
	uint16_t colorFont = COLOR_OFF;
	
	if(Gui.Sta.Bit.NetWifi != Dev.Sta.Bit.NetWifi)
	{
		Gui.Sta.Bit.NetWifi = Dev.Sta.Bit.NetWifi;
		if(Gui.Sta.Bit.NetWifi)
		{
			colorFont = COLOR_LINK;
		}else
		{
			colorFont = COLOR_OFF;
		}
		LCD_ShowPicMonocolour(X_TOP_BLE+24,Y_TOP_BLE,21,16,gImage_wifi, colorFont, COLOR_BG); //21 X 16
	}
}

void GUI_DisMainTop(void) //
{
	GUI_DisBleIcon();
	GUI_DisWifiIcon();
	GUI_DisTime();
	GUI_DisPow(); //30 X 20
}

#define X_MID 		37
#define Y_MID 		80
#define W_MID 		166
#define H_MID 		50

void GUI_ClearMid( void )
{
	LCD_Fill(X_MID, Y_MID, X_MID+W_MID, Y_MID+H_MID, COLOR_BG);
}


void GUI_DisMid(void) //
{
	;
}

void GUI_DisStartTest(void) //
{
	LCD_ShowChinese(X_MID,Y_MID,"请开始测量",COLOR_FONT, COLOR_BG,32,0);
}

void GUI_DisTesting(void) //
{
	LCD_ShowChinese(X_MID,Y_MID,"测，量，中",COLOR_FONT, COLOR_BG,32,0);
}


#define X_BOT 		5
#define Y_BOT 		(240-16-20)
#define X_BOT_1 	X_BOT
#define Y_BOT_1 	Y_BOT
#define X_BOT_2 	X_BOT_1+48+8
#define Y_BOT_2 	Y_BOT
#define X_BOT_3 	X_BOT_2+48+8
#define Y_BOT_3 	Y_BOT
#define X_BOT_4 	X_BOT_3+48+8
#define Y_BOT_4 	Y_BOT

void GUI_DisInfo(void) //
{
	if(Gui.InfoType == Dev.InfoType) return;
	Gui.InfoType = Dev.InfoType;

	if(Gui.InfoType == INFO_ALARM)
	{
		LCD_ShowChinese(X_BOT+160,	Y_BOT+10,	"报警：浓度超限", COLOR_ALARM, COLOR_BG, 16, 0);
	}else
	if(Gui.InfoType == INFO_MEASURING)
	{
		LCD_ShowChinese(X_BOT+160,	Y_BOT+10,	"测量中。。。", COLOR_FONT, COLOR_BG, 16, 0);
	}else
	if(Gui.InfoType == INFO_MEASURED)
	{
		LCD_ShowChinese(X_BOT+160,	Y_BOT+10,	"测量完成      ", COLOR_FONT, COLOR_BG, 16, 0);
	}else
	if(Gui.InfoType == INFO_UPLOADING)
	{
		LCD_ShowChinese(X_BOT+160,	Y_BOT+10,	"上传中。。。", COLOR_FONT, COLOR_BG, 16, 0);
	}else
	if(Gui.InfoType == INFO_UPLOADED)
	{
		LCD_ShowChinese(X_BOT+160,	Y_BOT+10,	"上传完成      ", COLOR_FONT, COLOR_BG, 16, 0);
	}else
	if(Gui.InfoType == INFO_COM_ERROR)
	{
		LCD_ShowChinese(X_BOT+160,	Y_BOT+10,	"通信故障      ", COLOR_FONT, COLOR_BG, 16, 0);
	}else
	{
		LCD_ShowChinese(X_BOT+160,	Y_BOT+10,	"             ", COLOR_FONT, COLOR_BG, 16, 0);
	}
}


void GUI_DisInfoNow(uint8_t *info) //
{
	Gui.InfoType = Dev.InfoType;
	LCD_ShowChinese(X_BOT+160,	Y_BOT+10,	info, COLOR_ALARM, COLOR_BG, 16, 0);
	
}

char STR_SYSCHECK[][16]={
	"开机检测。。。",
	"外部存储器：",
	"温湿度传感器：",
	"氧气传感器：",
	"气压传感器：",
	"线阵传感器：",
	"通信模组："
};

char STR_ERROR[][10]={
	"    ",
	"正常",
	"故障",
	"设置模式"
};

void GUI_DisSysCheck(uint8_t type, uint8_t err) //
{
	uint16_t color;
	color = COLOR_FONT;

	sprintf(GUI_STR, "%s%s", &STR_SYSCHECK[type][0], &STR_ERROR[err][0]);
	LCD_ShowChinese(0,	50+type*20, GUI_STR, color, COLOR_BG, 16, 0);
	
}

void GUI_DisMainBot(void) //
{
	
	if(Dev.WorkMode == WORK_MODE_MANUAL)
	{
		LCD_ShowChinese(X_BOT,	Y_BOT+10,	"【手动】", COLOR_FONT, COLOR_BG, 16, 0);
	}else
	{
		LCD_ShowChinese(X_BOT,	Y_BOT+10,	"【自动】", COLOR_FONT, COLOR_BG, 16, 0);
	}

	sprintf(GUI_STR, "[% 4d]", UsrData_GetSampleDataNum());
	LCD_ShowString(X_BOT+80, Y_BOT+10, GUI_STR, COLOR_DATA, COLOR_BG,16,0);

}
void GUI_DisLed(void) //
{
	;
}

#define X_TEST		0
#define Y_TEST 		130

uint8_t gui_disTestData = 0;

void GUI_TestModeOn(void) //
{
	gui_disTestData = 1;
}

void GUI_TestModeOff(void) //
{
	gui_disTestData = 0;
}

void GUI_DisTestData(void) //
{
	uint8_t temSeconds, DisTem[31];//一行显示最大30个字符
	
	if(temSeconds == SysTime.Seconds) return;
	temSeconds = SysTime.Seconds;
	

	sprintf(DisTem, "SHT20  %ld%% %ldC", Sht20.H32, Sht20.T32);
	LCD_ShowString(X_TEST, Y_TEST, DisTem, COLOR_DATA, COLOR_BG,16,0);

	sprintf(DisTem, "WF5803 %ldPa %ldC", WF5803F.P32, WF5803F.T32);
	LCD_ShowString(X_TEST, Y_TEST+16, DisTem, COLOR_DATA, COLOR_BG,16,0);

	sprintf(DisTem, "JXMO2  %d  %d", JXMO2.O2, JXMO2.resolution);
	LCD_ShowString(X_TEST, Y_TEST+32, DisTem, COLOR_DATA, COLOR_BG,16,0);
	
	sprintf(DisTem, "%s %-4d %3d%% %-5d %-2d", HW_VER, Dev.Vbat, Gui.pow, Saved.Para.data_in-Saved.Para.data_out, Dev.sample_count);
	LCD_ShowString(X_TEST, Y_TEST+48, DisTem, COLOR_DATA, COLOR_BG,16,0);

	GUI_DisFrame();
}


#define X_T 0
#define Y_T 50
void GUI_DisTemprature(void)
{
	LCD_ShowChinese(X_T, Y_T, "温度：", COLOR_FONT, COLOR_BG, 16, 0);
	LCD_ShowFloatNumx10(X_T+48, Y_T, Sht20.T32, 4, COLOR_DATA, COLOR_BG, 16);
}

#define X_H (X_T+96)
#define Y_H Y_T
void GUI_DisHumidity(void)
{
	LCD_ShowChinese(X_H, Y_H, "湿度：", COLOR_FONT, COLOR_BG, 16, 0);
	LCD_ShowFloatNumx10(X_H+48, Y_H, Sht20.H32, 4, COLOR_DATA, COLOR_BG, 16);
}

#define X_O2 0
#define Y_O2 66
void GUI_DisO2(void)
{
	LCD_ShowChinese(X_O2, Y_O2, "氧气：", COLOR_FONT, COLOR_BG, 16, 0);
	LCD_ShowFloatNumx10(X_O2+48, Y_O2, JXMO2.O2*10, 4, COLOR_DATA, COLOR_BG, 16);
}

#define X_P (X_O2+96)
#define Y_P Y_O2
void GUI_DisPressure(void)
{
	LCD_ShowChinese(X_P, Y_P, "气压：", COLOR_FONT, COLOR_BG, 16, 0);
	LCD_ShowFloatNumx10(X_P+48, Y_P, WF5803F.P32/10, 5, COLOR_DATA, COLOR_BG, 16);
}

void GUI_DisMainMid(void)
{
	//采集一次更新一次
	// if(SAMPLE_NOW.n != Saved.n) //
	{
		sprintf(GUI_STR, "%6.0f",SAMPLE_NOW.Methane);
		LCD_ShowString(80, 50, GUI_STR, COLOR_DATA, COLOR_BG, 32, 0);
		LCD_ShowString(200, 63, "%VOL", COLOR_DATA, COLOR_BG, 16, 0);

		
		sprintf(GUI_STR, "T:%4.2f~C H:%4.2f%% O2:%4.2f%% P:%4.2fKPa",SAMPLE_NOW.Temprature, SAMPLE_NOW.Humidity, SAMPLE_NOW.Oxygen, SAMPLE_NOW.Pressure);
		LCD_ShowString(0, 90, GUI_STR, COLOR_DATA, COLOR_BG,16,0);
	}

}

void GUI_DisData(void)
{
	;

}



#define X_BANNER	0
#define Y_BANNER	0

void GUI_DisBanner(void)
{
	if(page == PAGE_MAIN)
	{
		;
	}
	else
	if(page == PAGE_RECORD)
	{
		LCD_ShowChinese(X_BANNER+2, Y_BANNER+6,	  "【历史记录】", COLOR_FONT, COLOR_BG, 16, 0);
		LCD_ShowString(X_BANNER, Y_BANNER+32, "N   Date           Met     Tem   Hum   Pre   Oxy  ", COLOR_BG, COLOR_FONT, 12, 0);
		
	}
	else
	if(page == PAGE_DATATRAN)
	{
		LCD_ShowChinese(X_BANNER+2, Y_BANNER+6,	  "【数据通信】", COLOR_FONT, COLOR_BG, 16, 0);
		LCD_ShowChinese(X_BANNER+4, Y_BANNER+64,	  "总数：", COLOR_FONT, COLOR_BG, 16, 0);
		LCD_ShowChinese(X_BANNER+4, Y_BANNER+64,	  "已发送：", COLOR_FONT, COLOR_BG, 16, 0);
	}

}

void GUI_DisFrame(void)
{
	LCD_DrawLine(0, Y_TOP+30, 320, Y_TOP+30, COLOR_FONT); //页眉分割线

	if(page == PAGE_MAIN)
	{
		// GUI_DisMainBot();
	}
	else
	if(page == PAGE_RECORD)
	{
		LCD_ShowChinese(X_BANNER+2, Y_BANNER+6,	  "【历史记录】", COLOR_FONT, COLOR_BG, 16, 0);
		LCD_ShowString(X_BANNER, Y_BANNER+42, "N   Date           Met     Tem   Hum   Pre   Oxy  ", COLOR_BG, COLOR_FONT, 12, 0);
	}
	else
	if(page == PAGE_DATATRAN)
	{
		LCD_ShowChinese(X_BANNER+2, Y_BANNER+6, "【数据通信】", COLOR_FONT, COLOR_BG, 16, 0);
		
		LCD_ShowChinese(X_BANNER, Y_BANNER+64,	  "通信模式：", COLOR_FONT, COLOR_BG, 16, 0);
		LCD_ShowChinese(X_BANNER+4, Y_BANNER+100, "总数：", COLOR_FONT, COLOR_BG, 16, 0);
		LCD_ShowChinese(X_BANNER+4, Y_BANNER+132, "已发送：", COLOR_FONT, COLOR_BG, 16, 0);
	}
	LCD_DrawLine(0, Y_BOT, 320, Y_BOT, COLOR_FONT); //页脚分割线
}


void GUI_DisMain(void) //主界面显示
{
	GUI_DisMainTop();
	GUI_DisMainMid();
	GUI_DisMainBot();
	GUI_DisLineMap(&TCD1103.Data16[LINE_MAP_START-1]); //只有手动模式下，才显示条纹图形

}

#define X_RECORD 	1
#define Y_RECORD	1

void GUI_DisOneData(uint16_t x, uint16_t y, TYPE_STRUCT_SAMPLE_DATA *Data) //历史记录界面
{		  
	
	sprintf(GUI_STR, "%03d 20%02d%02d%02d%02d%02d%02d ", Data->n, Data->TIME[0], Data->TIME[1], Data->TIME[2], Data->TIME[3], Data->TIME[4], Data->TIME[5]);
	LCD_ShowString(x, y, GUI_STR, COLOR_FONT, COLOR_BG, 12, 0);
	sprintf(GUI_STR, "%6.3f %4.2f %4.2f %4.2f %4.2f", Data->Methane, Data->Temprature, Data->Humidity, Data->Pressure, Data->Oxygen);
	LCD_ShowString(x+19*6, y, GUI_STR, COLOR_FONT, COLOR_BG, 12, 0);
}

void GUI_UpdataRecord(void) //更新记录
{
	Record.dataTotal = UsrData_GetSampleDataNum();
	Record.pageTotal = Record.dataTotal/RECORD_DATA_ONEPAGE;
	if(Record.dataTotal%RECORD_DATA_ONEPAGE) Record.pageTotal++;
	if(Record.pageTotal == 0) Record.pageTotal = 1; //没有记录，总页数也为1
}

void GUI_DisOnePage(uint16_t page) //历史记录界面
{
	uint8_t line=0;
	uint16_t nStart, nEnd;
	uint8_t RecordData[LEN_SAMPLE_DATA];

	if(Record.pageTem == page) return;

	Record.pageTem = page;


	nStart = page*RECORD_DATA_ONEPAGE; //得到本页起始数据条数
	nEnd = (page+1)*RECORD_DATA_ONEPAGE; //得到本页结束数据条数

	if(nStart > Record.dataTotal) return; //数据超限
	if(nEnd > Record.dataTotal) nEnd = Record.dataTotal; //

	sprintf(GUI_STR, "[%d/%d]", Record.pageNow+1, Record.pageTotal);
	LCD_ShowString(320-80, Y_BOT+10, GUI_STR, COLOR_FONT, COLOR_BG, 16, 0);

	//先清屏,刷新速度会变慢
	for(line=0; nStart < nEnd; nStart++, line++)
	{
		UsrData_BrowseSampleData(RecordData, nStart);
		GUI_DisOneData(X_RECORD, Y_RECORD+58+line*12, RecordData); //(TYPE_STRUCT_SAMPLE_DATA*)
	}

	//如果每页显示前没有清屏，不够一页余下条数要清空显示，否则会残留上次记录
	for(; line < RECORD_DATA_ONEPAGE; line++)
	{
		LCD_ShowString(X_RECORD, Y_RECORD+58+line*12, "                                                     ", COLOR_FONT, COLOR_BG, 12, 0);
	}

}

void GUI_DisRecord(void) //历史记录界面
{
	GUI_DisOnePage(Record.pageNow);
}

#define X_DATATRAN 	1
#define Y_DATATRAN	1

void GUI_DisDataTran(void) //传输数据界面
{
	GUI_DisBleIcon();
	GUI_DisWifiIcon();
	
	if(Record.dataSendTem != Record.dataSend )
	{
		Record.dataSendTem = Record.dataSend;
		
		sprintf(GUI_STR, "%d", Record.dataTotal);
		LCD_ShowString(X_BANNER+52, Y_BANNER+100, GUI_STR, COLOR_FONT, COLOR_BG, 16, 0);
		sprintf(GUI_STR, "%d", Record.dataSend);
		LCD_ShowString(X_BANNER+68, Y_BANNER+132, GUI_STR, COLOR_FONT, COLOR_BG, 16, 0);
	}

	if(Gui.ComMode != Dev.ComMode)
	{
		Gui.ComMode = Dev.ComMode;
		if(Gui.ComMode == COM_MODE_BLE) LCD_ShowString(X_BANNER+80, Y_BANNER+64, "BLE  ", COLOR_FONT, COLOR_BG, 16, 0);
		else
		if(Gui.ComMode == COM_MODE_WIFI) LCD_ShowString(X_BANNER+80, Y_BANNER+64, "WIFI ", COLOR_FONT, COLOR_BG, 16, 0);
		else
		if(Gui.ComMode == COM_MODE_CLOSE) LCD_ShowString(X_BANNER+80, Y_BANNER+64, "CLOSE", COLOR_FONT, COLOR_BG, 16, 0);
		else LCD_ShowString(X_BANNER+80, Y_BANNER+64, "CLOSE", COLOR_FONT, COLOR_BG, 16, 0);
	}
}

void GUI_Display(void) //总显示
{
	if(pageTem != page)
	{
		pageTem = page;
		Record.dataSendTem = 0xFF; //刷新通信界面
		Record.pageTem = 0xFF; //刷新记录界面
		Gui.Sta.Bit.NetBle = (!Dev.Sta.Bit.NetBle); //刷新BW图标
		Gui.Sta.Bit.NetWifi = (!Dev.Sta.Bit.NetWifi); //刷新Wifi图标
		
		Gui.Sta.Bit.ChrgDone = (!Dev.Sta.Bit.ChrgDone); //刷新电量图标
		Gui.pow = 0xFF; //刷新电量图标
		Gui.ComMode = 0xFF; //刷新通信状态
		
		GUI_Clear();

		GUI_DisFrame();
		GUI_UpdataRecord();
	}

	if(page == PAGE_MAIN)
	{
		GUI_DisMain();
	}else
	if(page == PAGE_RECORD)
	{
		GUI_DisRecord();
	}
	else
	if(page == PAGE_DATATRAN)
	{
		GUI_DisDataTran();
	}
	
	GUI_DisInfo(); //刷新信息栏
}

uint16_t LineColor[320];
//12位整型数据转成RGB灰度值
uint16_t V12ToGreyRgb( uint16_t v12) 
{
	uint8_t grey, r,g, b;
	
	grey = (uint8_t)((v12&0x0FFF) >> 4);
	r = grey>>3;
	g = grey>>2;
	b = grey>>3;
	return (r<<11)|(g<<5)|(b<<0);
}

void GUI_DisLineMap(uint16_t *v12) //显示条纹图形
{
	uint16_t i;

	if(Dev.WorkMode != WORK_MODE_MANUAL) return;//只有手动模式下，才显示条纹
	if(!lg_DisLineMap)  return;
	lg_DisLineMap = 0;

	for(i=0; i<320; i++) //得到灰度值
	{
		LineColor[i] = V12ToGreyRgb(*(v12+i));
	}
	
#if 1
	DebugPrintStr("\r\n");
	DebugPrintStr("CCD Hex:\r\n");
	DebugPrintU16StrHex(v12, 320);
	DebugPrintStr("\r\nGray Hex:\r\n");
	DebugPrintU16StrHex(LineColor, 320);
	DebugPrintStr("\r\n");
#endif

	for(i=0; i<LINE_W; i++)
	{
		if(i>LCD_W) return;//超出屏幕范围，不显示
		LCD_DrawLine(LINE_X+i, LINE_Y, LINE_X+i, LINE_Y+LINE_H, LineColor[i]);
		
	}
}

void GUI_DisLineMapTest(void) //条纹图形测试显示，灰度从0~255
{
	uint16_t i;
	uint8_t grey, r,g, b;

	//显示纯黑---------------------------------------------------------------------------
	for(i=0; i<320; i++) //test
	{
		grey = 0;
		r = grey>>3;
		g = grey>>2;
		b = grey>>3;

		LineColor[i] = (r<<11)|(g<<5)|(b<<0);
	}

	for(i=0; i<LINE_W; i++)
	{
		if(i>LCD_W) return;//超出屏幕范围，不显示
		LCD_DrawLine(LINE_X+i, LINE_Y, LINE_X+i, LINE_Y+LINE_H, LineColor[i]);
		
	}

	//显示纯黑---------------------------------------------------------------------------
	for(i=0; i<320; i++) //test
	{
		grey = 255;
		r = grey>>3;
		g = grey>>2;
		b = grey>>3;

		LineColor[i] = (r<<11)|(g<<5)|(b<<0);
	}

	for(i=0; i<LINE_W; i++)
	{
		if(i>LCD_W) return;//超出屏幕范围，不显示
		LCD_DrawLine(LINE_X+i, LINE_Y, LINE_X+i, LINE_Y+LINE_H, LineColor[i]);
		
	}

	//显示灰度---------------------------------------------------------------------------
	for(i=0; i<320; i++) //test
	{
		grey = i%256;
		r = grey>>3;
		g = grey>>2;
		b = grey>>3;

		LineColor[i] = (r<<11)|(g<<5)|(b<<0);
	}

	for(i=0; i<LINE_W; i++)
	{
		if(i>LCD_W) return;//超出屏幕范围，不显示
		LCD_DrawLine(LINE_X+i, LINE_Y, LINE_X+i, LINE_Y+LINE_H, LineColor[i]);
		
	}
}
