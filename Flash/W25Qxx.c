/*
******************************************************************************
*       函 数 名：W25Qxx_ReadID
*       函数功能：读取W25Qxx外设ID
*       形    参：无
*       返 回 值：ID
*******************************************************************************
*/
#include "bsp.h"

uint16_t W25Qxx_ReadID(void)
{
  uint16_t id;
  uint8_t Txbuff[4] ={READ_ID_CMD,0,0,0}; 
  W25QXX_CS_LOW;
  HAL_SPI_Transmit(&W25QXX_HSPI,(uint8_t *)Txbuff,4,1000);
  HAL_SPI_Receive(&W25QXX_HSPI,(uint8_t *)&id,2,1000);  
  W25QXX_CS_HIGH;
  
  return id;
}

static uint8_t W25Qxx_GetStatus(void)
{
   uint8_t result;
   uint8_t Txbuff[4] ={READ_STATUS_REG1_CMD,0,0,0};
   
   W25QXX_CS_LOW;
   if(HAL_OK == HAL_SPI_Transmit(&W25QXX_HSPI,(uint8_t *)Txbuff,1,1000))
   {
      if(HAL_OK == HAL_SPI_Receive(&W25QXX_HSPI,&result,1,1000))
      {
        W25QXX_CS_HIGH; //+
        return result;
      }    
   }
   W25QXX_CS_HIGH;
   
   return 0;
}

/*
******************************************************************************
*       函 数 名：W25Qxx_Wait_Busy
*       函数功能：等待W25xx驱动
*       形    参：无
*       返 回 值：无
*******************************************************************************
*/
void W25Qxx_Wait_Busy(void)
{
  while((W25Qxx_GetStatus() & 0x01) == 0x01);
}


/*
******************************************************************************
*       函 数 名：W25Qxx_Write_Enable、W25Qxx_Write_Disable
*       函数功能：写命令使能、失能
*       形    参：无
*       返 回 值：无
*******************************************************************************
*/
void W25Qxx_Write_Enable(void)
{
  uint8_t TxBuff[1] ={WRITE_ENABLE_CMD};
  W25QXX_CS_LOW;
  HAL_SPI_Transmit(&W25QXX_HSPI,TxBuff,1,1000);
  W25QXX_CS_HIGH;
  
  W25Qxx_Wait_Busy();                   //等待W25Qxx空闲
}
void W25Qxx_Write_Disable(void)
{
  uint8_t TxBuff[1] ={WRITE_DISABLE_CMD};
  W25QXX_CS_LOW;
  HAL_SPI_Transmit(&W25QXX_HSPI,TxBuff,1,1000);
  W25QXX_CS_HIGH;
  
  W25Qxx_Wait_Busy();                   //等待W25Qxx空闲
}

/*
******************************************************************************
*       函 数 W25Qx_Write
*       函数功能：写入数据
*       形    参：pData：接收数据、WriteAddr:写入地址、Size：写入字节数
*       返 回 值：0:成功；-1：error
*******************************************************************************
*/
uint8_t W25Qx_Write(uint32_t WriteAddr, uint8_t* pData, uint32_t Size)
{
  uint8_t cmd[4];
  cmd[0] =PAGE_PROG_CMD;
  cmd[1] =(uint8_t)(WriteAddr >>16);
  cmd[2] =(uint8_t)(WriteAddr >>8);
  cmd[3] =(uint8_t)WriteAddr;
  
  W25Qxx_Write_Enable();
  W25QXX_CS_LOW;
  HAL_SPI_Transmit(&W25QXX_HSPI,(uint8_t *)cmd,4,1000);
    if(HAL_OK != HAL_SPI_Transmit(&W25QXX_HSPI,pData,Size,1000))
    {
      W25QXX_CS_HIGH; //+
      return -1;
    }
  W25QXX_CS_HIGH;
  W25Qxx_Wait_Busy();

  return 0;
}

/*
******************************************************************************
*       函 数 名：BSP_W25Qx_Read
*       函数功能：读取数据
*       形    参：pData：接收数据、ReadAddr:读取地址、Size：读取字节数
*       返 回 值：0:成功；-1：error
*******************************************************************************
*/
uint8_t W25Qx_Read(uint32_t ReadAddr, uint8_t* pData, uint32_t Size)
{
  uint8_t cmd[4];
  cmd[0] =READ_CMD;
  cmd[1] =(uint8_t)(ReadAddr >>16);
  cmd[2] =(uint8_t)(ReadAddr >>8);
  cmd[3] =(uint8_t)ReadAddr;
  W25Qxx_Wait_Busy();
  
  W25QXX_CS_LOW;
  HAL_SPI_Transmit(&W25QXX_HSPI,(uint8_t *)cmd,4,10000);
  if(HAL_OK != HAL_SPI_Receive(&W25QXX_HSPI,pData,Size,10000))
  {
      W25QXX_CS_HIGH; //+
      return -1;
  }
  
  W25QXX_CS_HIGH;
  return 0;
}

/*
*******************************************************************************
*       函    数: Erase_Sector
*       函数功能: 擦除指定扇区
*       形    参：sector_addr：扇区地址
*       返 回 值：0:成功；-1：error
*******************************************************************************
*/
uint8_t Erase_Sector(uint32_t sector_addr)
{
  uint8_t cmd[4];
  sector_addr *= 4096;
  cmd[0] =SECTOR_ERASE_CMD;
  cmd[1] =(uint8_t)(sector_addr >>16);
  cmd[2] =(uint8_t)(sector_addr >>8);
  cmd[3] =(uint8_t)sector_addr;
  
  W25Qxx_Write_Enable();
  W25Qxx_Wait_Busy();
  W25QXX_CS_LOW;
  if(HAL_OK != HAL_SPI_Transmit(&W25QXX_HSPI,cmd,4,1000))
  {
      W25QXX_CS_HIGH; //+
      return -1;
  }
  W25QXX_CS_HIGH;
  W25Qxx_Wait_Busy();
  
  return 0;
}

// Flash参数
#define SECTOR_SIZE             4096  // 4KB扇区
#define PAGE_SIZE               256   // 256字节页

// 可重复写入函数（自动处理扇区擦除）
uint8_t W25Qxx_Save(uint32_t address, uint8_t *data, uint16_t length)
{
    uint32_t current_addr = address;
    uint16_t remaining = length;
    uint8_t buffer[PAGE_SIZE];

    while (remaining > 0) {
        // 计算当前页剩余空间
        uint16_t page_offset = current_addr % PAGE_SIZE;
        uint16_t write_size = (PAGE_SIZE - page_offset) < remaining ? (PAGE_SIZE - page_offset) : remaining;

        // 读取当前页数据到缓冲区
        // W25Qx_Read(buffer, current_addr - page_offset, PAGE_SIZE);
        W25Qx_Read(current_addr - page_offset, buffer, PAGE_SIZE);
        // 修改缓冲区数据
        memcpy(buffer + page_offset, data, write_size);

        // 擦除当前扇区
        Erase_Sector(current_addr - page_offset);
        // 写入整个页
        W25Qx_Write(current_addr - page_offset, buffer, PAGE_SIZE);
        // 更新指针和剩余长度
        current_addr += write_size;
        data += write_size;
        remaining -= write_size;
    }
    return 0;
}

/*
*************************************************************************************
*       函 数 名：W25xx_Demo
*       函数功能：W25驱动测试
*       形    参：无
*       返 回 值：无
*************************************************************************************
*/
void W25xx_Demo(void)
{
    uint8_t err=0, sta;
    uint16_t i =0;

    uint8_t Txbuff0[] ={0x01,0x02,0x03,0x04,0x05};
    uint8_t Txbuff1[] ={0x06,0x07,0x08,0x09,0x0A};
    uint8_t Txbuff2[] ={11,12,13,14,15};
    uint8_t Rxbuff[15] ={0,0,0,0,0};
    uint16_t device_id;
    device_id =W25Qxx_ReadID();
//   printf("device_id =%d\r\n",device_id);
  
    sprintf(DEBUG_TEM, "Flash ID:%X\r\n", device_id);
    DebugPrintStr(DEBUG_TEM);

    sta = W25Qxx_GetStatus();
    sprintf(DEBUG_TEM, "Status:%X\r\n", sta);
    DebugPrintStr(DEBUG_TEM);

    for(i=0; i<TEST_SIZE; i++)
    {
      TEST1[i] = (uint8_t)i;
    }
    
    err = W25Qxx_Save(0x00,TEST1,TEST_SIZE);
    if(err == 0)
    {
        DebugPrintStr("write ok\r\n");
    }else
    {
        DebugPrintStr("write error\r\n");
    }
    err = W25Qx_Read(0x00,TEST2,TEST_SIZE);
    if(err == 0)
    {
        DebugPrintStr("read ok\r\n");
    }else
    {
        DebugPrintStr("read error\r\n");
    }

    for(i=0; i<TEST_SIZE; i++)
    {
      if(TEST1[i] != TEST2[i])
      {
        DebugPrintStr("read error\r\n");
        break;
      }
    }

    // for(i =0; i<TEST_SIZE; i++)
    {
      DebugPrintStrHex(TEST2, TEST_SIZE);
    }

    DebugPrintStr("read finish\r\n");

}




