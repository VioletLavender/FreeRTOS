/******************************************************************************
 * @file    rtc.c
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
#define __RTC_C__


/* Includes -----------------------------------------------------------------*/
#include "rtc.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/


/* Private variables --------------------------------------------------------*/
uint8_t RTC_AlarmFlag = 0;


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
void RTC_Configure(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable PWR Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Enable WKUP pin */
    PWR_WakeUpPinCmd(ENABLE);

    /* Enable Access To The RTC & Backup Registers */
    PWR_BackupAccessCmd(ENABLE);

    if(BKP_ReadBackupRegister(BKP_DR1) != 0x5A5A)
    {
        BKP_DeInit();

        /* Enable LSI Clock Source */
        RCC_LSICmd(ENABLE);

        /* Wait LSI Clock Source Ready */
        while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

        /* Config RTC Clock Source : LSI */
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

        /* Enable RTC Clock */
        RCC_RTCCLKCmd(ENABLE);

        /* Wait For Synchronization */
        RTC_WaitForSynchro();
        /* Wait Until Last Write Operation On RTC REG Has Finished */
        RTC_WaitForLastTask();

        /* Set The RTC Prescaler Value */
        RTC_SetPrescaler(32767);
        /* Wait Until Last Write Operation On RTC REG Has Finished */
        RTC_WaitForLastTask();

        /* Enable RTC Alarm Interrupt */
        RTC_ITConfig(RTC_IT_ALR, ENABLE);
        /* Wait Until Last Write Operation On RTC REG Has Finished */
        RTC_WaitForLastTask();

        /* Exit From The RTC Configuration Mode */
        RTC_ExitConfigMode();

        BKP_WriteBackupRegister(BKP_DR1, 0x5A5A);
        /* Wait Until Last Write Operation On RTC REG Has Finished */
        RTC_WaitForLastTask();
    }
    else
    {
        /* Wait For Synchronization */
        RTC_WaitForSynchro();

        /* Enable RTC Alarm Interrupt */
        RTC_ITConfig(RTC_IT_ALR, ENABLE);
        /* Wait Until Last Write Operation On RTC REG Has Finished */
        RTC_WaitForLastTask();
    }

    /* Config RTC NVIC */
    NVIC_InitStructure.NVIC_IRQChannel  = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Set The RTC Alarm Value */
    RTC_SetAlarm(RTC_GetCounter() + 3);
    /* Wait Until Last Write Operation On RTC REG Has Finished */
    RTC_WaitForLastTask();
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void RTC_BKP_IRQHandler(void)
{
    if(RTC_GetITStatus(RTC_IT_ALR) != RESET)
    {
        RTC_AlarmFlag = 1;

        /* Clear Alarm Flag */
        RTC_ClearITPendingBit(RTC_IT_ALR);
        /* Wait Until Last Write Operation On RTC REG Has Finished */
        RTC_WaitForLastTask();
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

