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
uint8_t UART_PrintData = 0;
uint8_t UART_Completed = 0;


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
    NVIC_InitTypeDef NVIC_InitStructure;

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

        /* UART Interrupt Configuration */
        NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPriority  = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd  = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
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
        UART_Init(UART1, &UART_InitStructure);

        /* Enable UART */
        UART_Cmd(UART1, ENABLE);

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

        /* UART Interrupt Configuration */
        NVIC_InitStructure.NVIC_IRQChannel = UART2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPriority  = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd  = ENABLE;
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
void UART1_IRQHandler(void)
{
    if(UART_GetITStatus(UART1, UART_IT_TXIEN) != RESET)
    {
        /* Clear UART1 TX Interrupt Flag */
        UART_ClearITPendingBit(UART1, UART_IT_TXIEN);

        /* Transmit Data */
        UART_SendData(UART1, UART_PrintData);

        /* Disable UART1 TX Interrupt, Send One Byte Every Once */
        UART_ITConfig(UART1, UART_IT_TXIEN, DISABLE);

        /* Set UART1 Transmit Complete Flag */
        UART_Completed = 1;
    }
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void UART2_IRQHandler(void)
{
    if(UART_GetITStatus(UART2, UART_IT_TXIEN) != RESET)
    {
        /* Clear UART1 TX Interrupt Flag */
        UART_ClearITPendingBit(UART2, UART_IT_TXIEN);

        /* Transmit Data */
        UART_SendData(UART2, UART_PrintData);

        /* Disable UART1 TX Interrupt, Send One Byte Every Once */
        UART_ITConfig(UART2, UART_IT_TXIEN, DISABLE);

        /* Set UART1 Transmit Complete Flag */
        UART_Completed = 1;
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
    UART_PrintData = (uint8_t)ch;

    /* Clear UART1 Transmit Complete Flag */
    UART_Completed = 0;

    /* Enable UART TX Interrupt */
    UART_ITConfig(DEBUG_UART, UART_IT_TXIEN, ENABLE);

     /* Wait UART1 Transmit Complete */
    while(UART_Completed == 0);

    return ch;
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

