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
extern volatile uint8_t UART_RxData;


/* Exported function prototypes ---------------------------------------------*/


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void InitSystem(void)
{
    /* Init SysTick timer 1ms for SysTick_DelayMs */
    SysTick_Init(1000);

    /* Config UART1 with parameter(9600, N, 8, 1) for printf */
    UARTx_Configure(DEBUG_UART, 9600, UART_WordLength_8b, UART_StopBits_1, UART_Parity_No);

    /* Init LEDs GPIO */
    BSP_LED_Init();
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
int main(void)
{
    uint8_t i = 0, WakeUpFlag = 0;

    /* Init System */
    InitSystem();

    printf("\r\nMM32F032R8T6 %s %s\r\n", __DATE__, __TIME__);

    while(1)
    {
        if(WakeUpFlag == 1)
        {
            WakeUpFlag = 0;

            /* Reset EXTI Configuration */
            EXTI_DeInit();

            exEXTI_LineDisable(EXTI_Line3 );
            exEXTI_LineDisable(EXTI_Line10);

            /* Init System Again After Wake Up From Stop Mode */
            InitSystem();

            printf("\r\nMCU Waked Up From Stop Mode\r\n");
        }

        /* Toggle LED1~LED4 status */
        BSP_LED1_TOGGLE();
        BSP_LED2_TOGGLE();
        BSP_LED3_TOGGLE();
        BSP_LED4_TOGGLE();

        /* Delay 500ms */
        SysTick_DelayMs(500);

        if((UART_RxData == 'S') || (UART_RxData == 's'))
        {
            UART_RxData = 0;

            printf("\r\nMCU Will Enter Stop Mode...\r\n");

            /* Disable UART */
            UART_Cmd(DEBUG_UART, DISABLE);

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

            /* Config UART Tx PIN As EXTI PIN */
            UARTx_EXTI_Configure(DEBUG_UART);

            /* Enter Stop Mode */
            PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);

            WakeUpFlag = 1;
        }
        else
        {
            if(UART_RxData != 0)
            {
                UARTx_TxData(DEBUG_UART, UART_RxData);

                UART_RxData = 0;
            }
        }
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

