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
#include "pwr.h"
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
void delay_nop(uint32_t cnt)
{
    uint8_t i = 0, j = 0;

    while(cnt--)
    {
        for(i = 0; i < 100; i++)
        {
            for(j = 0; j < 20; j++)
            {
                __NOP();
            }
        }
    }
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
int main(void)
{
    /* Init SysTick timer 1ms for SysTick_DelayMs */
    SysTick_Init(1000);

    /* Config UART1 with parameter(9600, N, 8, 1) for printf */
    UARTx_Configure(DEBUG_UART, 9600, UART_WordLength_8b, UART_StopBits_1, UART_Parity_No);

    /* Init LEDs GPIO */
    BSP_LED_Init();

    /* Config wake up from stop mode by RTC */
    PWR_Configure();

    printf("\r\nMM32F032R8T6 %s %s\r\n", __DATE__, __TIME__);

    while(1)
    {
        delay_nop(500);

        /* Wait till RTC Second event occurs */
        RTC_ClearFlag(RTC_FLAG_SEC);
        while(RTC_GetFlagStatus(RTC_FLAG_SEC) == RESET);

        /* Set The RTC Alarm Value */
        RTC_SetAlarm(RTC_GetCounter() + 3);
        /* Wait Until Last Write Operation On RTC REG Has Finished */
        RTC_WaitForLastTask();

        /* Turn LED1 On */
        BSP_LED1_ON();

        /* Enter Stop Mode */
        PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);

        /* Turn LED1 Off */
        BSP_LED1_OFF();
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

