/******************************************************************************
 * @file    uart.c
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
#define __UART_C__


/* Includes -----------------------------------------------------------------*/
#include "uart.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/


/* Private variables --------------------------------------------------------*/
volatile uint8_t DMA1_Channel2CompleteFlag = 0;
volatile uint8_t DMA1_Channel3CompleteFlag = 0;
volatile uint8_t DMA1_Channel4CompleteFlag = 0;
volatile uint8_t DMA1_Channel5CompleteFlag = 0;


/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/


/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/


/******************************************************************************
 * @brief       Config UART1 or UART2
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void UARTx_Configure(UART_TypeDef           *UARTx,
                     uint32_t                BaudRate,
                     UART_WordLength_TypeDef WordLength,
                     UART_Stop_Bits_TypeDef  StopBits,
                     UART_Parity_TypeDef     Parity)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;

    if(UARTx == UART1)
    {
        /* Enable UART1 clock */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);

        /* UART Configuration ad follow */
        UART_StructInit(&UART_InitStructure);
        UART_InitStructure.UART_BaudRate            = BaudRate;
        UART_InitStructure.UART_WordLength          = WordLength;
        UART_InitStructure.UART_StopBits            = StopBits;
        UART_InitStructure.UART_Parity              = Parity;
        UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
        UART_InitStructure.UART_Mode                = UART_Mode_Rx | UART_Mode_Tx;
        UART_Init(UART1, &UART_InitStructure);

        /* Enable The UART DMA Interface */
        UART_DMACmd(UART1, UART_DMAReq_EN, ENABLE);

        /* Enable UART */
        UART_Cmd(UART1, ENABLE);

        /* Enable GPIO and SYSCFG clock */
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,    ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

        /* Enable PA9 & PA10 alternate function */
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_1);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

        /* Configure UART Tx as alternate function push-pull */
        GPIO_StructInit(&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

         /* Configure UART Rx as input floating */
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    else if(UARTx == UART2)
    {
        /* Enable UART2 clock */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART2, ENABLE);

        /* UART Configuration ad follow */
        UART_StructInit(&UART_InitStructure);
        UART_InitStructure.UART_BaudRate            = BaudRate;
        UART_InitStructure.UART_WordLength          = WordLength;
        UART_InitStructure.UART_StopBits            = StopBits;
        UART_InitStructure.UART_Parity              = Parity;
        UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
        UART_InitStructure.UART_Mode                = UART_Mode_Rx | UART_Mode_Tx;
        UART_Init(UART2, &UART_InitStructure);

        /* Enable The UART DMA Interface */
        UART_DMACmd(UART2, UART_DMAReq_EN, ENABLE);

        /* Enable UART */
        UART_Cmd(UART2, ENABLE);

        /* Enable GPIO and SYSCFG clock */
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,    ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

        /* Enable PA2 & PA3 alternate function */
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,  GPIO_AF_1);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource3,  GPIO_AF_1);

        /* Configure UART Tx as alternate function push-pull */
        GPIO_StructInit(&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

         /* Configure UART Rx as input floating */
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    else
    {
    }
}


/******************************************************************************
 * @brief       Config UART1 or UART2 DMA Channel
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void UARTx_DMA_Configure(UART_TypeDef *UARTx)
{
    DMA_InitTypeDef  DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable DMA1 Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    if(UARTx == UART1)
    {
        /* Use DMA1 Channel2 As UART1 Tx DMA Channel */
        EXTI_DMAChannelRemapConfig(EXTI_DMARemap_UART1Tx, DISABLE);

        /* Use DMA1 Channel3 As UART1 Rx DMA Channel */
        EXTI_DMAChannelRemapConfig(EXTI_DMARemap_UART1Rx, DISABLE);

        /* Deinitializes The DMA Channel2 */
        DMA_DeInit(DMA1_Channel2);

        /* UART1 Tx DMA Configuration */
        DMA_StructInit(&DMA_InitStructure);
        DMA_InitStructure.DMA_PeripheralBaseAddr    = (uint32_t)&(UART1->TDR);
        DMA_InitStructure.DMA_MemoryBaseAddr        = NULL;
        DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralDST;
        DMA_InitStructure.DMA_BufferSize            = 0;
        DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;
        DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Byte;
        DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;
        DMA_InitStructure.DMA_Mode                  = DMA_Mode_Normal;
        DMA_InitStructure.DMA_Priority              = DMA_Priority_VeryHigh;
        DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;
        DMA_InitStructure.DMA_Auto_reload           = DMA_Auto_Reload_Disable;
        DMA_Init(DMA1_Channel2, &DMA_InitStructure);

        /* Enable DMA1 Channel2's TC Interrupt */
        DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);

        /* Deinitializes The DMA Channel3 */
        DMA_DeInit(DMA1_Channel3);

        /* UART1 Rx DMA Configuration */
        DMA_StructInit(&DMA_InitStructure);
        DMA_InitStructure.DMA_PeripheralBaseAddr    = (uint32_t)&(UART1->RDR);
        DMA_InitStructure.DMA_MemoryBaseAddr        = NULL;
        DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralSRC;
        DMA_InitStructure.DMA_BufferSize            = 0;
        DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;
        DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Byte;
        DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;
        DMA_InitStructure.DMA_Mode                  = DMA_Mode_Normal;
        DMA_InitStructure.DMA_Priority              = DMA_Priority_VeryHigh;
        DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;
        DMA_InitStructure.DMA_Auto_reload           = DMA_Auto_Reload_Disable;
        DMA_Init(DMA1_Channel3, &DMA_InitStructure);

        /* Enable DMA1 Channel3's TC Interrupt */
        DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);

        /* DMA1 Interrupt Configuration */
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
    else
    {
        /* Deinitializes The DMA Channel4 */
        DMA_DeInit(DMA1_Channel4);

        /* UART2 Tx DMA Configuration */
        DMA_StructInit(&DMA_InitStructure);
        DMA_InitStructure.DMA_PeripheralBaseAddr    = (uint32_t)&(UART2->TDR);
        DMA_InitStructure.DMA_MemoryBaseAddr        = NULL;
        DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralDST;
        DMA_InitStructure.DMA_BufferSize            = 0;
        DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;
        DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Byte;
        DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;
        DMA_InitStructure.DMA_Mode                  = DMA_Mode_Normal;
        DMA_InitStructure.DMA_Priority              = DMA_Priority_VeryHigh;
        DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;
        DMA_InitStructure.DMA_Auto_reload           = DMA_Auto_Reload_Disable;
        DMA_Init(DMA1_Channel4, &DMA_InitStructure);

        /* Enable DMA1 Channel4's TC Interrupt */
        DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);

        /* Deinitializes The DMA Channel5 */
        DMA_DeInit(DMA1_Channel5);

        /* UART2 Rx DMA Configuration */
        DMA_StructInit(&DMA_InitStructure);
        DMA_InitStructure.DMA_PeripheralBaseAddr    = (uint32_t)&(UART2->RDR);
        DMA_InitStructure.DMA_MemoryBaseAddr        = NULL;
        DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralSRC;
        DMA_InitStructure.DMA_BufferSize            = 0;
        DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;
        DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Byte;
        DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;
        DMA_InitStructure.DMA_Mode                  = DMA_Mode_Normal;
        DMA_InitStructure.DMA_Priority              = DMA_Priority_VeryHigh;
        DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;
        DMA_InitStructure.DMA_Auto_reload           = DMA_Auto_Reload_Disable;
        DMA_Init(DMA1_Channel5, &DMA_InitStructure);

        /* Enable DMA1 Channel5's TC Interrupt */
        DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);

        /* DMA1 Interrupt Configuration */
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_5_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
}


/******************************************************************************
 * @brief       UART1 or UART2 Send Data
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void UARTx_DMA_TxData(UART_TypeDef *UARTx, uint8_t *pBuf, uint8_t Len)
{
    if(UARTx == UART1)
    {
        /* Disable DMA1 Channel2 */
        DMA_Cmd(DMA1_Channel2, DISABLE);

        /* Set DMA1 Channel2's Memory Address */
        exDMA_SetMemoryAddress(DMA1_Channel2, (u32)pBuf);

        /* Set DMA1 Channel2's Transmit Length */
        exDMA_SetTransmitLen(DMA1_Channel2, Len);

        DMA1_Channel2CompleteFlag = 0;

        /* Enable DMA1 Channel2 */
        DMA_Cmd(DMA1_Channel2, ENABLE);

        /* Wait DMA Transmit Complete */
        while(DMA1_Channel2CompleteFlag == 0);

        /* Disable DMA1 Channel2 */
        DMA_Cmd(DMA1_Channel2, DISABLE);

        /* Wait UART1 TXD Shift Register Empty */
        while(!UART_GetFlagStatus(UART1, UART_FLAG_TXEMPTY));
    }
    else
    {
        /* Disable DMA1 Channel4 */
        DMA_Cmd(DMA1_Channel4, DISABLE);

        /* Set DMA1 Channel2's Memory Address */
        exDMA_SetMemoryAddress(DMA1_Channel4, (u32)pBuf);

        /* Set DMA1 Channel2's Transmit Length */
        exDMA_SetTransmitLen(DMA1_Channel4, Len);

        DMA1_Channel4CompleteFlag = 0;

        /* Enable DMA1 Channel2 */
        DMA_Cmd(DMA1_Channel4, ENABLE);

        /* Wait DMA Transmit Complete */
        while(DMA1_Channel4CompleteFlag == 0);

        /* Disable DMA1 Channel2 */
        DMA_Cmd(DMA1_Channel4, DISABLE);

        /* Wait UART2 TXD Shift Register Empty */
        while(!UART_GetFlagStatus(UART2, UART_FLAG_TXEMPTY));
    }
}


/******************************************************************************
 * @brief       UART1 or UART2 Receive Data
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void UARTx_DMA_RxData(UART_TypeDef *UARTx, uint8_t *pBuf, uint8_t Len)
{
    if(UARTx == UART1)
    {
        /* Disable DMA1 Channel3 */
        DMA_Cmd(DMA1_Channel3, DISABLE);

        /* Set DMA1 Channel3's Memory Address */
        exDMA_SetMemoryAddress(DMA1_Channel3, (u32)pBuf);

        /* Set DMA1 Channel3's Transmit Length */
        exDMA_SetTransmitLen(DMA1_Channel3, Len);

        DMA1_Channel3CompleteFlag = 0;

        /* Enable DMA1 Channel3 */
        DMA_Cmd(DMA1_Channel3, ENABLE);

        /* Wait DMA Transmit Complete */
        while(DMA1_Channel3CompleteFlag == 0);

        /* Disable DMA1 Channel3 */
        DMA_Cmd(DMA1_Channel3, DISABLE);
    }
    else
    {
        /* Disable DMA1 Channel5 */
        DMA_Cmd(DMA1_Channel5, DISABLE);

        /* Set DMA1 Channel5's Memory Address */
        exDMA_SetMemoryAddress(DMA1_Channel5, (u32)pBuf);

        /* Set DMA1 Channel5's Transmit Length */
        exDMA_SetTransmitLen(DMA1_Channel5, Len);

        DMA1_Channel5CompleteFlag = 0;

        /* Enable DMA1 Channel5 */
        DMA_Cmd(DMA1_Channel5, ENABLE);

        /* Wait DMA Transmit Complete */
        while(DMA1_Channel5CompleteFlag == 0);

        /* Disable DMA1 Channel5 */
        DMA_Cmd(DMA1_Channel5, DISABLE);
    }
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void DMA1_Channel2_3_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC2) != RESET)
    {
        DMA1_Channel2CompleteFlag = 1;

        DMA_ClearITPendingBit(DMA1_IT_TC2);
    }

    if(DMA_GetITStatus(DMA1_IT_TC3) != RESET)
    {
        DMA1_Channel3CompleteFlag = 1;

        DMA_ClearITPendingBit(DMA1_IT_TC3);
    }
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void DMA1_Channel4_5_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC4) != RESET)
    {
        DMA1_Channel4CompleteFlag = 1;

        DMA_ClearITPendingBit(DMA1_IT_TC4);
    }

    if(DMA_GetITStatus(DMA1_IT_TC5) != RESET)
    {
        DMA1_Channel5CompleteFlag = 1;

        DMA_ClearITPendingBit(DMA1_IT_TC5);
    }
}


/******************************************************************************
 * @brief       Retargets the C library printf function to the UART.
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
int fputc(int ch, FILE *f)
{
    /* Send a character to the UART */
    UART_SendData(DEBUG_UART, (uint8_t)ch);

     /* Loop until the end of transmission */
    while(!UART_GetFlagStatus(DEBUG_UART, UART_FLAG_TXEPT));

    return ch;
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

