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
volatile uint8_t ADC_EXTI_TriggerFlag = 0;


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
void ADC_EXTI_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIOA & GPIOB Clock */
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA,   ENABLE);

    /* Enable SYSCFG Clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource11);

    /* Configure PB11(KEY4) As EXTI Trigger Source */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void ADC_ChannelConfigure(void)
{
    ADC_InitTypeDef  ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStruct;

    /* Enable ADC1 Clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

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

    /* Configure The ADC1 External Trigger For Injected Channels Conversion */
    ADC_ExternalTrigInjectedConvConfig(ADC1, ADC1_ExternalTrigConv_EXTI_11);

    /* Enable The ADC1 Conversion Through External Trigger */
    ADC_ExternalTrigConvCmd(ADC1, ENABLE);

    /* Enable ADC1 EOC Interrupt */
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    /* Enable GPIOA & GPIOB Clock */
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);

    /* Configure PA1 As Analog Input Pin */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* ADC1 Interrupt Configuration */
    NVIC_InitStruct.NVIC_IRQChannel   =  ADC1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0x02;
    NVIC_InitStruct.NVIC_IRQChannelCmd    = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void ADC_Configure(void)
{
    ADC_EXTI_Configure();

    ADC_ChannelConfigure();
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
uint16_t ADC_ReadChannelValue(void)
{
    /* Enable The Selected ADC Software Start Conversion */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    /* Wait ADC1 End Of Conversion */
    while(ADC_GetFlagStatus(ADC1, ADC_IT_EOC) == RESET);

    /* Clear ADC1 EOC Flag */
    ADC_ClearFlag(ADC1, ADC_IT_EOC);

    /* Get ADC1 Channel1 Conversion Result */
    return (uint16_t)ADC_GetConversionValue(ADC1);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
uint16_t ADC_ReadAverageValue(uint8_t Times)
{
    uint8_t  i   = 0;
    uint32_t Sum = 0;

    for(i = 0; i < Times; i++)
    {
        Sum += ADC_ReadChannelValue();
    }

    return (uint16_t)(Sum / Times);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void ADC_EXTI_TriggerEntry(void)
{
    if(ADC_EXTI_TriggerFlag)
    {
        ADC_EXTI_TriggerFlag = 0;

        /* Disable ADC1 EOC Interrupt */
        ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);

        printf("\r\nADC Value = %d", ADC_ReadAverageValue(8));

        /* Enable ADC1 EOC Interrupt */
        ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
    }
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void ADC_COMP_IRQHandler(void)
{
    if(ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET)
    {
        ADC_EXTI_TriggerFlag = 1;

        /* Clear ADC Analog Watchdog Flag */
        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

