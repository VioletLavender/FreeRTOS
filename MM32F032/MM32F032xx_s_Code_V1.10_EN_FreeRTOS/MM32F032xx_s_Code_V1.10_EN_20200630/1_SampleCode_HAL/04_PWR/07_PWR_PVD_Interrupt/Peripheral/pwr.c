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
uint8_t PWR_PVDFlag = 0;


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
void PWR_Configure(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable PWR Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Clear EXTI Line16 Interrupt Flag */
    EXTI_ClearITPendingBit(EXTI_Line16);

    /* PVD Map To EXTI_Line16 */
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line    = EXTI_Line16;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Configure the PVD Level to 1.8 ~ 4.8V */
//    PWR_PVDLevelConfig(PWR_CR_PLS_1V8);
//    PWR_PVDLevelConfig(PWR_CR_PLS_2V1);
//    PWR_PVDLevelConfig(PWR_CR_PLS_2V4);
//    PWR_PVDLevelConfig(PWR_CR_PLS_2V7);
//    PWR_PVDLevelConfig(PWR_CR_PLS_3V0);
//    PWR_PVDLevelConfig(PWR_CR_PLS_3V3);
//    PWR_PVDLevelConfig(PWR_CR_PLS_3V6);
//    PWR_PVDLevelConfig(PWR_CR_PLS_3V9);
//    PWR_PVDLevelConfig(PWR_CR_PLS_4V2);
//    PWR_PVDLevelConfig(PWR_CR_PLS_4V5);
    PWR_PVDLevelConfig(PWR_CR_PLS_4V8);

    /* Enable PVD Output */
    PWR_PVDCmd(ENABLE);

    /* Enable the PVD Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel  = PVD_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void PVD_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line16) != RESET)
    {
        PWR_PVDFlag = 1;

        EXTI_ClearITPendingBit(EXTI_Line16);
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

