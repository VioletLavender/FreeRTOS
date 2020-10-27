/******************************************************************************
 * @file    dma.c
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
#define __DMA_C__


/* Includes -----------------------------------------------------------------*/
#include "dma.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/


/* Private variables --------------------------------------------------------*/
volatile uint16_t DMA_CurrDataCnt = 0;


/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/


/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/


/******************************************************************************
 * @brief       DMA SRAM To SRAM : Byte To Byte
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void DMA_SRAMtoSRAM_8to8(void)
{
    const uint8_t SrcBuf[32] =
    {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
        0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
        0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
    };

    uint8_t  CheckError = 0;
    uint8_t  DstBuf[32];
    uint16_t i = 0;

    DMA_InitTypeDef  DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Clear DST_Buffer */
    memset(DstBuf, 0, sizeof(DstBuf));

    /* Enable DMA1 Channel1 IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel         = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd      = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable DMA1 Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* Configure DMA1 Channel1 */
    DMA_DeInit(DMA1_Channel1);

    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SrcBuf;
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)DstBuf;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize         = 32;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Enable;
    DMA_InitStructure.DMA_Auto_reload        = DMA_Auto_Reload_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    /* Enable DMA1 Channel1 Transfer Complete interrupt */
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

    /* Get Current Data Counter value before transfer begins */
    DMA_CurrDataCnt = DMA_GetCurrDataCounter(DMA1_Channel1);

    /* Enable DMA1 Channel1 Transfer */
    DMA_Cmd(DMA1_Channel1, ENABLE);

    /* Wait the end of transmission */
    while(DMA_CurrDataCnt != 0);

    /* Check if the transmitted and received data are equal */
    CheckError = 0;

    for(i = 0; i < 32; i++)
    {
        /* Compare Value */
        if(SrcBuf[i] != DstBuf[i])
        {
            CheckError = 1;
            break;
        }

        printf("0x%02x  ", DstBuf[i]);

        if(!((i + 1) % 8))
        {
            printf("\r\n");
        }
    }

    if(CheckError == 1)
    {
        printf("\r\nDMA Transfer FAILED!\r\n");
    }
    else
    {
        printf("\r\nDMA Transfer PASSED!\r\n");
    }

    /* Disable DMA1 Channel1 */
    DMA_Cmd(DMA1_Channel1, DISABLE);
}


/******************************************************************************
 * @brief       DMA SRAM To SRAM : Byte To HalfWord
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void DMA_SRAMtoSRAM_8to16(void)
{
    const uint8_t SrcBuf[32] =
    {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
        0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
        0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
    };

    uint8_t  CheckError = 0;
    uint16_t DstBuf[32];
    uint16_t i = 0;

    DMA_InitTypeDef  DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Clear DST_Buffer */
    memset(DstBuf, 0, sizeof(DstBuf));

    /* Enable DMA1 Channel1 IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel         = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd      = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable DMA1 Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* Configure DMA1 Channel1 */
    DMA_DeInit(DMA1_Channel1);

    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SrcBuf;
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)DstBuf;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize         = 32;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Enable;
    DMA_InitStructure.DMA_Auto_reload        = DMA_Auto_Reload_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    /* Enable DMA1 Channel1 Transfer Complete interrupt */
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

    /* Get Current Data Counter value before transfer begins */
    DMA_CurrDataCnt = DMA_GetCurrDataCounter(DMA1_Channel1);

    /* Enable DMA1 Channel1 Transfer */
    DMA_Cmd(DMA1_Channel1, ENABLE);

    /* Wait the end of transmission */
    while(DMA_CurrDataCnt != 0);

    /* Check if the transmitted and received data are equal */
    CheckError = 0;

    for(i = 0; i < 32; i++)
    {
        /* Compare Value */
        if(SrcBuf[i] != (DstBuf[i] & 0x00FF))
        {
            CheckError = 1;
            break;
        }

        printf("0x%04x  ", DstBuf[i]);

        if(!((i + 1) % 8))
        {
            printf("\r\n");
        }
    }

    if(CheckError == 1)
    {
        printf("\r\nDMA Transfer FAILED!\r\n");
    }
    else
    {
        printf("\r\nDMA Transfer PASSED!\r\n");
    }

    /* Disable DMA1 Channel1 */
    DMA_Cmd(DMA1_Channel1, DISABLE);
}


/******************************************************************************
 * @brief       DMA SRAM To SRAM : HalfWord To Byte
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void DMA_SRAMtoSRAM_16to8(void)
{
    const uint16_t SrcBuf[32] =
    {
        0x0101, 0x2302, 0x4503, 0x6704, 0x8905, 0x0106, 0x2307, 0x4508,
        0x6711, 0x8912, 0x0113, 0x2314, 0x4515, 0x6716, 0x8917, 0x0118,
        0x2321, 0x4522, 0x6723, 0x8924, 0x0125, 0x2326, 0x4527, 0x6728,
        0x8931, 0x0132, 0x2333, 0x4534, 0x6735, 0x8936, 0x0137, 0x2338,
    };

    uint8_t  CheckError = 0;
    uint8_t  DstBuf[32];
    uint16_t i = 0;

    DMA_InitTypeDef  DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Clear DST_Buffer */
    memset(DstBuf, 0, sizeof(DstBuf));

    /* Enable DMA1 Channel1 IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel         = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd      = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable DMA1 Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* Configure DMA1 Channel1 */
    DMA_DeInit(DMA1_Channel1);

    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SrcBuf;
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)DstBuf;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize         = 32;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Enable;
    DMA_InitStructure.DMA_Auto_reload        = DMA_Auto_Reload_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    /* Enable DMA1 Channel1 Transfer Complete interrupt */
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

    /* Get Current Data Counter value before transfer begins */
    DMA_CurrDataCnt = DMA_GetCurrDataCounter(DMA1_Channel1);

    /* Enable DMA1 Channel1 Transfer */
    DMA_Cmd(DMA1_Channel1, ENABLE);

    /* Wait the end of transmission */
    while(DMA_CurrDataCnt != 0);

    /* Check if the transmitted and received data are equal */
    CheckError = 0;

    for(i = 0; i < 32; i++)
    {
        /* Compare Value */
        if((SrcBuf[i] & 0x00FF) != DstBuf[i])
        {
            CheckError = 1;
            break;
        }

        printf("0x%04x  ", DstBuf[i]);

        if(!((i + 1) % 8))
        {
        printf("\r\n");
        }
    }

    if(CheckError == 1)
    {
        printf("\r\nDMA Transfer FAILED!\r\n");
    }
    else
    {
        printf("\r\nDMA Transfer PASSED!\r\n");
    }

    /* Disable DMA1 Channel1 */
    DMA_Cmd(DMA1_Channel1, DISABLE);
}


/******************************************************************************
 * @brief       DMA SRAM To SRAM
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void DMA_SRAMtoSRAM(void)
{
    printf("\r\nDMA SRAM To SRAM (Byte To Byte) :\r\n");

    DMA_SRAMtoSRAM_8to8();

    printf("\r\nDMA SRAM To SRAM (Byte To HalfWord) :\r\n");

    DMA_SRAMtoSRAM_8to16();

    printf("\r\nDMA SRAM To SRAM (HalfWord To Byte) :\r\n");

    DMA_SRAMtoSRAM_16to8();
}


/******************************************************************************
 * @brief       DMA1 Channel 1 interrupt Handles
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void DMA1_Channel1_IRQHandler(void)
{
    /* Test on DMA1 Channel1 Transfer Complete interrupt */
    if(DMA_GetITStatus(DMA1_IT_TC1))
    {
        /* Get Current Data Counter value after complete transfer */
        DMA_CurrDataCnt = DMA_GetCurrDataCounter(DMA1_Channel1);

        /* Clear DMA1 Channel1 Transfer Complete interrupt pending bits */
        DMA_ClearITPendingBit(DMA1_IT_TC1);
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

