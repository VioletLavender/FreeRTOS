/******************************************************************************
 * @file    rcc.c
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
#define __RCC_C__


/* Includes -----------------------------------------------------------------*/
#include "rcc.h"


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
void RCC_CSS_Configure(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable Clock Security System(CSS) */
    /* This Will Generate an NMI Exception When HSE Clock Fails */
    RCC_ClockSecuritySystemCmd(ENABLE);

    /* Enable And Configure RCC Global IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel  = RCC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void RCC_MCO_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIOA and SYSCFG Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,    ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* Enable PA8 MCO Alternate Functionio */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_0);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Select The PLLCLK_Div2 Clock Source To Output On MCO Pin */
    RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void RCC_Configure(void)
{
    RCC_CSS_Configure();

    RCC_MCO_Configure();
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void RCC_PrintClocksInformation(void)
{
    RCC_ClocksTypeDef  RCC_Clocks;

    RCC_GetClocksFreq(&RCC_Clocks);

    printf("\r\n");
    printf("\r\nSYSCLK %dHz", RCC_Clocks.SYSCLK_Frequency);
    printf("\r\nHCLK   %dHz", RCC_Clocks.HCLK_Frequency  );
    printf("\r\nPCLK1  %dHz", RCC_Clocks.PCLK1_Frequency );
    printf("\r\nPCLK2  %dHz", RCC_Clocks.PCLK2_Frequency );
    printf("\r\nADCCLK %dHz", RCC_Clocks.ADCCLK_Frequency);
    printf("\r\n");
}


/******************************************************************************
 * @brief       NMI exception
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void NMI_Handler(void)
{
    /* This interrupt is generated when HSE clock fails */
    if(RCC_GetITStatus(RCC_IT_CSS) != RESET)
    {
        /* Enable HSE */
        RCC_HSEConfig(RCC_HSE_ON);

        /* Enable HSE Ready interrupt */
        RCC_ITConfig(RCC_IT_HSERDY, ENABLE);

        /* Enable PLL Ready interrupt */
        RCC_ITConfig(RCC_IT_PLLRDY, ENABLE);

        /* Clear Clock Security System interrupt pending bit */
        RCC_ClearITPendingBit(RCC_IT_CSS);

        printf("\r\nNMI IRQ");
    }
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void RCC_CRS_IRQHandler(void)
{
    if(RCC_GetITStatus(RCC_IT_HSERDY) != RESET)
    {
        /* Clear HSERDY interrupt pending bit */
        RCC_ClearITPendingBit(RCC_IT_HSERDY);

        /* Check if the HSE clock is still available */
        if(RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET)
        {
            /* Enable PLL: once the PLL is ready the PLLRDY interrupt is generated */ 
            RCC_PLLCmd(ENABLE);
        }

        printf("\r\nRCC HSERDY IRQ");
    }

    if(RCC_GetITStatus(RCC_IT_PLLRDY) != RESET)
    {
        /* Clear PLLRDY interrupt pending bit */
        RCC_ClearITPendingBit(RCC_IT_PLLRDY);

        /* Check if the PLL is still locked */
        if(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != RESET)
        { 
            /* Select PLL as system clock source */
            RCC_SYSCLKConfig(RCC_PLL);
        }

        printf("\r\nRCC PLLRDY IRQ");
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

