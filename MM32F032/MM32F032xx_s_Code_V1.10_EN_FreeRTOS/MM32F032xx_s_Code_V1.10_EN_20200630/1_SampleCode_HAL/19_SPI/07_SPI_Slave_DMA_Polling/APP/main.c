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
#include "spi.h"
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
extern uint8_t SPI_RxBuf[20];


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

    /* Config UARTx with parameter(115200, N, 8, 1) for printf */
    UARTx_Configure(DEBUG_UART, 115200, UART_WordLength_8b, UART_StopBits_1, UART_Parity_No);

    /* Config SPIx as slave */
    SPIx_Configure(DEBUG_SPIx);

    SPIx_DMA_Configure(DEBUG_SPIx);

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
        if(DEBUG_SPIx == SPI1)
        {
            /* DMA1 Channel2 SPI1_RX Transmit Completed */
            if(DMA_GetFlagStatus(DMA1_FLAG_TC2) != RESET)
            {
                /* Clear TC4 Flag */
                DMA_ClearFlag(DMA1_FLAG_TC2);

                for(i = 0; i < 20; i++)
                {
                    printf("0x%02x ", SPI_RxBuf[i]);

                    if(((i + 1) % 10) == 0)
                    {
                        printf("\r\n");
                    }
                }
            }
        }
        else
        {
            /* DMA1 Channel4 SPI2_RX Transmit Completed */
            if(DMA_GetFlagStatus(DMA1_FLAG_TC4) != RESET)
            {
                /* Clear TC4 Flag */
                DMA_ClearFlag(DMA1_FLAG_TC4);

                for(i = 0; i < 20; i++)
                {
                    printf("0x%02x ", SPI_RxBuf[i]);

                    if(((i + 1) % 10) == 0)
                    {
                        printf("\r\n");
                    }
                }
            }
        }
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

