/******************************************************************************
 * @file    flash.c
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
#define __FLASH_C__


/* Includes -----------------------------------------------------------------*/
#include "flash.h"


/* Private typedef ----------------------------------------------------------*/


/* Private define -----------------------------------------------------------*/
#define FLASH_PAGE_NUMBER               64
#define FLASH_PAGE_SIZE                 1024
#define FLASH_START_ADDRESS             ((uint32_t)0x8000000)

#define SIMULATE_EEPROM_START_PAGE      62
#define SIMULATE_EEPROM_START_ADDRESS   ((uint32_t)SIMULATE_EEPROM_START_PAGE * \
                                        FLASH_PAGE_SIZE + FLASH_START_ADDRESS)


/* Private macro ------------------------------------------------------------*/


/* Private variables --------------------------------------------------------*/
uint8_t FLASH_SimulateEEPROM_Buffer[FLASH_PAGE_SIZE];


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
void FLASH_SimulateEEPROM_ReadBytes(uint16_t Addr, uint8_t *pBuf, uint16_t Len)
{
    uint16_t i = 0;

    for(i = 0; i < Len; i++)
    {
        pBuf[i] = (*(__IO uint8_t *)(SIMULATE_EEPROM_START_ADDRESS + Addr + i));
    }
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void FLASH_SimulateEEPROM_WriteBytes(uint16_t Addr, uint8_t *pBuf, uint16_t Len)
{
    uint16_t i = 0, HalfWord = 0;

    /* Read All Data To Simulate EEPROM Buffer */
    FLASH_SimulateEEPROM_ReadBytes(0, FLASH_SimulateEEPROM_Buffer, FLASH_PAGE_SIZE);

    /* Modify Simulate EEPROM Buffer Data */
    for(i = 0; i < Len; i++)
    {
        FLASH_SimulateEEPROM_Buffer[Addr + i] = pBuf[i];
    }

    /* Unlock The FLASH Program Erase Controller */
    FLASH_Unlock();

    /* Clear The Flash's Pending Flags */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

    /* Erase Flash Page */
    FLASH_ErasePage(SIMULATE_EEPROM_START_ADDRESS);

    /* Clear The Flash's Pending Flags */
    FLASH_ClearFlag(FLASH_FLAG_EOP);

    /* Write Simulate EEPROM Buffer To Flash */
    for(i = 0; i < FLASH_PAGE_SIZE; i += 2)
    {
        HalfWord = (uint16_t)FLASH_SimulateEEPROM_Buffer[i + 1] << 8 | FLASH_SimulateEEPROM_Buffer[i];

        /* Program A Word At A Specified Address */
        FLASH_ProgramHalfWord(SIMULATE_EEPROM_START_ADDRESS + i,  HalfWord);

        /* Clear The Flash's Pending Flags */
        FLASH_ClearFlag(FLASH_FLAG_EOP);
    }

    /* Lock The Flash Program Erase Controller */
    FLASH_Lock();
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void FLASH_SimulateEEPROM_Entry(void)
{
    uint8_t WriteBuffer[10] =
    {
        0x01, 0x12, 0x23, 0x34, 0x45,
        0x56, 0x67, 0x78, 0x89, 0x90
    };

    uint8_t ReadBuffer[10], i = 0;

    memset(ReadBuffer, 0, sizeof(ReadBuffer));

    printf("\r\n\r\nWrite Data To Flash : ");

    /* Write data to flash start at 0 offset address */
    FLASH_SimulateEEPROM_WriteBytes(0, WriteBuffer, 10);

    printf("OK");

    /* Read data from flash */
    FLASH_SimulateEEPROM_ReadBytes(0, ReadBuffer, 10);

    printf("\r\n\r\nRead Data From Flash : \r\n");

    for(i = 0; i < 10; i++)
    {
        printf("0x%02x ", ReadBuffer[i]);
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

