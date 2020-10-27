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
void RCC_AutoCalcPLLFactor(uint32_t FCLKO, uint32_t FREFIN, uint8_t *PLLDIV, uint8_t *PLLMUL)
{
    uint8_t  Div  = 0, Mul  = 0;
    uint32_t Freq = 0, Diff = 0;
    uint32_t MinDiff = FCLKO;

    for(Div = 0; Div < 8; Div++)
    {
        for(Mul = 0; Mul < 64; Mul++)
        {
            Freq = FREFIN * (Mul + 1) / (Div + 1);

            Diff = (Freq > FCLKO) ? (Freq - FCLKO) : (FCLKO - Freq);

            if(Diff < MinDiff)
            {
                *PLLDIV = Div;
                *PLLMUL = Mul;
                MinDiff = Diff;
            }

            if(Diff == 0) return;
        }
    }
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void RCC_ClockConfigure(uint32_t SystemClock)
{
    uint8_t PLLDIV = 0, PLLMUL = 0;

    /* Reset The RCC Clock Configuration To Default State */
    RCC_DeInit();

    /* Enable HSI */
    RCC_HSICmd(ENABLE);

    /* Wait Till HSE Is Ready */
    while(RCC_WaitForFlagStartUp(RCC_FLAG_HSIRDY) != SUCCESS);

    /* Set Latency By System Clock */
    if(SystemClock <= 24000000)
    {
        FLASH_SetLatency(FLASH_Latency_0);
    }
    else if(SystemClock <= 48000000)
    {
        FLASH_SetLatency(FLASH_Latency_1);
    }
    else if(SystemClock <= 72000000)
    {
        FLASH_SetLatency(FLASH_Latency_2);
    }
    else /* SystemClock <= 96000000 */
    {
        FLASH_SetLatency(FLASH_Latency_3);
    }

    /* Prefetch Buffer Enable */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* AHB = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    if(SystemClock > 36000000)
    {
        /* APB1 = HCLK / 2 */
        RCC_PCLK1Config(RCC_HCLK_Div2);
    }
    else
    {
        /* APB1 = HCLK */
        RCC_PCLK1Config(RCC_HCLK_Div1);
    }

    /* APB2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PLL = HSI / 4, PLLSRC : HSI */
    RCC_PLLConfig(RCC_HSI_Div4, RCC_PLLMul_2);

    /* Disable PLLON */
    RCC_PLLCmd(DISABLE);

    /* Set PLLMUL and PLLDIV */
    RCC_AutoCalcPLLFactor(SystemClock, 12000000, &PLLDIV, &PLLMUL);
    RCC_PLLDMDNConfig(PLLMUL, PLLDIV);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);
    /* Wait Till PLL Is Ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_PLL);

    /* System Clock Switch Status */
    while(RCC_GetSYSCLKSource() != 0x08);
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
void RCC_Configure(uint32_t SystemClock)
{
    RCC_ClockConfigure(SystemClock);

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


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

