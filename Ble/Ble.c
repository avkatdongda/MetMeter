/*
=================================================================================================================
 	模块名称 : 蓝牙wifi驱动
 	文件名称 : Ble.c
 	版    本 : V1.0.0
 	说    明 : 
=================================================================================================================
*/

#define BLE_GLOBAL
#include "Ble.h"
// #include "bsp.h"
TYPE_BLE BLE;

void BLE_Init(void)
{
    uint8_t err;
    lg_BwAtMode = 0;
    Ble.Sta.All = 0;
    // Ble.DevId == 0xFF;
    
    BW_POW_ON();
    
    if(Dev.ComMode == BW_MODE_BLE) 
    {
        err = BW_SetBleMode();
    }
    else
    if(Dev.ComMode == BW_MODE_WIFI) 
    {
        err = BW_SetWifiMode();
    }

    if(err == AT_OK)
    {
        Dev.Sta.Bit.ErrBw = 0;
    }else
    {
        Dev.Sta.Bit.ErrBw = 1;
    }
    // BW_POW_OFF();
}
    
uint8_t BW_PoweOn(void) //开机
{
    uint8_t err;
    lg_BwAtMode = 0;
    Ble.Sta.All = 0;
    
    Dev.Sta.Bit.NetBle = 0;
    Dev.Sta.Bit.NetWifi = 0;

    BW_POW_OFF();
    osDelay(500);
    BW_POW_ON();
    osDelay(100);

    if(Dev.ComMode == BW_MODE_BLE) 
    {
        err = BW_SetBleMode();
    }
    else
    if(Dev.ComMode == BW_MODE_WIFI) 
    {
        err = BW_SetWifiMode();
    }

    if(err == AT_OK)
    {
        Dev.Sta.Bit.ErrBw = 0;
    }else
    {
        Dev.Sta.Bit.ErrBw = 1;
    }
    // BW_POW_OFF();
    return err;
}

void BLE_PowRst(void)
{
    BW_POW_OFF();
    osDelay(3000);
    BW_POW_ON();
    osDelay(1000);
}

//static uint8_t BLE_Rxd(void)
//{
//	return 1;
//}


            
            
static uint8_t BLE_Txd(uint8_t *data, uint16_t len)
{
    comSendBuf( &BLE_COM, (uint8_t*)data, len);
    return 1;
}



uint8_t at_cmd(char *cmd, char *ack, uint16_t t)
{
    uint16_t qn;
    qn = t;

    if(*(cmd) != ' ')
    {
        comSendBuf( &BLE_COM, (uint8_t*)cmd, strlen(cmd));
        DebugPrintStr(cmd);//
    }else
    {
        DebugPrintStr("wait:");//
        DebugPrintStr(ack);//
        DebugPrintStr("\r\n");//
    }
    while( qn )
    {
        if( comGetALine(&BLE_COM, BLETem) )//
        {
            BLE.timer_noack = BLE_TIME_NOACK;

            qn = t;
            DebugPrintStr(BLETem);
            if(strstr((char*)BLETem,(char*)ack))
            {
                //comSendBuf(&BLE_COM, ATCMD, strlen((char*)ATCMD));
               
                return AT_OK;
            }
        }
        else
        {
            qn--;
            osDelay(1);
        }
    }
    
    return AT_ERROR;
}

// AT+DEV?
uint8_t AT_DEV(void)
{
    uint8_t err=0;
    err = at_cmd("AT+DEV?\r\n", "+DEV:", AT_TIMEOUT);
    return err;
}

// AT+MAC=F1F2F3F4F500
uint8_t AT_MAC(uint8_t *mac)
{
    uint8_t err=0;
    
    sprintf(AT_TEM, "AT+MAC=%s\r\n", mac);
    err = at_cmd(AT_TEM, "OK", 2000); //AT+AUTO_MAC
    return err;
}

//删除自动重连列表 
uint8_t AT_AUTO_DEL(void)
{
    uint8_t err=0;
    err = at_cmd("AT+AUTO_DEL\r\n", "OK", AT_TIMEOUT); //
    return err;
}

//开启自动重连
uint8_t AT_AUTO_CFG_ON(void)
{
    uint8_t err=0;
    err = at_cmd("AT+AUTO_CFG=1\r\n", "OK", AT_TIMEOUT); //
    return err;
}

// AT+AUTO_MAC
uint8_t AT_AUTO_MAC(uint8_t *mac)
{
    uint8_t err=0;
    
    sprintf(AT_TEM, "AT+AUTO_MAC=%s\r\n", mac);
    err = at_cmd(AT_TEM, "OK", 2000); //AT+AUTO_MAC
    return err;
}

// AT+NAME=
uint8_t AT_NAME(uint8_t *name)
{
    uint8_t err=0;
    
    sprintf(AT_TEM, "AT+NAME=%s\r\n", name);
    err = at_cmd(AT_TEM, "OK", 2000);
    return err;
}

// AT+REBOOT=1
uint8_t AT_REBOOT(void)
{
    uint8_t err=0;
    err = at_cmd("AT+REBOOT=1\r\n", "+READY", 3000); //
    return err;
}

uint8_t BLE_SetAutoMac(uint8_t *mac)
{
    uint8_t i=0, err=0;
    for(i=0; i<5; i++)
    {
//        err = AT_AutoMac(&BleDev[i].MAC[0], 5000);
        if(err != AT_OK)
        {
            return ERR_ERROR;
        }
    }
    return ERR_OK;
}

// +CONNECTED
//static uint8_t AT_GetLinkMac(uint16_t t)
//{
//    uint8_t err, i=0;
//    uint16_t qn;
//    char* pch, *ptr;
//    qn = t;
//
//    char TEM[5];
//
//    err = strstr((char*)BLETem,"+CONNECTED:");
//
//    comSendBuf( &BLE_COM, "AT+CSQ\r\n", strlen("AT+CSQ\r\n"));
//    while (qn)
//    {
//        if (comGetALine(&BLE_COM, BLETem))//
//        {
//            BLE.timer_noack = BLE_TIME_NOACK;
//
//            qn = t;
////            LED_R_ON();
//            DebugPrintStr(BLETem);
//            //+CSQ: 19,99
//            pch = strstr((char*)BLETem,"+CONNECTED:");
//            if (pch != NULL)
//            {
//                pch += 6;
//
//                while((*(pch+i) !=','))
//                {
//                    TEM[i] = *(pch+i);
//                    i++;
//                    if(i>2) return AT_ERROR;
//                }
//                TEM[i++] = 0;
//                BLE.ssi = atoi(TEM);
//                pch += i;
//                i=0;
//                while((*(pch+i) !=0x0D))
//                {
//                    TEM[i] = *(pch+i);
//                    i++;
//                    if(i>2) return AT_ERROR;
//                }
//                TEM[i++] = 0;
//                BLE.ber = atoi(TEM);
//
//                if ( (BLE.ssi != 99) && (BLE.ssi>10) )
//                {
//                    return AT_OK;
//                }else
//                {
//                    osDelay(1000);
//                    return AT_ERROR;
//                }
//            }
//        }
//        else
//        {
//            qn--;
//            osDelay(1);
//        }
////        LED_R_OFF();
//
//    }
//
//    return AT_ERROR;
//}

//static uint8_t at_getcsq(uint16_t t)
//{
//    uint8_t i=0;
//    uint16_t qn;
//    char* pch, *ptr;
//    qn = t;
//
//    char TEM[5];
//
//    comSendBuf( &BLE_COM, "AT+CSQ\r\n", strlen("AT+CSQ\r\n"));
//    while (qn)
//    {
//        if (comGetALine(&BLE_COM, BLETem))//�õ�??����??
//        {
//            BLE.timer_noack = BLE_TIME_NOACK;
//
//            qn = t;
////            LED_R_ON();
//            DebugPrintStr(BLETem);
//            //+CSQ: 19,99
//            pch = strstr((char*)BLETem,"+CSQ:");
//            if (pch != NULL)
//            {
//                pch += 6;
//
//                while((*(pch+i) !=','))
//                {
//                    TEM[i] = *(pch+i);
//                    i++;
//                    if(i>2) return AT_ERROR;
//                }
//                TEM[i++] = 0;
//                BLE.ssi = atoi(TEM);
//                pch += i;
//                i=0;
//                while((*(pch+i) !=0x0D))
//                {
//                    TEM[i] = *(pch+i);
//                    i++;
//                    if(i>2) return AT_ERROR;
//                }
//                TEM[i++] = 0;
//                BLE.ber = atoi(TEM);
//
//                if ( (BLE.ssi != 99) && (BLE.ssi>10) )
//                {
//                    return AT_OK;
//                }else
//                {
//                    osDelay(1000);
//                    return AT_ERROR;
//                }
//            }
//        }
//        else
//        {
//            qn--;
//            osDelay(1);
//        }
////        LED_R_OFF();
//
//    }
//
//    return AT_ERROR;
//}

void BLE_TimerInt(void)
{
    if (BLE.timer_noack) BLE.timer_noack--;
}

// void BLE_PowerOn(void)
// {
//     BW_POW_ON();
//     Ble.Sta.Pow = 1;
// }

// void BLE_PowerOff(void)
// {
//     BW_POW_OFF();
//     Ble.Sta.Pow = 0;
// }

uint8_t BLE_GetDataAndCmd(uint8_t *TEM, uint8_t *RxdLen)
{
    uint8_t dataType;
	uint16_t i, len;
    uint8_t temC;
    uint16_t timeout;
    
    i = 0;
    temC = 0;
	timeout = 0;

    dataType = BLE_DATATYPE_CMD;

	len = getQueueElemNum(&uartRcvFifo[BW_INDEX]);
   
    if(len>2)
    {
        while( timeout<200)//
        {
            if(GetElemFromQueue(&uartRcvFifo[BW_INDEX], &temC))
            {
                timeout = 0;
                if(dataType == BLE_DATATYPE_CMD)
                {
                    TEM[i++] = temC;
                    if(temC == P_HEAD)
                    {
                        i=0;
                        TEM[i++] = temC;
                        dataType = BLE_DATATYPE_DAT;
                    }else
                    if(temC==0x0A)
                    {
                        dataType = BLE_DATATYPE_CMD;
                        if( i>2 )
                        {
                            // if( TEM[i-2] == 0x0D )//检测到AT指令
                            {
                                TEM[i++] = 0;
                               
                                *RxdLen = i-1;
                                //  return i-1;
                                return BLE_DATATYPE_CMD;
                            }
                        }else
                        {
                            return BLE_DATATYPE_NUL;
                        }
                    }
                }else
                if(dataType == BLE_DATATYPE_DAT)
                {
                    if(i == 0)
                    {
                        if(temC == P_HEAD)
                        {
                            TEM[i++] = temC;
                        }
                    }else
                    {
                        TEM[i++] = temC;
                        if(temC == P_TAIL)
                        {
                            *RxdLen = i;
                            return BLE_DATATYPE_DAT;
                        }
                    }

                }
            }
            else
            {
                osDelay(1);
                timeout++;
            }
            
            
        }
    }   
	return BLE_DATATYPE_NUL;
}

// void BLE_InitTask(void)
void BW_SetBleSlaveMode(void)
{
    ble_task = BW_POW_ON;
    Dev.Sta.Bit.ErrBw = 0;
	for(;;)
	{
       osDelay(1000);
       if(lg_BwAtMode) continue;

		switch(ble_task)
		{
			case BW_POW_ON :
            case BLE_HWRESET :
                DebugPrintStr("BLE_HWRESET\r\n");//
                osDelay(1000);
                ble_err_counter = 0;
                
                BLE_PowRst();
                ble_task = BLE_Q_MODE;
				break;
            case BLE_Q_MODE :
                ble_err = BW_GetBleMode();
                if(ble_err == 0)
                {
                    ble_err_counter = 0;
                    // Ble.Sta.Bit.Error = 0;
                    // Dev.Sta.Bit.Err = 0;
                    // Ble.Sta.Bit.Ready = 1;
                    ble_task = BLE_IDLE;
                }else
                if(ble_err == 255)
                {
                    ble_err_counter++;
                    if(ble_err_counter > BLE_ERR_COUNT)
                    {
                        ble_err_counter = 0;
                        // Ble.Sta.Bit.Error = 1;
                        // Dev.Sta.Bit.Err = 1;
                        ble_task = BLE_HWRESET;
                    }
                }else
                {
                    ble_task = BLE_S_MODE;
                }
				break;
			case BLE_S_MODE : //
                ble_err = at_cmd("AT+BLEMODE=0\r\n","OK",5000);
                if(ble_err == AT_OK)
                { 
                    Dev.Sta.Bit.ErrBw = 0;
                    ble_err_counter = 0;
                    ble_task = BLE_IDLE;
                }else
                {
                    ble_err_counter++;
                    if(ble_err_counter > BLE_ERR_COUNT)
                    {
                        ble_err_counter = 0;
                        // Ble.Sta.Bit.Error = 1;
                        Dev.Sta.Bit.ErrBw = 1;
                    }
                }
				break;
            case BLE_SOFTRESET :
                // ble_err = at_cmd("AT+REBOOT=1\r\n","OK",5000);
				// ble_task = BLE_TRANS;
				break;

            case BLE_TRANS :
                
				// ble_task = BLE_IDLE;
				break;
			case BLE_IDLE :
                DebugPrintStr("BLE_IDLE\r\n");//
                return;
				// break;
			default:
				break;
		}
		
	}
}

uint8_t BW_SetBleMode(void)
{
    uint8_t errNum = 0;
    Dev.Sta.Bit.ErrBw = 0;
	for(;;)
	{
        errNum++;
        if(errNum > 10 ) return AT_ERROR;
        osDelay(1);
        // ble_err = at_cmd("AT+RST\r\n","OK",5000);
        // if(ble_err != AT_OK) continue;
        ble_err = at_cmd("AT+BLEMODE=0\r\n","OK",1000);
        if(ble_err != AT_OK) continue;
        ble_err = at_cmd("AT+WAUTOCONN=0\r\n","OK",1000);
        if(ble_err != AT_OK) continue;
        // ble_err = at_cmd("AT+RST","OK",5000);
        // if(ble_err != AT_OK) continue;

        return AT_OK;
	}
}



// AT+SOCKETAUTOTT=4,192.168.18.11,6666
#define AT_SOCKETAUTOTT  "AT+SOCKETAUTOTT=4,"SERVER_IP","SERVER_PORT"\r\n"
//AT+WJAP="CU_rN8C","w634rmwj"
#define AT_WJAP "AT+WJAP=\""ROUTER_NAME"\",\""ROUTER_PSWD"\"\r\n"

uint8_t BW_SetWifiMode(void)
{
    uint8_t errNum = 0;
    Dev.Sta.Bit.ErrBw = 0;
	for(;;)
	{
        errNum++;
        if(errNum > 10 ) return AT_ERROR;
        osDelay(1);
        // ble_err = at_cmd("AT+RST\r\n","OK",5000);
        // if(ble_err != AT_OK) continue;
        // ble_err = at_cmd("+++"," ",500);
        // if(ble_err != AT_OK) continue;
        
        ble_err = at_cmd("AT+WAUTOCONN=0\r\n","OK",2000);
        if(ble_err != AT_OK) continue;
        ble_err = at_cmd("AT+WMODE=1,1\r\n","OK",2000);
        if(ble_err != AT_OK) continue;
        ble_err = at_cmd(AT_WJAP,"OK",5000);
        if(ble_err != AT_OK) continue;
        ble_err = at_cmd(AT_SOCKETAUTOTT,"OK",2000);
        if(ble_err != AT_OK) continue;
        ble_err = at_cmd("AT+RST","WIFI_GOT_IP",5000);
        if(ble_err != AT_OK) continue;

        return AT_OK;
	}
}

void BLE_RxdTask(void)
{
    uint8_t tem8, rxdLen, txdLen;

    tem8 = BLE_GetDataAndCmd(BLETem, &rxdLen);
    
    if(tem8 == BLE_DATATYPE_DAT)
    {
        DebugPrintStrHex(BLETem, rxdLen);
        
        txdLen = P_Pares(BLETem, rxdLen);
        if(txdLen)
        {
            BLE_Txd(BLETem, txdLen);
        }
    }else
    if(tem8 == BLE_DATATYPE_CMD)
    {
        DebugPrintStr(BLETem);
        
        if(strstr((char*)BLETem, "ready"))
        {
            Ble.Sta.Bit.Ready = 1;
            Dev.Sta.Bit.ErrBw = 0;
        }else
        if(strstr((char*)BLETem, "+EVENT:BLE_CONNECT"))
        {
            Ble.Sta.Bit.NetBle = 1;
            Dev.Sta.Bit.NetBle = Ble.Sta.Bit.NetBle;
        }else
        if(strstr((char*)BLETem, "+EVENT:BLE_DISCONNECT"))
        {
            Ble.Sta.Bit.NetBle = 0;
            Dev.Sta.Bit.NetBle = Ble.Sta.Bit.NetBle;
        }else
         if(strstr((char*)BLETem, "+EVENT:WIFI_GOT_IP"))
        {
            Ble.Sta.Bit.NetWifi = 1;
            Dev.Sta.Bit.NetWifi = Ble.Sta.Bit.NetWifi;
        }else
        if(strstr((char*)BLETem, "connect success"))
        {
            Ble.Sta.Bit.NetWifi = 1;
            Dev.Sta.Bit.NetWifi = Ble.Sta.Bit.NetWifi;
        }else
        if(strstr((char*)BLETem, "SocketReconnect"))
        {
            Ble.Sta.Bit.NetWifi = 1;
            Dev.Sta.Bit.NetWifi = Ble.Sta.Bit.NetWifi;
        }
    }else
    {

    }
}


uint8_t BW_GetBleMode(void)
{
    uint8_t tem8;
    uint16_t qn;
    char* pch;
    qn = 1000;
    
    comSendBuf( &BLE_COM, "AT+BLEMODE?\r\n", strlen("AT+BLEMODE?\r\n"));
    while (qn)
    {
        if (comGetALine(&BLE_COM, BLETem))//
        {
            BLE.timer_noack = BLE_TIME_NOACK;
            
            DebugPrintStr(BLETem);
            pch = strstr((char*)BLETem,"+BLEMODE:");
            if (pch != NULL)
            {
                tem8 = *(pch+9);

                //0：从机模式 1：主机模式 2：iBeacon 模式 9：蓝牙关闭
                if( tem8 == '0' ) return 0;
                else 
                if( tem8 == '1' ) return 1;
                else 
                if( tem8 == '2' ) return 2;
                else 
                if( tem8 == '9' ) return 9;
                else  return 255;
            }
        }
        else
        {
            qn--;
            osDelay(1);
        }
    }
    
    return 255;
}

uint8_t BW_UartAtTask(void) //串口AT指令透传
{
    uint8_t len1, len2, bwRxd[512], debugRxd[512];
    for(;;)
	{
        
		osDelay(1);
		// if(GetElemFromQueue(&uartRcvFifo[BW_INDEX], &bwRxd))
		// {
		// 	comSendBuf(&DEBUG_COM, &bwRxd, 1);
		// }
        // if(GetElemFromQueue(&uartRcvFifo[DEBUG_INDEX], &debugRxd))
		// {
		// 	comSendBuf(&BLE_COM, &debugRxd, 1);
		// }
        len1 = comGetALine(&DEBUG_COM, debugRxd);
        if(len1)
        {
            comSendBuf(&BLE_COM, debugRxd, len1);
        }
        len2 = comGetALine(&BLE_COM, bwRxd);
        if(len2)
        {
            comSendBuf(&DEBUG_COM, bwRxd, len2);
        }
	}

}
