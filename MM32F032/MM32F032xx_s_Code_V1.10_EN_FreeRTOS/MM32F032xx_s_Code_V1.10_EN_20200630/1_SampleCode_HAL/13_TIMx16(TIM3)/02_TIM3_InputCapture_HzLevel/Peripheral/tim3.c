/******************************************************************************
 * @file    tim3.c
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
#define __TIM3_C__


/* Includes -----------------------------------------------------------------*/
#include "tim3.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/


/* Private variables --------------------------------------------------------*/
uint8_t  TIM3_CC1Flag   = 0;
uint32_t TIM3_CCR1Value = 0;
uint32_t TIM3_CCR2Value = 0;
uint32_t TIM3_Clock     = 10000;


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
void TIM3_Configure(void)
{
    GPIO_InitTypeDef        GPIO_InitStructure;
    NVIC_InitTypeDef        NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef       TIM_ICInitStructure;

    TIM_DeInit(TIM3);

    /* Enable TIM3 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,   ENABLE);

    /* Time Base configuration */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler             = (RCC_GetSysClockFreq() / TIM3_Clock -1);
    TIM_TimeBaseStructure.TIM_CounterMode           = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period                = 0xFFFF;
    TIM_TimeBaseStructure.TIM_ClockDivision         = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter     = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* TIM3 Channel 1 Input Capture Configuration */
    TIM_ICInitStructure.TIM_Channel     = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter    = 0x00;
    TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);

    /* Select The Valid Input */
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);

    /* Configuration In Master-Slave Reset Mode */
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);

    /* Clear TIM3 CC1 Interrupt Flag */
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);

    /* Enable TIM3 CC1 Interrupt */
    TIM_ITConfig(TIM3, TIM_IT_CC1,  ENABLE);

    /* Enable TIM3 */
    TIM_Cmd(TIM3, ENABLE);

    /* Enable GPIOA & SYSCFG Clock */
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA,   ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* Enable PA6 Alternate Function1 */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);  //---TIM3_CH1

    /* Config PA6(TIM3_CH1) For Input Capture Channel */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Enable TIM3 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
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
void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
    {
        TIM3_CC1Flag   = 1;

        TIM3_CCR1Value = TIM_GetCapture1(TIM3);
        TIM3_CCR2Value = TIM_GetCapture2(TIM3);
    }

    /* Clear TIM3 CC1 Interrupt Flag */
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void TIM3_IC_ReadFreq(void)
{
    float Freq = 0.0, Duty = 0.0;

    if(TIM3_CC1Flag == 1)
    {
        TIM3_CC1Flag = 0;

        Freq = (float)TIM3_Clock / TIM3_CCR1Value;

        Duty = (float)TIM3_CCR2Value * 100 / (float)TIM3_CCR1Value;

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

