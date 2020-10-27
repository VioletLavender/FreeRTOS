/******************************************************************************
 * @file    spi.c
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
#define __SPI_C__


/* Includes -----------------------------------------------------------------*/
#include "spi.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/


/* Private variables --------------------------------------------------------*/
uint8_t SPI_RxBuffer[20];
uint8_t SPI_RxFlag = 0;


/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/


/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/


/*
 *------------------------------------------------
 *          NSS     SCK     MISO    MOSI    AF
 *------------------------------------------------
 *   SPI1   PA4     PA5     PA6     PA7     AF0
 *   SPI2   PB12    PB13    PB14    PB15    AF0
 *------------------------------------------------
 */


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void SPIx_Configure(SPI_TypeDef *SPIx)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    if(SPIx == SPI1)
    {
        /* Enable SPI1 Clock */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

        /* SPI1 Configuration */
        SPI_StructInit(&SPI_InitStructure);
        SPI_InitStructure.SPI_Mode              = SPI_Mode_Slave;
        SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
        SPI_InitStructure.SPI_DataWidth         = 8;
        SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
        SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;
        SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
        SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
        SPI_Init(SPI1, &SPI_InitStructure);

        SPI_BiDirectionalLineConfig(SPI1, SPI_Direction_Rx);
        SPI_BiDirectionalLineConfig(SPI1, SPI_Direction_Tx);

        /* Enable SPI1 */
        SPI_Cmd(SPI1, ENABLE);

        /* Enable GPIOA Clock */
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,    ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

        /* Enable SPI1 GPIO Alternate Function */
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_0);    /* PA4 SPI1_NSS  */
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_0);    /* PA5 SPI1_SCK  */
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_0);    /* PA6 SPI1_MISO */
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_0);    /* PA7 SPI1_MOSI */

        /* SPI1_NSS(PA4) Configuration */
        GPIO_StructInit(&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        /* SPI1_SCK(PA5) Configuration */
        GPIO_StructInit(&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        /* SPI1_MISO(PA6) Configuration */
        GPIO_StructInit(&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        /* SPI1_MOSI(PA7) Configuration */
        GPIO_StructInit(&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    else if(SPIx == SPI2)
    {
        /* Enable SPI2 Clock */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

        /* SPI2 Configuration */
        SPI_StructInit(&SPI_InitStructure);
        SPI_InitStructure.SPI_Mode              = SPI_Mode_Slave;
        SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
        SPI_InitStructure.SPI_DataWidth         = 8;
        SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
        SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;
        SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
        SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
        SPI_Init(SPI2, &SPI_InitStructure);

        SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Rx);
        SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Tx);

        /* Enable SPI2 */
        SPI_Cmd(SPI2, ENABLE);

        /* Enable GPIOB Clock */
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,    ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

        /* Enable SPI2 GPIO Alternate Function */
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_0);   /* PB12 SPI2_NSS  */
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_0);   /* PB13 SPI2_SCK  */
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_0);   /* PB14 SPI2_MISO */
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_0);   /* PB14 SPI2_MOSI */

        /* SPI2_NSS(PB12) Configuration */
        GPIO_StructInit(&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        /* SPI2_SCK(PB13) Configuration */
        GPIO_StructInit(&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_FLOATING;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        /* SPI2_MISO(PB14) Configuration */
        GPIO_StructInit(&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        /* SPI2_MOSI(PB15) Configuration */
        GPIO_StructInit(&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    else
    {
    }
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void SPIx_DMA_Configure(SPI_TypeDef *SPIx)
{
    DMA_InitTypeDef  DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable DMA1 Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    if(SPIx == SPI1)
    {
        DMA_DeInit(DMA1_Channel2);

        /* DMA1 Channel2(SPI1_RX) Configuration */
        DMA_StructInit(&DMA_InitStructure);
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(SPI1->RDR);
        DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)SPI_RxBuffer;
        DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
        DMA_InitStructure.DMA_BufferSize         = 20;
        DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
        DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
        DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
        DMA_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;
        DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
        DMA_InitStructure.DMA_Auto_reload        = DMA_Auto_Reload_Disable;
        DMA_Init(DMA1_Channel2, &DMA_InitStructure);

        /* Enable DMA1 Channel Transmit Completed Interrupt */
        DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);

        /* Enable SPI1 DMA */
        SPI_DMACmd(SPI1, ENABLE);

        /* Enable DMA1 Channel2 */
        DMA_Cmd(DMA1_Channel2, ENABLE);

        /* DMA1 Channel2 Interrupt Configuration */
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
    else if(SPIx == SPI2)
    {
        DMA_DeInit(DMA1_Channel4);

        /* DMA1 Channel4(SPI2_RX) Configuration */
        DMA_StructInit(&DMA_InitStructure);
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(SPI2->RDR);
        DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)SPI_RxBuffer;
        DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
        DMA_InitStructure.DMA_BufferSize         = 20;
        DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
        DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
        DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
        DMA_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;
        DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
        DMA_InitStructure.DMA_Auto_reload        = DMA_Auto_Reload_Disable;
        DMA_Init(DMA1_Channel4, &DMA_InitStructure);

        /* Enable DMA1 Channel Transmit Completed Interrupt */
        DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);

        /* Enable SPI2 DMA */
        SPI_DMACmd(SPI2, ENABLE);

        /* Enable DMA1 Channel4 */
        DMA_Cmd(DMA1_Channel4, ENABLE);

        /* DMA1 Channel4 Interrupt Configuration */
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_5_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
    else
    {
    }
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void DMA1_Channel2_3_IRQHandler()
{
    /* DMA1 Channel2 SPI1_RX Transmit Completed */
    if(DMA_GetITStatus(DMA1_IT_TC2) != RESET)
    {
        SPI_RxFlag = 1;

        /* Clear TC2 Flag */
        DMA_ClearITPendingBit(DMA1_IT_TC2);
    }
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void DMA1_Channel4_5_IRQHandler()
{
    /* DMA1 Channel4 SPI2_RX Transmit Completed */
    if(DMA_GetITStatus(DMA1_IT_TC4) != RESET)
    {
        SPI_RxFlag = 1;

        /* Clear TC4 Flag */
        DMA_ClearITPendingBit(DMA1_IT_TC4);
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

