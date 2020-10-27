/******************************************************************************
 * @file    bkp.c
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
#define __BKP_C__


/* Includes -----------------------------------------------------------------*/
#include "bkp.h"


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
void BKP_BackupDataEntry(void)
{
    uint16_t ReadData = 0, CheckData = 0x1234;

    /* Enable PWR & BKP Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Enables Access To The RTC And Backup Registers */
    PWR_BackupAccessCmd(ENABLE);

    /* Reads Data From The Specified Data Backup Register, BKP_DR1~BKP_DR20 */
    ReadData = BKP_ReadBackupRegister(BKP_DR1);

    if(ReadData == CheckData)
    {
        printf("\r\nSpecified Data Backup Register : 0x%04x\r\n", ReadData);
    }
    else
    {
        /* Writes User Data To The Specified Data Backup Register */
        BKP_WriteBackupRegister(BKP_DR1, CheckData);

        printf("\r\nMCU Will Be Software Reset and Check Specified Data Backup Register Was Stored ?\r\n");

        /* Software reset */
        NVIC_SystemReset();
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

