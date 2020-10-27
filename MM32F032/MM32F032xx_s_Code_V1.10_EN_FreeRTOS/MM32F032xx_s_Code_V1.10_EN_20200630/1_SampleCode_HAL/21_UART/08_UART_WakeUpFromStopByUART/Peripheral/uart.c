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
volatile uint8_t UART_RxData = 0;


/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/


/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/


/******************************************************************************
 * @brief       Config UART1 or UART2 EXTI
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void UARTx_EXTI_Configure(UART_TypeDef *UARTx)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;


    if(UARTx == UART1)
    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,    ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

        GPIO_StructInit(&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource10);

        EXTI_StructInit(&EXTI_InitStructure);
        EXTI_InitStructure.EXTI_Line    = EXTI_Line10;
        EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure);

        NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
    else
    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,    ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

        GPIO_StructInit(&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource3);

        EXTI_StructInit(&EXTI_InitStructure);
        EXTI_InitStructure.EXTI_Line    = EXTI_Line3;
        EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure);

        NVIC_InitStructure.NVIC_IRQChannel = EXTI2_3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
}


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

    /* Reset UART Configuration */
    UART_DeInit(UARTx);


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

        /* Enable UART Rx Interrupt */
        UART_ITConfig(UART1, UART_IT_RXIEN, ENABLE);

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
        NVIC_InitStructure.NVIC_IRQChannelPriority  = 1;
        NVIC_InitStructure.NVIC_IRQChannelCmd  = ENABLE;
        NVIC_Init(&NVIC_InitStructure);    }
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

        /* Enable UART Rx Interrupt */
        UART_ITConfig(UART2, UART_IT_RXIEN, ENABLE);

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

        /* UART Interrupt Configuration */
        NVIC_InitStructure.NVIC_IRQChannel = UART2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPriority  = 1;
        NVIC_InitStructure.NVIC_IRQChannelCmd  = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
    else
    {
    }
}


/******************************************************************************
 * @brief       UART2 RX PA3
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void EXTI2_3_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line3) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}


/******************************************************************************
 * @brief       UART1 RX PA10
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void EXTI4_15_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line10) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line10);
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
    if(UART_GetITStatus(UART1, UART_IT_RXIEN) != RESET)
    {
        /* Receive UART Data */
        UART_RxData = UART_ReceiveData(UART1);

        /* Clear UART Rx Interrupt Flag */
        UART_ClearITPendingBit(UART1, UART_IT_RXIEN);
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
    if(UART_GetITStatus(UART2, UART_IT_RXIEN) != RESET)
    {
        /* Receive UART Data */
        UART_RxData = UART_ReceiveData(UART2);

        /* Clear UART Rx Interrupt Flag */
        UART_ClearITPendingBit(UART2, UART_IT_RXIEN);
    }
}


/******************************************************************************
 * @brief       UART1 or UART2 Send Data
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void UARTx_TxData(UART_TypeDef *UARTx, uint8_t Data)
{
    /* send a character to the UART */
    UART_SendData(UARTx, Data);

     /* Loop until the end of transmission */
    while(UART_GetFlagStatus(UARTx, UART_FLAG_TXEPT) == RESET);
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

