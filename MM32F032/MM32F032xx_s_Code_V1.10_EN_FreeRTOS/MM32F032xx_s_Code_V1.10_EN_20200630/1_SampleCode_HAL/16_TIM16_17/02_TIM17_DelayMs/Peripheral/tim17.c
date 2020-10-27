/******************************************************************************
 * @file    tim17.c
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
#define __TIM17_C__


/* Includes -----------------------------------------------------------------*/
#include "tim17.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/


/* Private variables --------------------------------------------------------*/
volatile uint32_t TIM17_DelayTicks = 0;


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
void TIM17_Configure(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef        NVIC_InitStructure;

    TIM_DeInit(TIM17);

    /* Enable TIM17 Clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);

    /* Config TIM17 Every 1ms Generate Interrupt */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler             = (RCC_GetSysClockFreq() / 1000000 - 1);
    TIM_TimeBaseStructure.TIM_CounterMode           = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period                = (1000 - 1);
    TIM_TimeBaseStructure.TIM_ClockDivision         = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter     = 0;
    TIM_TimeBaseInit(TIM17, &TIM_TimeBaseStructure);

    /* Clear TIM17 Update Flag */
    TIM_ClearFlag(TIM17, TIM_FLAG_Update);

    /* Enable TIM17 Update Interrupt */
    TIM_ITConfig(TIM17, TIM_IT_Update, ENABLE);

    /* Enable TIM17 */
    TIM_Cmd(TIM17, ENABLE);

    /* Enable TIM17 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM17_IRQn;
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
void TIM17_IRQHandler(void)
{
    if(TIM17_DelayTicks)
    {
        TIM17_DelayTicks--;
    }

    /* Clear TIM17 Update Interrupt Flag */
    TIM_ClearITPendingBit(TIM17, TIM_IT_Update);
}


/******************************************************************************
 * @brief       delay milliseconds
 * @param       ms : specifies the delay time length, in milliseconds.
 * @retval      
 * @attention   
******************************************************************************/
void TIM17_DelayMs(uint32_t ms)
{
    TIM17_DelayTicks = ms;
    while(TIM17_DelayTicks);
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

