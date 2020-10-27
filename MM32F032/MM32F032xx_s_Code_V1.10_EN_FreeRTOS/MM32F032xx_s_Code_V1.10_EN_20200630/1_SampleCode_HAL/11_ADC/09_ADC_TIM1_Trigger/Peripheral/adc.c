/******************************************************************************
 * @file    adc.c
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
#define __ADC_C__


/* Includes -----------------------------------------------------------------*/
#include "adc.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/


/* Private variables --------------------------------------------------------*/
volatile uint8_t  ADC_TIM1_TriggerFlag = 0;
volatile uint16_t ADC_Buffer[30];


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
void ADC_DMA_Configure(void)
{
    DMA_InitTypeDef  DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStruct;

    /* Enable DMA1 Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* DMA1 Channel1 Configuration */
    DMA_DeInit(DMA1_Channel1);
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(ADC1->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr     = (u32)&ADC_Buffer;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize         = 30;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
    DMA_InitStructure.DMA_Auto_reload        = DMA_Auto_Reload_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    /* Enable DMA1 Transfer Complete Interrupt */
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

    /* Enable DMA1 Channel1 */
    DMA_Cmd(DMA1_Channel1, ENABLE);

    /* DMA1 Interrupt Configuration */
    NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void ADC_TIM1CC1_Configure(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef       TIM_OCInitStructure;

    /* Enable TIM1 Clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    /* TIM1 Base Configuration , 100ms */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler         = RCC_GetSysClockFreq() / 10000 - 1;
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period            = 1000 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    /* TIM1 OC Configuration */
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode          = TIM_OCMode_Timing;
    TIM_OCInitStructure.TIM_OutputState     = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse           = 1000 / 2 - 1;
    TIM_OCInitStructure.TIM_OCPolarity      = TIM_OCPolarity_High;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    /* Enable TIM1 Peripheral Preload Register On ARR*/
    TIM_ARRPreloadConfig(TIM1, ENABLE);

    /* Enables or disables the specified TIM peripheral*/
    TIM_Cmd(TIM1, ENABLE);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void ADC_ChannelConfigure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef  ADC_InitStructure;

    /* Enable GPIOA Clock */
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);

    /* Enable ADC1 Clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    /* Configure PA1 As Analog Input Pin */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* ADC1 Configuration */
    ADC_StructInit(&ADC_InitStructure);
    ADC_InitStructure.ADC_Resolution       = ADC_Resolution_12b;
    ADC_InitStructure.ADC_PRESCARE         = ADC_PCLK2_PRESCARE_16;
    ADC_InitStructure.ADC_Mode             = ADC_Mode_Continue;
    ADC_InitStructure.ADC_DataAlign        = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_Init(ADC1, &ADC_InitStructure);

    /* Enable The Selected ADC Channel And Configure Its Sample Time */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 0, ADC_Samctl_239_5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 0, ADC_Samctl_239_5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 0, ADC_Samctl_239_5);

    /* Configure The ADC1 External Trigger For Injected Channels Conversion */
    ADC_ExternalTrigInjectedConvConfig(ADC1, ADC1_ExternalTrigConv_T1_CC1);

    /* Enable The ADC1 Conversion Through External Trigger */
    ADC_ExternalTrigConvCmd(ADC1, ENABLE);

    /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void ADC_Configure(void)
{
    ADC_DMA_Configure();

    ADC_TIM1CC1_Configure();

    ADC_ChannelConfigure();
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void ADC_TIM1_TriggerEntry(void)
{
    uint8_t  i   = 0, j = 0;
    uint32_t Sum = 0;
    float    FilterValue[3], VoltageValue[3];

    if(ADC_TIM1_TriggerFlag)
    {
        ADC_TIM1_TriggerFlag = 0;

        /* ADC Filter */
        for(i = 0; i < 3; i++)
        {
            for(j = 0; j < 10; j++)
            {
                Sum += ADC_Buffer[i + j * 3];
            }

            FilterValue[i] = Sum / 10;
        }

        /* Print Every Channels Voltage */
        for(i = 0; i < 3; i++)
        {
            VoltageValue[i] = FilterValue[i] / 4095.0 * 3.3;

            printf("\r\nRV%d's Voltage : %0.1f", i+1, VoltageValue[i]);
        }

        printf("\r\n\r\n");
    }
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void DMA1_Channel1_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC1) != RESET)
    {
        ADC_TIM1_TriggerFlag = 1;

        /* Clear DMA TC1 Flag */
        DMA_ClearITPendingBit(DMA1_IT_TC1);
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/
