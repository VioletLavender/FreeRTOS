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
#define FLASH_PAGE_NUMBER       64
#define FLASH_PAGE_SIZE         1024
#define FLASH_START_ADDRESS    ((uint32_t)0x8000000)


/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
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
void FLASH_ProgramEntry(void)
{
    uint32_t TestData = 0x12345678;

    uint32_t OffsetAddr = FLASH_START_ADDRESS + 63 * FLASH_PAGE_SIZE;

    /* Unlock The FLASH Program Erase Controller */
    FLASH_Unlock();

    /* Clear The Flash's All Pending Flags */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

    /* Erase The Flash Page */
    FLASH_ErasePage(OffsetAddr);

    /* Clear The Flash's Pending Flags */
    FLASH_ClearFlag(FLASH_FLAG_EOP);

    /* Program A Word At A Specified Address */
    FLASH_ProgramWord(OffsetAddr, TestData);

    /* Clear The Flash's Pending Flags */
    FLASH_ClearFlag(FLASH_FLAG_EOP);

    /* Lock The Flash Program Erase Controller */
    FLASH_Lock();

    /* Read Data From Flash And Compare Data Value */
    if(*(__IO uint32_t *)(OffsetAddr) == TestData)
    {
        printf("\r\n\r\nFlash program test passed: 0x%x\r\n", TestData);
    }
    else
    {
        printf("\r\n\r\nFlash program test failed: 0x%x\r\n", TestData);
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

