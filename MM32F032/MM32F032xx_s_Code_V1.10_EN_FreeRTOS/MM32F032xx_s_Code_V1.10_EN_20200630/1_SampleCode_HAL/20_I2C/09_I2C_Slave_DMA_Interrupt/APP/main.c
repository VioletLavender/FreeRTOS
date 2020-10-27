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
#include "i2c.h"
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
/* Exported function prototypes ---------------------------------------------*/


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
int main(void)
{
    uint8_t i = 0, Buffer[100], Index = 0, Flag = 0, Data = 0;

    /* Init SysTick timer 1ms for SysTick_DelayMs */
    SysTick_Init(1000);

    /* Config UART1 with parameter(115200, N, 8, 1) for printf */
    UARTx_Configure(DEBUG_UART, 115200, UART_WordLength_8b, UART_StopBits_1, UART_Parity_No);

    /*
     * Config I2C1 Work in Slave  Mode, Owen Address is 0x50
     * Config I2Cx Work in Master Mode, 100k Speed For Test
     */
    I2C_Configure(0x50);

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
        /* flag = 0 : Wait For Receive Data From Master */
        if(Flag == 0)
        {
            /* I2C Receive Buffer Not Empty */
            if(I2C_GetFlagStatus(I2C1, I2C_FLAG_RX_FULL) != RESET)
            {
                /* Receive Data To buffer */
                I2C_DMA_RxData(&Buffer[Index++], 1);

                /* Generate STOP Signal, Release I2C Bus, Only Polling Mode */
                I2C_GenerateSTOP(I2C1, ENABLE);
            }

            /* 0xA5 Is EOP, Print Received From Master Data  */
            if(Buffer[Index - 1] == 0xA5)
            {
                printf("\r\n\r\n");

                for(i = 0; i < Index; i++)
                {
                    printf("0x%02x ", Buffer[i]);
                }

                Index = 0;
                Flag  = 1;
            }
        }
        else    /* I2C_Flag = 1 : Send Data To Master, When Master Send Receive Request To Slave */
        {
            /* Master Read Request */
            if(I2C_GetFlagStatus(I2C1, I2C_FLAG_RD_REQ)  != RESET)
            {
                /* Send Data To Master */
                I2C_SendData(I2C1, Data++);

                /* Clear Read Request Flag */
                I2C_ClearFlag(I2C1, I2C_FLAG_RD_REQ);

                /* Master Read 10 Bytes From Slave At One Time */
                if((Data % 10) == 0)
                {
                    Flag = 0;
                }
            }
        }
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

