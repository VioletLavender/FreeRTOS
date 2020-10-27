/******************************************************************************
 * @file    pwr.c
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
#define __PWR_C__


/* Includes -----------------------------------------------------------------*/
#include "pwr.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/


/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/


/******************************************************************************
 * @brief       Wake up from stop mode by RTC
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void PWR_Configure(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
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

    /* Clear EXTI Line17 Flag */
    EXTI_ClearITPendingBit(EXTI_Line17);

    /* Configure EXTI Line17(RTC Alarm) To Generate An Interrupt On Rising Edge */
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line    = EXTI_Line17;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Config RTC NVIC */
    NVIC_InitStructure.NVIC_IRQChannel  = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
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
        /* Clear EXTI Line17 Flag */
        EXTI_ClearITPendingBit(EXTI_Line17);

        /* Check If The Wake-Up Flag Is Set */
        if(PWR_GetFlagStatus(PWR_FLAG_WU) != RESET)
        {
            /* Clear Wake Up Flag */
            PWR_ClearFlag(PWR_FLAG_WU);
        }

        /* Wait Until Last Write Operation On RTC REG Has Finished */
        RTC_WaitForLastTask();

        /* Clear Alarm Flag */
        RTC_ClearITPendingBit(RTC_IT_ALR);
        /* Wait Until Last Write Operation On RTC REG Has Finished */
        RTC_WaitForLastTask();
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

