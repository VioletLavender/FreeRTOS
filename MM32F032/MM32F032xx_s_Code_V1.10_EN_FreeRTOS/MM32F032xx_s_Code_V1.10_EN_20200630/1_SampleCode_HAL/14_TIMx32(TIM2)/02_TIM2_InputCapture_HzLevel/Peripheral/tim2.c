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
uint8_t  TIM2_CC1Flag   = 0;
uint32_t TIM2_CCR1Value = 0;
uint32_t TIM2_CCR2Value = 0;
uint32_t TIM2_Clock     = 1000000;


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
    GPIO_InitTypeDef        GPIO_InitStructure;
    NVIC_InitTypeDef        NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef       TIM_ICInitStructure;

    TIM_DeInit(TIM2);

    /* Enable TIM2 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,   ENABLE);

    /* Time Base configuration */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler             = (RCC_GetSysClockFreq() / TIM2_Clock - 1);
    TIM_TimeBaseStructure.TIM_CounterMode           = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period                = 0xFFFFFFFF;
    TIM_TimeBaseStructure.TIM_ClockDivision         = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter     = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* TIM2 Channel 1 Input Capture Configuration */
    TIM_ICInitStructure.TIM_Channel     = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter    = 0x00;
    TIM_PWMIConfig(TIM2, &TIM_ICInitStructure);

    /* Select The Valid Input */
    TIM_SelectInputTrigger(TIM2, TIM_TS_TI1FP1);

    /* Configuration In Master-Slave Reset Mode */
    TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);

    /* Clear TIM2 CC1 Interrupt Flag */
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

    /* Enable TIM2 CC1 Interrupt */
    TIM_ITConfig(TIM2, TIM_IT_CC1,  ENABLE);

    /* Enable TIM2 */
    TIM_Cmd(TIM2, ENABLE);

    /* Enable GPIOA & SYSCFG Clock */
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA,   ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* Enable PA5 Alternate Function2 */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_2);  //---TIM2_CH1

    /* Config PA5(TIM2_CH1) For Input Capture Channel */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

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
    if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
    {
        TIM2_CC1Flag   = 1;

        TIM2_CCR1Value = TIM_GetCapture1(TIM2);
        TIM2_CCR2Value = TIM_GetCapture2(TIM2);
    }

    /* Clear TIM2 CC1 Interrupt Flag */
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void TIM2_IC_ReadFreq(void)
{
    float Freq = 0.0, Duty = 0.0;

    if(TIM2_CC1Flag == 1)
    {
        TIM2_CC1Flag = 0;

        Freq = (float)TIM2_Clock / TIM2_CCR1Value;

        Duty = (float)TIM2_CCR2Value * 100 / (float)TIM2_CCR1Value;

        if(Freq >= 1000000.0)
        {
            printf("\r\nFrequency = %0.1fMHz, Duty = %0.1f%c", Freq / 1000000.0, Duty, '%');
        }
        else if(Freq >= 1000.0)
        {
            printf("\r\nFrequency = %0.1fkHz, Duty = %0.1f%c", Freq / 1000.0, Duty, '%');
        }
        else
        {
            printf("\r\nFrequency = %0.1fHz, Duty = %0.1f%c", Freq, Duty, '%');
        }
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

