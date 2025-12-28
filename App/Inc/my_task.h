/*
=================================================================================================================
 	模块名称 : 用户任务头文件
 	文件名称 : my_task.h
 	版    本 : V1.0.0
 	说    明 : 
=================================================================================================================
*/
#ifndef _MY_TASK_H_
#define _MY_TASK_H_

#ifndef MY_TASK_GLOBAL
#define MY_TASK_EXT	extern
#else
#define MY_TASK_EXT
#endif

#include "bsp.h"

//可修改参数--------------------------------------------------------------------
#define TEST_EN         0 //测试模式使能 未使用
#define TEST_KEY_EN     0 //通过按键测试基础功能

#define HW_VER "V1.10" //程序版本号

#if TEST_EN == 1 //测试模式下，缩短采样时间，打开数据显示

#define TIME_Preheat    (100*60*3) //预热3分钟
#define SAMPLE_NUM      20 //每次测量的次数
#define SAMPLE_FRE      1000 //采样频率 单位：1ms 每1s采样一次

#else //正常模式下参数

#define TIME_Preheat    (100*60*3) //预热3分钟
#define SAMPLE_NUM      20 //每次测量的次数
#define SAMPLE_FRE      1000 //采样频率 单位：1ms 每1s采样一次

#endif

//门限默认值
#define DEFAULT_THR_PRESSURE 650 //压力值
#define DEFAULT_THR_METLEV1 223.456 //甲醛浓度告警门限1
#define DEFAULT_THR_METLEV2 323.456 //甲醛浓度告警门限2

#define LED_USE1 0 //使用的LED, 0~2:LED1-LED3
#define LED_USE2 1 //使用的LED, 0~2:LED1-LED3

//CCD求最小值范围0~2500
#define CCD_DATA_START 	0//CCD求最小值起始坐标
#define CCD_DATA_END 	2500//CCD求最小值起始坐标

//--------------------------------------------------------------------可修改参数

#define LED_DUTY_CYCLE  50 //占空比,单位%
#define LED_FREQUENCE   1000 //频率值，单位Hz

#define PUMP_DUTY_CYCLE  98 //占空比,单位%
#define PUMP_FREQUENCE   20 //频率值，单位Hz


#define TCD1103_I_TIME I_7500US //TCD1103积分时间

// #define LINE_MAP_START 590 //TCD1103 条纹图形显示起始像素点， 0~1500 中选择320个点
#define LINE_MAP_START 1090 //TCD1254 条纹图形显示起始像素点， 0~2500 中选择320个点

//------------------------------------------------------------------可修改参数

//不可修改参数------------------------------------------------------------------
//定时器 单位 10ms
#define CCD_TYPE_1103 0
#define CCD_TYPE_1254 1
#define CCD_TYPE CCD_TYPE_1103
// #define CCD_TYPE CCD_TYPE_1254

#define TIME_LedToggle      100
#define TIME_BatIconToggle  100

//采样状态
#define SAMPLE_STOP 	0 //
#define SAMPLE_START 	1 //
#define SAMPLE_FINISH 	2 //
//------------------------------------------------------------------不可修改参数

MY_TASK_EXT	uint16_t timer_LedToggle;
MY_TASK_EXT	uint16_t timer_BatIconToggle;
MY_TASK_EXT	uint16_t timer_Sample;
MY_TASK_EXT	uint16_t timer_Preheat;

typedef enum
{
    STATE_IDLE=0, //空闲
    STATE_PREHEAT, //预热
    STATE_READY, //准备
    STATE_SAMPLE, //采集
    STATE_OK,
    STATE_ERROR,
    STATE_N
}TASK_STATE;


MY_TASK_EXT uint8_t task_n;

typedef enum
{
    TASK_INIT=0,   //初始化
    TASK_PREHEAT,   //预热
    TASK_SAMPLE_START, //采集开始
    TASK_AIR_OUT, //抽气
    TASK_SAMPLE, //采集中
    TASK_SAMPLE_STOP, //采集结束
    TASK_IDEL,
    TASK_ERROR,
    TASK_N
}TASK_ID;

//错误编码
typedef enum
{
    ERR_OK=0,
    ERR_ERROR,
    ERR_TIMEOUT,
    ERR_IDEL,
    ERR_N
}ENUM_ERR;


typedef struct
{
    uint16_t data_num; //数据总数
	uint16_t data_in; //
	uint16_t data_out; //
    uint8_t ComMode; //通信模式
    uint8_t nc; //
	float ThrMetLev1;  // 甲烷浓度门限1
	float ThrMetLev2;  // 甲烷浓度门限2

}TYPE_STRUCT_PARA_DATA;

typedef union
{
   TYPE_STRUCT_PARA_DATA Para;
   uint16_t Buf[sizeof(TYPE_STRUCT_PARA_DATA)];
}TYPE_UNION_SAVED_DATA;

MY_TASK_EXT TYPE_UNION_SAVED_DATA Saved;

#define COUNT_SEND 3
MY_TASK_EXT uint8_t counter_send;

#define LG_SEND_STOP            0 //停止上传
#define LG_SEND_SAMPLEDATA      1 //上传采集数据

MY_TASK_EXT uint8_t lg_SendData;

#define MASK_SYSCHK 0xFC00
// #define MASK_SYSCHK 0xDC00 //屏蔽氧气传感器

typedef struct
{
    uint16_t ChrgIn     :1;//充电中
    uint16_t ChrgDone   :1;//充电完成
    uint16_t PowLow     :1;//低电量告警
    uint16_t MetAlarm   :1;//甲烷浓度告警
    uint16_t NetWifi    :1;//wifi连接状态
    uint16_t NetBle     :1;//蓝牙连接状态
    uint16_t            :3;//
    uint16_t SysChkOk   :1;//系统自检完成标志
    uint16_t ErrExFlash :1;//外部FLASH故障
    uint16_t ErrBw      :1;//通信模组故障
    uint16_t ErrTh      :1;//温湿度传感器故障
    uint16_t ErrO2      :1;//氧气传感器故障
    uint16_t ErrP       :1;//气压传感器故障
    uint16_t ErrCcd     :1;//CCD传感器故障

}TYPE_DEV_STA_BIT;

typedef union
{
    TYPE_DEV_STA_BIT Bit;
    uint16_t All;
}TYPE_UNION_DEV_STA;

typedef struct //info
{
    float MetLev1;  // 甲烷浓度 门限1
    float MetLev2;  // 甲烷浓度 门限2
}TYPE_STRUCT_THRESHOLD; //Threshold

#define TIME_SAMPLE_ONCE 200 //实际采集一次的时间估算值,不影响实际采集时长，但会限制自动采集的频率，谨慎修改，不能太小

#define TIME_PUMP       200
#define TIME_SAVE_DATA  500
#define TIME_SEND_DATA  100
#define TIME_SYS_START  100
#define TIME_SAMPLE_AUTO 500 //自动采集频率,不能小于一次的采集时间 TIME_SAMPLE_ONCE

typedef struct //info
{
    uint16_t pump;
    uint16_t SaveData;
    uint16_t SendData;
    uint16_t SysStart;
    uint16_t AutoSample; //自动采集频率
}TYPE_STRUCT_TIMER; 

MY_TASK_EXT TYPE_STRUCT_TIMER Timer;

#define WORK_MODE_MANUAL    0 //手动模式
#define WORK_MODE_AUTO      1 //自动模式
#define GUI_MODE_WORK       0 //工作模式
#define GUI_MODE_RECORD     1 //历史记录浏览模式
#define GUI_MODE_DATATRAN   2 //数据传输模式

#define COM_MODE_CLOSE  0 //关闭
#define COM_MODE_BLE    1 //蓝牙模式
#define COM_MODE_WIFI   2 //WIFI模式

typedef struct //info
{
    uint8_t sample_count;
    uint8_t id;
    uint8_t pow;// 
    uint16_t Vbat; 
    uint8_t WorkState;
    uint8_t WorkMode; //工作模式
    uint8_t GuiMode; //显示模式
    uint8_t ComMode; //通信模式
    TYPE_UNION_DEV_STA Sta;
    TYPE_STRUCT_THRESHOLD Threshold;
    uint8_t AlarmLevel; //告警等级 0:正常 1：一级告警 2：二级告警
    uint8_t InfoType; //信息类型
    float Methane;  // 甲烷浓度
    float Temprature;  // 温度
    float Humidity; //湿度
    float Pressure; //气压
    float Oxygen; //氧气浓度
}TYPE_STRUCT_DEV; 

MY_TASK_EXT TYPE_STRUCT_DEV Dev;
MY_TASK_EXT TYPE_UNION_DEV_STA DevStaTem;

#define LEN_SAMPLE_DATA (sizeof(TYPE_STRUCT_SAMPLE_DATA))
typedef struct //info
{
    //28个字节
    uint16_t n; //记数
    uint8_t TIME[6];
    float Methane;       // 甲烷浓度
    float Temprature;    // 温度
    float Humidity;      // 湿度
    float Pressure;      //气压
    float Oxygen;        //氧气浓度

}TYPE_STRUCT_SAMPLE_DATA; 


typedef union
{
    TYPE_STRUCT_SAMPLE_DATA Data;
    uint8_t Buf[LEN_SAMPLE_DATA]; //28个字节
}TYPE_UNION_SAMPLE_DATA;

MY_TASK_EXT TYPE_UNION_SAMPLE_DATA SAMPLE_DATA[SAMPLE_NUM];

#define TEST_SIZE 16
MY_TASK_EXT uint8_t TEST1[TEST_SIZE],TEST2[TEST_SIZE];
MY_TASK_EXT TYPE_STRUCT_SAMPLE_DATA SAMPLE_NOW;

MY_TASK_EXT uint16_t sample_n, sample_in, sample_out;
MY_TASK_EXT uint8_t lg_sample;

MY_TASK_EXT void UsrParaInit(void);
MY_TASK_EXT void InputScan(void);

#endif