/*
*********************************************************************************************************
*
*	模块名称 : 滤波法
*	文件名称 : filter.c
*	版    本 : V1.0
*   M  C  U  : STM32F103RBT6
*   编 译 器 : IAR-Arm 8.22.2
*
*	说    明 : 滤波法
*              
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2019-03-19 robin   中位值平均滤波法
*
*	Copyright (C), 2019-2039
*
*********************************************************************************************************
*/


#define FILTER_GLOBAL
#include "filter.h"


void FILTER_Clear( void )  
{
    FilterTem.i_in = 0;
    FilterTem.lg_ok = 0;
}

uint8_t FILTER_Push( FILTER_DATA_TYPE data ) 
{  

    if(!FilterTem.lg_ok) FilterTem.Buf[ FilterTem.i_in++ ] = data;
    
    if( FilterTem.i_in > FILTER_N-1 ) 
    {
        FilterTem.i_in = 0;
        FilterTem.lg_ok = 1;
    }
    return FilterTem.i_in;
}


FILTER_DATA_TYPE FILTER_Get(void)  
{  
    FILTER_DATA_TYPE temData, value_buf[FILTER_N];  
    uint8_t count, i, j;  
    uint8_t filter_i = 0;
    uint64_t tem64;

    temData = FILTER_DATA_ERROR;
    filter_i = FilterTem.i_in;

    if(FilterTem.lg_ok)
    {
        for ( count=0; count<FILTER_N; count++ )  
        {   
            value_buf[count] = FilterTem.Buf[ count ];
        }
        
        for (j=0;j<FILTER_N-1;j++)
        {  
            for (i=0;i<FILTER_N-j-1;i++)
            {  
                if ( value_buf[i]>value_buf[i+1] )  
                {  
                    temData = value_buf[i];  
                    value_buf[i] = value_buf[i+1];  
                    value_buf[i+1] = temData;  
                }  
            }  
        }

        for (tem64=0,i=1; i<FILTER_N-1; i++)
        {  
            tem64 += value_buf[i];
        }  

        temData =(FILTER_DATA_TYPE)(tem64/(FILTER_N-2));
        FilterTem.lg_ok = 0;
    }

    return temData;
}



/******************************************************* END OF FILE ************************************************************/
