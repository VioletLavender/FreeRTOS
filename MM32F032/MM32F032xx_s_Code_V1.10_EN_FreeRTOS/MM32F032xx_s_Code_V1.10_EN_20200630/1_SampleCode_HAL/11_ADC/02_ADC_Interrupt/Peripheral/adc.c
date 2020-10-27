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
volatile uint8_t  ADC_Flag   = 0;
volatile uint16_t ADC_Result = 0;


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
void ADC_Configure(void)
{
    ADC_InitTypeDef  ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable ADC1 Clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    /* ADC1 Configuration */
    ADC_StructInit(&ADC_InitStructure);
    ADC_InitStructure.ADC_Resolution       = ADC_Resolution_12b;
    ADC_InitStructure.ADC_PRESCARE         = ADC_PCLK2_PRESCARE_16;
    ADC_InitStructure.ADC_Mode             = ADC_Mode_Imm;
    ADC_InitStructure.ADC_DataAlign        = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_Init(ADC1, &ADC_InitStructure);

    /* Enable The Selected ADC Channel And Configure Its Sample Time */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 0, ADC_Samctl_239_5);

    /* Enable ADC End Of Conversion Interrupt */
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    /* Enable GPIOA Clock */
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);

    /* Config PA1 As Analog Input Pin */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* ADC Interrupt Configuration */
    NVIC_InitStructure.NVIC_IRQChannel     = ADC1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority =  0x02;
    NVIC_InitStructure.NVIC_IRQChannelCmd     = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable The Selected ADC Software Start Conversion */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
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
        /* Clear ADC Conversion IT Flag */
        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);

        /* Disable The Selected ADC Software Start Conversion */
        ADC_SoftwareStartConvCmd(ADC1, DISABLE);

        ADC_Flag = 1;

        /* Get ADC Conversion Result */
        ADC_Result = ADC_GetConversionValue(ADC1);
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

