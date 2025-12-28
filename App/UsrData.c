/*
=================================================================================================================
 	模块名称 : 存储用户数据处理
 	文件名称 : UsrData.c
 	版    本 : V1.0.0
 	说    明 : 
=================================================================================================================
*/
#define USR_DATA_GLOBAL
#include "UsrData.h"

uint8_t UsrDataRxdBuf[50];
uint8_t UsrDataTxdBuf[50];

//初始化
void UsrData_Init(void)
{
    ;
}

//保存数据
void SaveFlashData(void)
{
	Saved.Para.ThrMetLev1 = Dev.Threshold.MetLev1;
	Saved.Para.ThrMetLev2 = Dev.Threshold.MetLev2;
    //保存数据
    STMFLASH_Write(FLASH_SAVE_PARA_ADDR,(uint16_t*)&Saved.Buf[0],sizeof(Saved.Buf)/2);
}

//读取数据
void ReadFlashData(void)
{
    //读取数据
    STMFLASH_Read(FLASH_SAVE_PARA_ADDR,(uint16_t*)&Saved.Buf[0],sizeof(Saved.Buf)/2);
    
   if(Saved.Para.data_in == 0xFFFF)
   {
       Saved.Para.data_in = 0;
   }

   if(Saved.Para.data_out == 0xFFFF)
   {
       Saved.Para.data_out = 0;
   }

   if(Saved.Para.ThrMetLev1 == 0xFFFF)
   {
       Saved.Para.ThrMetLev1 = DEFAULT_THR_METLEV1;
   }
	if(Saved.Para.ThrMetLev2 == 0xFFFF)
	{
	 Saved.Para.ThrMetLev2 = DEFAULT_THR_METLEV2;
	}

	Dev.Threshold.MetLev1 = Saved.Para.ThrMetLev1; //甲烷浓度门限1
	Dev.Threshold.MetLev2 = Saved.Para.ThrMetLev2; //甲烷浓度门限2
    Dev.GuiMode = GUI_MODE_WORK;
    Dev.WorkMode = WORK_MODE_MANUAL;
    GUI_UpdataRecord();
}

//查询已保存数据条数
uint16_t UsrData_GetSampleDataNum(void)
{
    if(Saved.Para.data_in >= Saved.Para.data_out)
        return Saved.Para.data_in - Saved.Para.data_out;
    else
        return RECORD_DATA_MAX-Saved.Para.data_out+Saved.Para.data_in;
}

//存储n条采样数据
void UsrData_SaveSampleData(uint16_t n)
{
    uint16_t paraLen, sampLen;
    uint32_t i_in, n1, n2, save_data_add;
    if (n == 0) return;
    //超限处理
    
    paraLen = sizeof(Saved.Buf)/2;
    sampLen = LEN_SAMPLE_DATA;//(sizeof(TYPE_STRUCT_SAMPLE_DATA));
    i_in = Saved.Para.data_in;
    n1 = i_in  + n;
    
    if(n1 >= RECORD_DATA_MAX) 
    {
        n2 = n1-RECORD_DATA_MAX;
        n1 = RECORD_DATA_MAX-i_in;
    }else
    {
        n1 = n;
        n2 = 0;
    }

    save_data_add = USR_DATA_ADD_DATA + i_in*sampLen;
    Saved.Para.data_in = i_in  + n1;

    W25Qxx_Save(save_data_add, &SAMPLE_DATA[0].Buf[0], sampLen*n1); //保存采集数据

    if(n2) //若果超限，循环存储
    {
        save_data_add = USR_DATA_ADD_DATA;
        Saved.Para.data_in = n2;
        W25Qxx_Save(save_data_add, &SAMPLE_DATA[n1].Buf[0], sampLen*n2); //保存采集数据
    }

    if(Saved.Para.data_in <= Saved.Para.data_out)
    {
        Saved.Para.data_out = Saved.Para.data_in+1; //数据追尾
        if(Saved.Para.data_out >= RECORD_DATA_MAX) //超限循环记录
        {
            Saved.Para.data_out = 0;
        }
    }
    STMFLASH_Write(FLASH_SAVE_PARA_ADDR,(uint16_t*)&Saved.Buf[0],sizeof(Saved.Buf)/2);//保存系统参数
}

//读取1条采样数据，并删除记录
uint16_t UsrData_ReadSampleData(uint8_t *Buf)
{
    uint16_t sampLen;
    uint32_t save_data_add;

    if(UsrData_GetSampleDataNum() == 0) return 0;
    
    sampLen = LEN_SAMPLE_DATA;//sizeof(TYPE_STRUCT_SAMPLE_DATA);

    save_data_add = USR_DATA_ADD_DATA+(Saved.Para.data_out*sampLen);
    //读取数据
    STMFLASH_Read(save_data_add, Buf, (sampLen/2));
    W25Qx_Read(save_data_add, Buf, sampLen);

    Saved.Para.data_out++;
    if(Saved.Para.data_out == Saved.Para.data_in)
    {
        Saved.Para.data_in = 0;
        Saved.Para.data_out = 0;
    }
    if(Saved.Para.data_out >= RECORD_DATA_MAX) //超限循环记录
    {
        Saved.Para.data_out = 0;
    }
    return UsrData_GetSampleDataNum();
}

//读取任意位置的1条采样数据，不删除记录
uint16_t UsrData_BrowseSampleData(uint8_t *Buf, uint16_t n)
{
    uint16_t sampLen, sampTotalNum;
    uint32_t save_data_add;
    sampTotalNum = UsrData_GetSampleDataNum();

    if(sampTotalNum == 0) return 0;
    if(n >= sampTotalNum) return 0;

    sampLen = LEN_SAMPLE_DATA;
    save_data_add = USR_DATA_ADD_DATA+((Saved.Para.data_out+n)*sampLen);

    //读取数据
    W25Qx_Read(save_data_add, Buf, sampLen);

    return UsrData_GetSampleDataNum();
}

//清除数据
uint16_t UsrData_ClearSampleData(void)
{
    Saved.Para.data_in = 0;
    Saved.Para.data_out = 0;
    STMFLASH_Write(FLASH_SAVE_PARA_ADDR, (uint16_t*)&Saved.Buf[0], LEN_PARA); //保存系统参数

    Dev.WorkState = STATE_READY; //显示“请开始测量”
    return 1;
}

//发送一条空数据
uint16_t UsrData_SendNuLData(void)
{
	uint16_t i,k;
    
	uint8_t len = LEN_SAMPLE_DATA;

	k=0;
	UsrDataTxdBuf[k++] = P_HEAD;
    UsrDataTxdBuf[k++] = Dev.id;
	UsrDataTxdBuf[k++] = P_ACK|P_CMD_NOTE;
	UsrDataTxdBuf[k++] = 0;
	UsrDataTxdBuf[k++] = 0;
    UsrDataTxdBuf[k++] = 0;
	UsrDataTxdBuf[k++] = 0;
	for(i=2; i<len; i++)
	{
		UsrDataTxdBuf[k++] = 0;
	}
	i = k;
	UsrDataTxdBuf[k++] = check_sum(UsrDataTxdBuf+1, i); //
	UsrDataTxdBuf[k++] = P_TAIL;

	comSendBuf( &BLE_COM, UsrDataTxdBuf, k);
	return 1;
}

//发送一条数据
uint16_t UsrData_SendOne(uint8_t *data, uint8_t len)
{
	uint16_t i,k, n;
	
    n = Record.dataSend+1;
	k=0;
	UsrDataTxdBuf[k++] = P_HEAD;
    UsrDataTxdBuf[k++] = Dev.id;
	UsrDataTxdBuf[k++] = P_ACK|P_CMD_NOTE;
	UsrDataTxdBuf[k++] = (uint8_t)(Record.dataTotal>>8);    //DATA_NUM;
    UsrDataTxdBuf[k++] = (uint8_t)(Record.dataTotal);
    UsrDataTxdBuf[k++] = (uint8_t)(n>>8);    //DATA_NUM;
    UsrDataTxdBuf[k++] = (uint8_t)(n);

	for(i=2; i<len; i++) //
	{
		UsrDataTxdBuf[k++] = *(data+i);
	}

	i = k;
	UsrDataTxdBuf[k++] = check_sum(UsrDataTxdBuf+1, i); //
	UsrDataTxdBuf[k++] = P_TAIL;

	comSendBuf( &BLE_COM, UsrDataTxdBuf, k);
	osDelay(100);

    if(Saved.Para.data_out >= Saved.Para.data_in)
    {
        Saved.Para.data_in = 0;
        Saved.Para.data_out = 0;
        
        STMFLASH_Write(FLASH_SAVE_PARA_ADDR, (uint16_t*)&Saved.Buf[0], LEN_PARA); //保存系统参数
    }
    
    return UsrData_GetSampleDataNum();
}

//发送多条数据
uint16_t UsrData_SandSampleData(void)
{
    uint16_t n;

    if(UsrData_GetSampleDataNum() == 0)
    {
        lg_SendData = LG_SEND_STOP;
        UsrData_SendNuLData();
    }else
    {
        n = UsrData_ReadSampleData(UsrDataRxdBuf);
        UsrData_SendOne(UsrDataRxdBuf, LEN_SAMPLE_DATA);
        if(n == 0)
        {
            DebugPrintStr("Upload data finished\r\n");
	        Dev.InfoType = INFO_UPLOADED;
            
	        lg_SendData = LG_SEND_STOP;
        }
    }
    return n;

}
