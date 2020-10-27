/******************************************************************************
 * @file    main.c
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
#define __MAIN_C__


/* Includes -----------------------------------------------------------------*/
#include "main.h"


/* Includes -----------------------------------------------------------------*/
#include "systick.h"
#include "uart.h"


/* Includes -----------------------------------------------------------------*/
#include "bsp_led.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/


/* Exported variables -------------------------------------------------------*/
extern UART_RxTx_TypeDef UART1_TxStruct;
extern UART_RxTx_TypeDef UART1_RxStruct;

extern UART_RxTx_TypeDef UART2_TxStruct;
extern UART_RxTx_TypeDef UART2_RxStruct;


/* Exported function prototypes ---------------------------------------------*/


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
int main(void)
{
    uint8_t i = 0;

    /* Init SysTick timer 1ms for SysTick_DelayMs */
    SysTick_Init(1000);

    /* Config UART1 with parameter(115200, N, 8, 1) for printf */
    UARTx_Configure(DEBUG_UART, 115200, UART_WordLength_8b, UART_StopBits_1, UART_Parity_No);

    /* Init LEDs GPIO */
    BSP_LED_Init();

    for(i = 0; i < 10; i++)
    {
        /* Toggle LED1~LED4 status */
        BSP_LED1_TOGGLE();
        BSP_LED2_TOGGLE();
        BSP_LED3_TOGGLE();
        BSP_LED4_TOGGLE();

        /* Delay 100ms */
        SysTick_DelayMs(100);
    }

    printf("\r\nMM32F032R8T6 %s %s\r\n", __DATE__, __TIME__);

    while(1)
    {
        /* Receive Data From DEBUG_UART and Send Data Back */

        printf("\r\n\r\nPlease Input 10 Characters...");

        /* Receive 10 Byte Data */
        UARTx_RxData(DEBUG_UART, 10);

        if(DEBUG_UART == UART1)
        {
            /* Wait Receive Complete Flag */
            while(UART1_RxStruct.CompleteFlag != 1);
        }
        else
        {
            /* Wait Receive Complete Flag */
            while(UART2_RxStruct.CompleteFlag != 1);
        }

        printf("\r\n\r\nThe Characters you input : ");

        if(DEBUG_UART == UART1)
        {
            /* Transmit Received Data */
            UARTx_TxData(DEBUG_UART, UART1_RxStruct.Buffer, UART1_RxStruct.Length);

            /* Wait Transmit Complete Flag */
            while(UART1_TxStruct.CompleteFlag != 1);
        }
        else
        {
            /* Transmit Received Data */
            UARTx_TxData(DEBUG_UART, UART2_RxStruct.Buffer, UART2_RxStruct.Length);

            /* Wait Transmit Complete Flag */
            while(UART2_TxStruct.CompleteFlag != 1);
        }
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

