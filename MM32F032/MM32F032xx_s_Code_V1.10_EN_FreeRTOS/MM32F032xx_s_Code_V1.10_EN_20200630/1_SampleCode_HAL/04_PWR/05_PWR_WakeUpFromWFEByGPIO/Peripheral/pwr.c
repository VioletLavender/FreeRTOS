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
 * @brief       Wake up from WFE by GPIO
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void PWR_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    /* Enable GPIOB & SYSCFG Clock */
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOB,   ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* PB2 Use EXTI Line2 */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource2);

    /* EXTI line2 configuration */
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line    = EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Clear EXTI line2 interrupt flag */
    EXTI_ClearITPendingBit(EXTI_Line2);

    /* PB2(KEY2) Wakeup */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Config EXTI line2 interrupt and enable */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelCmd    = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void EXTI2_3_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line2) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line2);
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

