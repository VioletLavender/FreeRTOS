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
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void ADC_ReadChannelVoltage(void)
{
    uint16_t Value = 0;

    /* Enable The Selected ADC Software Start Conversion */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    /* Wait ADC Conversion Finish */
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

    /* Clear ADC Conversion Flag */
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);

    /* Get ADC Conversion Result */
    Value = ADC_GetConversionValue(ADC1);

    printf("\r\nADC1 CH1 Voltage : %0.1f", ((float)Value / 4095.0 * 3.3));
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

