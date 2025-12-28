/*
=================================================================================================================
 	模块名称 : 蓝牙wifi驱动
 	文件名称 : Ble.h
 	版    本 : V1.0.0
 	说    明 : 
=================================================================================================================
*/

#ifndef BLE_H
#define BLE_H

#ifndef BLE_GLOBAL
#define BLE_EXT	extern
#else
#define BLE_EXT
#endif

#include "bsp.h"

#define BLE_COM     huart4 //蓝牙wifi模块串口
#define BW_INDEX    UART4INDEX //串口索引


// #define SERVER_IP "192.168.18.11"   //TCP服务器IP地址
// #define SERVER_PORT "6666"          //TCP服务器端口号
// #define ROUTER_NAME "CU_rN8C"       //路由器名称
// #define ROUTER_PSWD "w634rmwj"      //路由器密码

#define SERVER_IP "192.168.3.2"   //TCP服务器IP地址
#define SERVER_PORT "6666"          //TCP服务器端口号
#define ROUTER_NAME "kedaya"       //路由器名称
#define ROUTER_PSWD "648487771"      //路由器密码

#define BLE_DATATYPE_NUL 0 //
#define BLE_DATATYPE_CMD 1 //
#define BLE_DATATYPE_DAT 2 //

#define AT_TIMEOUT   500
BLE_EXT uint8_t AT_TEM[20];
#define BLE_BUF_SIZE 100
BLE_EXT uint8_t BLETem[BLE_BUF_SIZE];

#define BW_POW_ON()  HAL_GPIO_WritePin(BW_EN_GPIO_Port, BW_EN_Pin, GPIO_PIN_RESET) //电源开
#define BW_POW_OFF() HAL_GPIO_WritePin(BW_EN_GPIO_Port, BW_EN_Pin, GPIO_PIN_SET)   //电源关
#define BLE_SLEEP()  ;// HAL_GPIO_WritePin(BT_SLEEP_GPIO_Port, BT_SLEEP_Pin, GPIO_PIN_SET)
#define BLE_WAKEUP() ;// HAL_GPIO_WritePin(BT_SLEEP_GPIO_Port, BT_SLEEP_Pin, GPIO_PIN_RESET)

//命令状态
#define AT_OK           0 //正确
#define AT_ERROR        1 //错误
#define AT_ERR_TIMEOUT  2 //超时

#define BLE_ERROR_MAX   6 //允许出错次数

#define BLE_SLEEP_EN    0 //休眠使能

BLE_EXT uint8_t ble_task;
BLE_EXT uint8_t ble_err;
BLE_EXT uint8_t lg_BwAtMode;
#define BLE_ERR_COUNT 3
BLE_EXT uint8_t ble_err_counter;

enum BLE_ATTYPE{
    BW_POW_ON, //BLE上电
    BLE_Q_MODE,
    BLE_S_MODE,
    BLE_TRANS, //透传
    ATE,
    CPIN, //
    CSQ, //查�?信号质量
   
    CONNECT, //
    BLE_SOFTRESET, 
    BLE_HWRESET, 
    BLE_SHUTDOWN,
    BLE_IDLE
};

enum TYPE_BLE_STATE_{
    OFFLINE,    //下线
    LOGIN,      //登录�?
    ONLINE,     //上线
    ERR,         //故障
    ERR_SIM_REMOVED //无SIM�?
};

#define BLE_TIME_NOACK 10000

typedef struct
{
    uint8_t sta;    //状�??
    uint8_t sta_c;  //状�?��?�?
    uint8_t ssi; //<rssi>应在0�?31之间(�?99表示无信号），数值越大表明信号质量越�?
    uint8_t ber; //�?���?
    uint16_t timer_noack; //无应答时�?
}TYPE_BLE;

typedef struct
{
    uint16_t Pow    :1;//
    uint16_t Sleep  :1;//
    uint16_t Ready  :1;//
    uint16_t Mac    :1;//
    uint16_t NetBle :1;//
    uint16_t NetWifi:1;//
    uint16_t Error  :1;//
    uint16_t        :9;//

}TYPE_STRUCT_BLE_STA_BIT;

typedef union
{
    TYPE_STRUCT_BLE_STA_BIT Bit;
    uint16_t All;
}TYPE_UNION_BLE_STA;

#define BW_MODE_CLOSE   0
#define BW_MODE_BLE     1
#define BW_MODE_WIFI    2

typedef struct
{
    TYPE_UNION_BLE_STA Sta;
    uint8_t mode; //
    uint8_t DevId; //
    uint8_t macType; //MAC类型
    uint8_t ssi; //信号质量
    uint8_t MAC[6];    //MAC地址
    uint8_t DevName[20];  //设备名称
}TYPE_STRUCT_BLE_DEV;

BLE_EXT TYPE_STRUCT_BLE_DEV Ble;

typedef struct
{
    char MAC[13];    //MAC地址
    char NAME[20];  //设备名称
}TYPE_STRUCT_BLE_ID;


BLE_EXT TYPE_STRUCT_BLE_ID BleId[6];

#define AUTO_CFG "AT+AUTO_CFG"

BLE_EXT uint8_t at_cmd(char *cmd, char *ack, uint16_t t);
BLE_EXT void BLE_Init(void);
BLE_EXT uint8_t BLE_GetDataAndCmd(uint8_t *TEM, uint8_t *len);
// BLE_EXT void BLE_InitTask(void);
BLE_EXT void BLE_RxdTask(void);

BLE_EXT uint8_t AT_MAC(uint8_t *mac);
BLE_EXT uint8_t AT_AUTO_MAC(uint8_t *mac);
BLE_EXT uint8_t AT_AUTO_CFG_ON(void);
BLE_EXT uint8_t AT_NAME(uint8_t *name);
BLE_EXT uint8_t AT_REBOOT(void);
BLE_EXT uint8_t AT_AUTO_DEL(void);

BLE_EXT uint8_t BW_GetBleMode(void);
BLE_EXT void BW_SetBleSlaveMode(void);
BLE_EXT uint8_t BW_UartAtTask(void); //串口AT指令透传
BLE_EXT uint8_t BW_SetBleMode(void);
BLE_EXT uint8_t BW_SetWifiMode(void);
BLE_EXT uint8_t BW_PoweOn(void); //开机
#endif

/******************************************************* END OF FILE ************************************************************/
