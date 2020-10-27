/******************************************************************************
 * @file    tim1.c
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
#define __TIM1_C__


/* Includes -----------------------------------------------------------------*/
#include "tim1.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
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
void TIM1_Configure(void)
{
    GPIO_InitTypeDef        GPIO_InitStructure;
    TIM_OCInitTypeDef       TIM_OCInitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    /* Compute the value to be set in ARR regiter to generate signal frequency at 17.57 Khz */
    u32 TimerPeriod = (RCC_GetSysClockFreq() / 17570 ) - 1;

    /* Compute CCR1 value to generate a duty cycle at 50% for channel 1 and 1N */
    u32 Channel1Pulse = (uint16_t) (((uint32_t) 500 * (TimerPeriod - 1)) / 1000);

    /* Compute CCR2 value to generate a duty cycle at 37.5%  for channel 2 and 2N */
    u32 Channel2Pulse = (uint16_t) (((uint32_t) 375 * (TimerPeriod - 1)) / 1000);

    /* Compute CCR3 value to generate a duty cycle at 25%  for channel 3 and 3N */
    u32 Channel3Pulse = (uint16_t) (((uint32_t) 250 * (TimerPeriod - 1)) / 1000);

    /* Compute CCR4 value to generate a duty cycle at 12.5%  for channel 4 */
    u32 Channel4Pulse = (uint16_t) (((uint32_t) 125 * (TimerPeriod - 1)) / 1000);

    /* TIM1 clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    /* Time Base configuration */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler         = 0;
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period            = TimerPeriod;
    TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    /* Channel 1, 2 and 3 Configuration in Timing mode */
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode          = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState     = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState    = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse           = Channel1Pulse;
    TIM_OCInitStructure.TIM_OCPolarity      = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity     = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState     = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState    = TIM_OCNIdleState_Reset;

    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);

    TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);

    TIM_OCInitStructure.TIM_Pulse = Channel4Pulse;
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);

    /* TIM1 counter enable */
    TIM_Cmd(TIM1, ENABLE);

    /* Main Output Enable */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    /* GPIOA and GPIOB clocks enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* Connect TIM pins to AF2 */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8,  GPIO_AF_2);   /* TIM1_CH1  */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_2);   /* TIM1_CH2  */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_2);   /* TIM1_CH3  */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_2);   /* TIM1_CH4  */

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_2);   /* TIM1_CH1N */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_2);   /* TIM1_CH2N */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_2);   /* TIM1_CH3N */

    /* GPIOA Configuration: Channel 1, 2 3 and 4 as alternate function push-pull */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* GPIOB Configuration: Channel 1N 2N and 3N as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

