/******************************************************************************
 * @file    tim14.c
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
#define __TIM14_C__


/* Includes -----------------------------------------------------------------*/
#include "tim14.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/


/* Private variables --------------------------------------------------------*/
volatile uint32_t TIM14_DelayTicks = 0;


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
void TIM14_Configure(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef        NVIC_InitStructure;

    TIM_DeInit(TIM14);

    /* Enable TIM14 Clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM14, ENABLE);

    /* Config TIM14 Every 1ms Generate Interrupt */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler             = (RCC_GetSysClockFreq() / 1000000 - 1);
    TIM_TimeBaseStructure.TIM_CounterMode           = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period                = (1000 - 1);
    TIM_TimeBaseStructure.TIM_ClockDivision         = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter     = 0;
    TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);

    /* Clear TIM14 Update Flag */
    TIM_ClearFlag(TIM14, TIM_FLAG_Update);

    /* Enable TIM14 Update Interrupt */
    TIM_ITConfig(TIM14, TIM_IT_Update, ENABLE);

    /* Enable TIM14 */
    TIM_Cmd(TIM14, ENABLE);

    /* Enable TIM14 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority  = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd  = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void TIM14_IRQHandler(void)
{
    if(TIM14_DelayTicks)
    {
        TIM14_DelayTicks--;
    }

    /* Clear TIM14 Update Interrupt Flag */
    TIM_ClearITPendingBit(TIM14, TIM_IT_Update);
}


/******************************************************************************
 * @brief       delay milliseconds
 * @param       ms : specifies the delay time length, in milliseconds.
 * @retval      
 * @attention   
******************************************************************************/
void TIM14_DelayMs(uint32_t ms)
{
    TIM14_DelayTicks = ms;
    while(TIM14_DelayTicks);
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

