/******************************************************************************
 * @file    bsp_spi_flash.c
 * @author  King
 * @version V1.00
 * @date    20-May-2020
 * @brief   ......
 ******************************************************************************
 * @attention
 * 
 * THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
 * CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
 * TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
 * HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
 * CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
 * <H2><CENTER>&COPY; COPYRIGHT 2020 MINDMOTION </CENTER></H2>
******************************************************************************/


/* Define to prevent recursive inclusion ------------------------------------*/
#define __BSP_SPI_FLASH_C__


/* Includes -----------------------------------------------------------------*/
#include "spi.h"
#include "bsp_spi_flash.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/


/* Private variables --------------------------------------------------------*/
uint8_t SPI_FLASH_Buffer[SPI_FLASH_SECTOR_SIZE];


/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/


/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_SPI_FLASH_Init(void)
{
    /* Set SPI Flash CS Pin High Level */
    BSP_SPI_FLASH_CS_H();

    /* SPI Configuration */
    SPIx_Configure(SPI2);

    /* Enable SPI Flash TX And RX */
    BSP_SPI_FLASH_TXEN();
    BSP_SPI_FLASH_RXEN();
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
static void BSP_SPI_FLASH_RxBurst(uint8_t *pBuf, uint16_t Len)
{
    if(Len)
    {
        SPIx_DMA_RxData(SPI2, pBuf, Len);
    }
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
static void BSP_SPI_FLASH_TxBurst(uint8_t *pBuf, uint16_t Len)
{
    if(Len)
    {
        SPIx_DMA_TxData(SPI2, pBuf, Len);
    }
}


/******************************************************************************
 * @brief       Read Status Register-1
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
uint8_t BSP_SPI_FLASH_ReadSR1(void)
{
    uint8_t Status = 0, pCmd[2] = {0x05, 0xFF};

    BSP_SPI_FLASH_CS_L();

    BSP_SPI_FLASH_TxBurst(pCmd,    2);
    BSP_SPI_FLASH_RxBurst(&Status, 1);

    BSP_SPI_FLASH_CS_H();

    return Status;
}


/******************************************************************************
 * @brief       Write Status Register-1
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_SPI_FLASH_WriteSR1(uint8_t Status)
{
    uint8_t pCmd[2] = {0x01, 0xFF};

    pCmd[1] = Status;

    BSP_SPI_FLASH_CS_L();

    BSP_SPI_FLASH_TxBurst(pCmd, 2);

    BSP_SPI_FLASH_CS_H();
}


/******************************************************************************
 * @brief       SPI Flash Write Disable
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_SPI_FLASH_WriteDisable(void)
{
    uint8_t Cmd = 0x04;

    BSP_SPI_FLASH_CS_L();

    BSP_SPI_FLASH_TxBurst(&Cmd, 1);

    BSP_SPI_FLASH_CS_H();
}


/******************************************************************************
 * @brief       SPI Flash Write Enable
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_SPI_FLASH_WriteEnable(void)
{
    uint8_t Cmd = 0x06;

    BSP_SPI_FLASH_CS_L();

    BSP_SPI_FLASH_TxBurst(&Cmd, 1);

    BSP_SPI_FLASH_CS_H();
}


/******************************************************************************
 * @brief       Read SPI FLASH Manufacturer / Device ID
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_SPI_FLASH_ReadDeviceID(void)
{
    uint8_t pCmd[4] = {0x90, 0xFF, 0xFF, 0x00};
    uint8_t pBuf[2];

    uint16_t DeviceID = 0;

    BSP_SPI_FLASH_CS_L();

    BSP_SPI_FLASH_TxBurst(pCmd, 4);
    BSP_SPI_FLASH_RxBurst(pBuf, 2);

    BSP_SPI_FLASH_CS_H();

    DeviceID   = pBuf[0];
    DeviceID <<= 8;
    DeviceID  |= pBuf[1];

    printf("\r\n\r\nSPI Flash DeviceID : 0x%04x", DeviceID);
}


/******************************************************************************
 * @brief       Read SPI FLASH JEDEC ID
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_SPI_FLASH_ReadJEDEC_ID(void)
{
    uint8_t Cmd = 0x9F;
    uint8_t pBuf[3];

    uint32_t JEDEC_ID = 0;

    BSP_SPI_FLASH_CS_L();

    BSP_SPI_FLASH_TxBurst(&Cmd, 1);
    BSP_SPI_FLASH_RxBurst(pBuf, 3);

    BSP_SPI_FLASH_CS_H();

    JEDEC_ID   = pBuf[0];
    JEDEC_ID <<= 8;
    JEDEC_ID  |= pBuf[1];
    JEDEC_ID <<= 8;
    JEDEC_ID  |= pBuf[2];

    printf("\r\n\r\nSPI Flash JEDEC ID : 0x%06x", JEDEC_ID);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_SPI_FLASH_WaitBusy(void)
{
    uint32_t Timeout = 0;

    while((BSP_SPI_FLASH_ReadSR1() & 0x01) == 0x01)
    {
        if(Timeout++ > 0xFFFF)
        {
            break;
        }
    }
}


/******************************************************************************
 * @brief       Power-down
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_SPI_FLASH_PowerDown(void)
{
    uint8_t i = 0, Cmd = 0xB9;

    BSP_SPI_FLASH_CS_L();

    BSP_SPI_FLASH_TxBurst(&Cmd, 1);

    BSP_SPI_FLASH_CS_H();

    for(i = 0; i < 200; i++);
}


/******************************************************************************
 * @brief       Release Power-down
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_SPI_FLASH_WakeUp(void)
{
    uint8_t i = 0, Cmd = 0xAB;

    BSP_SPI_FLASH_CS_L();

    BSP_SPI_FLASH_TxBurst(&Cmd, 1);

    BSP_SPI_FLASH_CS_H();

    for(i = 0; i < 200; i++);
}


/******************************************************************************
 * @brief       Chip Erase
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_SPI_FLASH_ChipErase(void)
{
    uint8_t Cmd = 0xC7;

    BSP_SPI_FLASH_WriteEnable();
    BSP_SPI_FLASH_WaitBusy();

    BSP_SPI_FLASH_CS_L();

    BSP_SPI_FLASH_TxBurst(&Cmd, 1);

    BSP_SPI_FLASH_CS_H();

    BSP_SPI_FLASH_WaitBusy();
}


/******************************************************************************
 * @brief       Sector Erase 4KB
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_SPI_FLASH_SectorErase(uint16_t Sector)
{
    uint8_t  pCmd[4] = {0x20, 0x00, 0x00, 0x00};
    uint32_t Addr    = Sector * 4 * 1024;

    pCmd[1] = (uint8_t)((Addr >> 16) & 0x000000FF);
    pCmd[2] = (uint8_t)((Addr >>  8) & 0x000000FF);
    pCmd[3] = (uint8_t)((Addr >>  0) & 0x000000FF);

    BSP_SPI_FLASH_WriteEnable();
    BSP_SPI_FLASH_WaitBusy();

    BSP_SPI_FLASH_CS_L();

    BSP_SPI_FLASH_TxBurst(pCmd, 4);

    BSP_SPI_FLASH_CS_H();

    BSP_SPI_FLASH_WaitBusy();
}


/******************************************************************************
 * @brief       Blobk Erase 32KB
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_SPI_FLASH_BlockErase32K(uint16_t Block)
{
    uint8_t  pCmd[4] = {0x52, 0x00, 0x00, 0x00};
    uint32_t Addr    = Block * 32 * 1024;

    pCmd[1] = (uint8_t)((Addr >> 16) & 0x000000FF);
    pCmd[2] = (uint8_t)((Addr >>  8) & 0x000000FF);
    pCmd[3] = (uint8_t)((Addr >>  0) & 0x000000FF);

    BSP_SPI_FLASH_WriteEnable();
    BSP_SPI_FLASH_WaitBusy();

    BSP_SPI_FLASH_CS_L();

    BSP_SPI_FLASH_TxBurst(pCmd, 4);

    BSP_SPI_FLASH_CS_H();

    BSP_SPI_FLASH_WaitBusy();
}


/******************************************************************************
 * @brief       Block Erase 64KB
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_SPI_FLASH_BlockErase64K(uint16_t Block)
{
    uint8_t  pCmd[4] = {0xD8, 0x00, 0x00, 0x00};
    uint32_t Addr    = Block * 64 * 1024;

    pCmd[1] = (uint8_t)((Addr >> 16) & 0x000000FF);
    pCmd[2] = (uint8_t)((Addr >>  8) & 0x000000FF);
    pCmd[3] = (uint8_t)((Addr >>  0) & 0x000000FF);

    BSP_SPI_FLASH_WriteEnable();
    BSP_SPI_FLASH_WaitBusy();

    BSP_SPI_FLASH_CS_L();

    BSP_SPI_FLASH_TxBurst(pCmd, 4);

    BSP_SPI_FLASH_CS_H();

    BSP_SPI_FLASH_WaitBusy();
}


/******************************************************************************
 * @brief       Read Data
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_SPI_FLASH_ReadData(uint32_t Addr, uint8_t *pBuf, uint32_t Num)
{
    uint8_t pCmd[4] = {0x03, 0x00, 0x00, 0x00};

    pCmd[1] = (uint8_t)((Addr >> 16) & 0x000000FF);
    pCmd[2] = (uint8_t)((Addr >>  8) & 0x000000FF);
    pCmd[3] = (uint8_t)((Addr >>  0) & 0x000000FF);

    BSP_SPI_FLASH_CS_L();

    BSP_SPI_FLASH_TxBurst(pCmd, 4);
    BSP_SPI_FLASH_RxBurst(pBuf, Num);

    BSP_SPI_FLASH_CS_H();
}


/******************************************************************************
 * @brief       Fast Read
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_SPI_FLASH_FastRead(uint32_t Addr, uint8_t *pBuf, uint32_t Num)
{
    uint8_t pCmd[5] = {0x0B, 0x00, 0x00, 0x00, 0xFF};

    pCmd[1] = (uint8_t)((Addr >> 16) & 0x000000FF);
    pCmd[2] = (uint8_t)((Addr >>  8) & 0x000000FF);
    pCmd[3] = (uint8_t)((Addr >>  0) & 0x000000FF);

    BSP_SPI_FLASH_CS_L();

    BSP_SPI_FLASH_TxBurst(pCmd, 5);
    BSP_SPI_FLASH_RxBurst(pBuf, Num);

    BSP_SPI_FLASH_CS_H();
}


/******************************************************************************
 * @brief       Page Program
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_SPI_FLASH_PageProgram(uint32_t Addr, uint8_t *pBuf, uint32_t Num)
{
    uint8_t pCmd[4] = {0x02, 0x00, 0x00, 0x00};

    pCmd[1] = (uint8_t)((Addr >> 16) & 0x000000FF);
    pCmd[2] = (uint8_t)((Addr >>  8) & 0x000000FF);
    pCmd[3] = (uint8_t)((Addr >>  0) & 0x000000FF);

    BSP_SPI_FLASH_WriteEnable();

    BSP_SPI_FLASH_CS_L();

    BSP_SPI_FLASH_TxBurst(pCmd, 4);
    BSP_SPI_FLASH_TxBurst(pBuf, Num);

    BSP_SPI_FLASH_CS_H();

    BSP_SPI_FLASH_WaitBusy();
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_SPI_FLASH_WriteWithoutCheck(uint32_t Addr, uint8_t *pBuf, uint32_t Num)
{
    uint16_t PageRemain = 256 - (Addr % 256);

    if(Num <= PageRemain)
    {
        PageRemain = Num;
    }

    while(1)
    {
        BSP_SPI_FLASH_PageProgram(Addr, pBuf, PageRemain);

        if(PageRemain == Num)
        {
            break;
        }
        else
        {
            pBuf += PageRemain;
            Addr += PageRemain;
            Num  -= PageRemain;

            if(Num > SPI_FLASH_PAGE_SIZE)
            {
                PageRemain = SPI_FLASH_PAGE_SIZE;
            }
            else
            {
                PageRemain = Num;
            }
        }
    }
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_SPI_FLASH_WriteWithCheck(uint32_t Addr, uint8_t *pBuf, uint32_t Num)
{
    uint32_t SectorIndex;
    uint16_t SectorOffset;
    uint16_t SectorRemain;

    uint16_t i;

    SectorIndex  = Addr / SPI_FLASH_SECTOR_SIZE;
    SectorOffset = Addr % SPI_FLASH_SECTOR_SIZE;
    SectorRemain = SPI_FLASH_SECTOR_SIZE - SectorOffset;

    if(Num <= SectorRemain)
    {
        SectorRemain = Num;
    }

    while(1)
    {
        memset(SPI_FLASH_Buffer, 0, sizeof(SPI_FLASH_Buffer));

        /* Read Sector Data To SPI_FLASH_Buffer */
        BSP_SPI_FLASH_FastRead(SectorIndex * SPI_FLASH_SECTOR_SIZE, SPI_FLASH_Buffer, SPI_FLASH_SECTOR_SIZE);

        /* Check Data Is All 0xFF, If Not Then Need Erase Sector */
        for(i = 0; i < SPI_FLASH_SECTOR_SIZE; i++)
        {
            if(SPI_FLASH_Buffer[i] != 0xFF)
            {
                break;
            }
        }

        if(i < SPI_FLASH_SECTOR_SIZE)
        {
            BSP_SPI_FLASH_SectorErase(SectorIndex); /* Erase */

            for(i = 0; i < SectorRemain; i++)   /* Copy  */
            {
                SPI_FLASH_Buffer[i + SectorOffset] = pBuf[i];
            }

            BSP_SPI_FLASH_WriteWithoutCheck(Addr, SPI_FLASH_Buffer, SectorRemain);
        }
        else
        {
            BSP_SPI_FLASH_WriteWithoutCheck(Addr, pBuf, SectorRemain);
        }

        if(Num == SectorRemain)
        {
            break;
        }
        else
        {
            SectorIndex += 1;
            SectorOffset = 0;

            pBuf += SectorRemain;
            Addr += SectorRemain;
            Num  -= SectorRemain;

            if(Num > 4096)
            {
                SectorRemain = 4096;
            }
            else
            {
                SectorRemain = Num;
            }
        }
    }
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_SPI_FLASH_Entry(void)
{
    uint8_t i = 0, WriteBuffer[100], ReadBuffer[100];

    /*Read Device ID------------------------------------*/
    BSP_SPI_FLASH_ReadDeviceID();

    /*Read JEDEC ID-------------------------------------*/
    BSP_SPI_FLASH_ReadJEDEC_ID();

    /*Write Data----------------------------------------*/
    printf("\r\n\r\nWrite Data : ");

    memset(WriteBuffer, 0, sizeof(WriteBuffer));

    for(i = 0; i < sizeof(WriteBuffer); i++)
    {
        WriteBuffer[i] = i;
    }

    BSP_SPI_FLASH_WriteWithCheck(0, WriteBuffer, sizeof(WriteBuffer));

    printf("OK");

    /*Read Data-----------------------------------------*/
    printf("\r\n\r\nRead Data : \r\n");

    memset(ReadBuffer, 0, sizeof(ReadBuffer));

    BSP_SPI_FLASH_ReadData(0, ReadBuffer, sizeof(ReadBuffer));

    for(i = 0; i < sizeof(ReadBuffer); i++)
    {
        printf("0x%02x ", ReadBuffer[i]);

        if(((i + 1) % 10) == 0)
        {
            printf("\r\n");     /* new line */
        }
    }

    printf("\r\n\r\n");
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

