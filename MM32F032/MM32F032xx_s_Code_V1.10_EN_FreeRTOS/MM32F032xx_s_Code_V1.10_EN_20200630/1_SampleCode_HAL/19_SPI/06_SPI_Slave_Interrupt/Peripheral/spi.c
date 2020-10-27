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
    NVIC_InitTypeDef NVIC_InitStructure;

    if(SPIx == SPI1)
    {
        /* Enable SPI1 Clock */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

        /* SPIx Configuration */
        SPI_StructInit(&SPI_InitStructure);
        SPI_InitStructure.SPI_Mode              = SPI_Mode_Slave;
        SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
        SPI_InitStructure.SPI_DataWidth         = 8;
        SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
        SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;
        SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
        SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
        SPI_Init(SPIx, &SPI_InitStructure);

        /* Enable SPIx Receive FIFO Interrupt */
        SPI_ITConfig(SPI1, SPI_IT_RX, ENABLE);

        SPI_BiDirectionalLineConfig(SPI1, SPI_Direction_Rx);
        SPI_BiDirectionalLineConfig(SPI1, SPI_Direction_Tx);

        /* Enable SPIx */
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

        /* SPI2 Interrupt Configuration */
        NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
    else if(SPIx == SPI2)
    {
        /* Enable SPI2 Clock */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

        /* SPIx Configuration */
        SPI_StructInit(&SPI_InitStructure);
        SPI_InitStructure.SPI_Mode              = SPI_Mode_Slave;
        SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
        SPI_InitStructure.SPI_DataWidth         = 8;
        SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
        SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;
        SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
        SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
        SPI_Init(SPIx, &SPI_InitStructure);

        /* Enable SPIx Receive FIFO Interrupt */
        SPI_ITConfig(SPI2, SPI_IT_RX, ENABLE);

        SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Rx);
        SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Tx);

        /* Enable SPIx */
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

        /* SPI2 Interrupt Configuration */
        NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
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
void SPI1_IRQHandler(void)
{
    static uint8_t Count = 0;

    if(SPI_GetITStatus(SPI1, SPI_IT_RX) != RESET)
    {
        /* Receive Data */
        uint8_t data = SPI_ReceiveData(SPI1);

        /* Transmit Data */
        SPI_SendData(SPI1, data);

        /* Print Data On Terminal */
        printf("0x%02x ", data);

        Count = (Count + 1) % 10;

        if(Count == 0)
        {
            printf("\r\n");
        }

        SPI_ClearITPendingBit(SPI1, SPI_IT_RX);
    }
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void SPI2_IRQHandler(void)
{
    static uint8_t Count = 0;

    if(SPI_GetITStatus(SPI2, SPI_IT_RX) != RESET)
    {
        /* Receive Data */
        uint8_t data = SPI_ReceiveData(SPI2);

        /* Transmit Data */
        SPI_SendData(SPI2, data);

        /* Print Data On Terminal */
        printf("0x%02x ", data);

        Count = (Count + 1) % 10;

        if(Count == 0)
        {
            printf("\r\n");
        }

        SPI_ClearITPendingBit(SPI2, SPI_IT_RX);
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

