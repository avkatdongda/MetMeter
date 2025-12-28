/*
=================================================================================================================
 	模块名称 : LCD用户界面显示
 	文件名称 : gui.h
 	版    本 : V1.0.0
 	说    明 : 
=================================================================================================================
*/
#ifndef _GUI_H
#define _GUI_H

#ifndef GUI_GLOBAL

#define GUI_EXT extern
#else
#define GUI_EXT
#endif

#include "my_task.h"

GUI_EXT uint8_t GUI_STR[50];   //

GUI_EXT uint16_t colorBg;   //背景颜色

#define COLOR_BG 		WHITE//LGRAY
#define COLOR_FONT 		BLACK
#define COLOR_NORMAL 	BLACK
#define COLOR_ALARM 	RED
#define COLOR_SAFE 		GREEN
#define COLOR_LINE 		GRAY
#define COLOR_OFF 		GRAY
#define COLOR_ON 		BLUE
#define COLOR_DATA 		BLACK
#define COLOR_LINK 		BLUE
#define COLOR_ERROR		RED
#define COLOR_OK 		DARKGREEN
#define CURSOR_BACK_COLOR 	DARKBLUE //光标背景颜色
#define CURSOR_FRONT_COLOR 	WHITE //光标前景颜色

#define MASK_LINK0_LINK4 0x0F80

typedef struct
{

    uint16_t ChrgIn     :1;//充电中
	uint16_t ChrgDone   :1;//充电完成
	uint16_t PowLow     :1;//低电量告警
	uint16_t MetAlarm      :1;//甲烷浓度告警
	uint16_t NetWifi       :1;//wifi连接状态
	uint16_t NetBle     :1;//蓝牙连接状态
	uint16_t NC1        :1;//

	uint16_t ErrSht20   :1;//SHT20故障
	uint16_t ErrJxmO2   :1;//JxmO2故障
	uint16_t ErrTcd1103 :1;//Tcd1103故障
	uint16_t ErrExFlash :1;//外部FLASH故障
	uint16_t            :5;//



}TYPE_STRUCT_GUI_STA_BIT;

typedef union
{
    TYPE_STRUCT_GUI_STA_BIT Bit;
    uint16_t All;
}TYPE_UNION_GUI_STA;

#define GUI_TIME_BKL 5
#define GUI_TIME_SETCLOCK 5

typedef struct
{
	 TYPE_UNION_GUI_STA Sta;
	uint8_t WorkState;//
	uint8_t DevId;
	uint8_t InfoType;
	uint8_t pow;// 
	uint16_t vbat;// mv
	uint32_t vAdc;// mv
	uint16_t RxdNum;//
	uint8_t Seconds;// 
	uint8_t Minutes;
	uint8_t Hours;
	uint8_t WorkSta;// 
	uint8_t ComMode;// 
	RTC_DateTypeDef Date;
	uint16_t timer_bkl;
	uint16_t timer_setclock;

	uint8_t mode; //模式 0：工K作模?? 1：配??模式
    
}TYPE_GUI;

GUI_EXT TYPE_GUI Gui;

GUI_EXT TYPE_UNION_GUI_STA StaTem;

GUI_EXT RTC_DateTypeDef SysData;
GUI_EXT RTC_TimeTypeDef SysTime;

GUI_EXT RTC_DateTypeDef SetData;
GUI_EXT RTC_TimeTypeDef SetTime;

GUI_EXT uint16_t timer_net;

#define PAGE_MAIN 		0
#define PAGE_RECORD 	1
#define PAGE_DATATRAN 	2

GUI_EXT uint8_t page, pageTem;
GUI_EXT uint8_t page_n;

#define RECORD_DATA_MAX 		1000  	//最大数记录条数
#define RECORD_DATA_ONEPAGE 	10  		//每页显示记录条数
typedef struct
{
	uint16_t dataTotal;//数据总条数
	uint16_t pageTotal;//总页数
	uint16_t pageNow;//当前页
	uint16_t pageTem;//
	uint16_t pageKey;//
	uint16_t dataSend;//已发送调试
	uint16_t dataSendTem;//
}TYPE_STRUCT_RECORD;
GUI_EXT TYPE_STRUCT_RECORD Record;

GUI_EXT uint8_t lg_DisLineMap;


#define INFO_NORMAL		0
#define INFO_ALARM 		1
#define INFO_MEASURING 	2
#define INFO_MEASURED 	3
#define INFO_UPLOADING 	4
#define INFO_UPLOADED 	5
#define INFO_COM_ERROR 	6 

GUI_EXT enum{
	CHK_START = 0,
	CHK_EXFLASH,
	CHK_TH,
	CHK_O2,
	CHK_P,
	CHK_CCD,
	CHK_BW
}TYPE_ENUM_SYSCHECK;

GUI_EXT enum{
	CHK_NORMAL = 0,
	CHK_OK,
	CHK_ERROR,
	CHK_SETMODE
}TYPE_ENUM_ERROR;


GUI_EXT void GUI_Init( void );
GUI_EXT void GUI_Clear( void );
GUI_EXT void GUI_BlkOn( void );
GUI_EXT void GUI_BlkOff( void );
GUI_EXT void GUI_BlkFlash( uint8_t n );

GUI_EXT void GUI_ClrTimeTem(void);
GUI_EXT void GUI_DisFrame(void); //显示框架
GUI_EXT void GUI_DisBanner(void);
GUI_EXT void GUI_DisDate(void);
GUI_EXT void GUI_DisTime(void);
GUI_EXT void GUI_SetTime(void);
GUI_EXT void GUI_SetData(void);
GUI_EXT void GUI_DisData(void);
GUI_EXT void GUI_DisDevId(void);
GUI_EXT void GUI_DisMid(void);
GUI_EXT void GUI_DisTop(void);

GUI_EXT void GUI_Black( void );
GUI_EXT void GUI_DisPow(void);
GUI_EXT void GUI_DisTestData(void);
GUI_EXT void GUI_TestModeOn(void);
GUI_EXT void GUI_TestModeOff(void);
GUI_EXT void GUI_DisLed(void);

GUI_EXT void GUI_DisData(void);
GUI_EXT void GUI_DisRecord(void); //历史记录界面
GUI_EXT void GUI_DisDataTran(void); //传输数据界面
GUI_EXT void GUI_Display(void); //传输数据界面
GUI_EXT void GUI_DisLineMap(uint16_t *v12);
GUI_EXT void GUI_DisTHOP(void);
GUI_EXT void GUI_DisSysCheck(uint8_t type, uint8_t err); //
GUI_EXT void GUI_UpdataRecord(void); //更新记录
GUI_EXT void GUI_DisInfoNow(uint8_t *info); //
GUI_EXT void GUI_DisWifiIcon(void);

#endif
