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
void FLASH_ReadProtect(void)
{
    FLASH_Status Status;

    /* Unlock The Flash Program Erase Controller */
    FLASH_Unlock();

    /* Erase The Flash Protect Bytes */
    FLASH_EraseProtect();

    /* Erase The Flash Option Bytes */
    FLASH_EraseOptionBytes();

    /* Program 0x7F80 At 0x1FFE0000 Address */
    Status = FLASH_ProgramOptionHalfWord(0x1FFE0000, 0x7F80);

    if(Status == FLASH_COMPLETE)
    {
        /* Program 0xFF00 At 0x1FFE0002 Address */
        Status = FLASH_ProgramOptionHalfWord(0x1FFE0002, 0xFF00);

        if(Status != FLASH_COMPLETE)
        {
            printf("\r\n\r\n0x1FFE0002 Write Error!!!");
        }
    }
    else
    {
        printf("\r\n\r\n0x1FFE0000 Write Error!!!");
    }

    /* Lock The Flash Program Erase Controller */
    FLASH_Lock();
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

