/******************************************************************************
 * @file    tim2.c
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
#define __TIM2_C__


/* Includes -----------------------------------------------------------------*/
#include "tim2.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/


/* Private variables --------------------------------------------------------*/
volatile uint32_t TIM2_DelayTicks = 0;


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
void TIM2_Configure(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef        NVIC_InitStructure;

    TIM_DeInit(TIM2);

    /* Enable TIM2 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* Config TIM2 Every 1ms Generate Interrupt */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler             = (RCC_GetSysClockFreq() / 1000000 - 1);
    TIM_TimeBaseStructure.TIM_CounterMode           = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period                = (1000 - 1);
    TIM_TimeBaseStructure.TIM_ClockDivision         = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter     = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* Clear TIM2 Update Flag */
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    /* Enable TIM2 Update Interrupt */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    /* Enable TIM2 */
    TIM_Cmd(TIM2, ENABLE);

    /* Enable TIM2 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
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
void TIM2_IRQHandler(void)
{
    if(TIM2_DelayTicks)
    {
        TIM2_DelayTicks--;
    }

    /* Clear TIM2 Update Interrupt Flag */
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}


/******************************************************************************
 * @brief       delay milliseconds
 * @param       ms : specifies the delay time length, in milliseconds.
 * @retval      
 * @attention   
******************************************************************************/
void TIM2_DelayMs(uint32_t ms)
{
    TIM2_DelayTicks = ms;
    while(TIM2_DelayTicks);
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

