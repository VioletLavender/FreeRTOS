/******************************************************************************
 * @file    tim16.c
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
#define __TIM16_C__


/* Includes -----------------------------------------------------------------*/
#include "tim16.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/


/* Private variables --------------------------------------------------------*/
volatile uint32_t TIM16_DelayTicks = 0;


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
void TIM16_Configure(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef        NVIC_InitStructure;

    TIM_DeInit(TIM16);

    /* Enable TIM16 Clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);

    /* Config TIM16 Every 1ms Generate Interrupt */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler             = (RCC_GetSysClockFreq() / 1000000 - 1);
    TIM_TimeBaseStructure.TIM_CounterMode           = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period                = (1000 - 1);
    TIM_TimeBaseStructure.TIM_ClockDivision         = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter     = 0;
    TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);

    /* Clear TIM16 Update Flag */
    TIM_ClearFlag(TIM16, TIM_FLAG_Update);

    /* Enable TIM16 Update Interrupt */
    TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE);

    /* Enable TIM16 */
    TIM_Cmd(TIM16, ENABLE);

    /* Enable TIM16 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM16_IRQn;
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
void TIM16_IRQHandler(void)
{
    if(TIM16_DelayTicks)
    {
        TIM16_DelayTicks--;
    }

    /* Clear TIM16 Update Interrupt Flag */
    TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
}


/******************************************************************************
 * @brief       delay milliseconds
 * @param       ms : specifies the delay time length, in milliseconds.
 * @retval      
 * @attention   
******************************************************************************/
void TIM16_DelayMs(uint32_t ms)
{
    TIM16_DelayTicks = ms;
    while(TIM16_DelayTicks);
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

