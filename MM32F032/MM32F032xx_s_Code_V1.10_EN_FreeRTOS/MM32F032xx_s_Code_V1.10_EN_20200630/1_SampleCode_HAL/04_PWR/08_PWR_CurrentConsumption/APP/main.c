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
void InitSystem(void)
{
    /* Init SysTick timer 1ms for SysTick_DelayMs */
    SysTick_Init(1000);

    /* Config UART1 with parameter(9600, N, 8, 1) for printf */
    UARTx_Configure(DEBUG_UART, 9600, UART_WordLength_8b, UART_StopBits_1, UART_Parity_No);

    /* Init LEDs GPIO */
    BSP_LED_Init();

    printf("\r\nMM32F032R8T6 %s %s\r\n", __DATE__, __TIME__);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
int main(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable All GPIO Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);

    /* Config All GPIO Pins As Analog Input */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;

    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* Config wake up from stop mode by GPIO */
    PWR_Configure();

    /* Entry Stop Mode */
    PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);

    InitSystem();

    while(1)
    {
        /* Toggle LED1~LED4 status every 500ms */
        BSP_LED1_TOGGLE();
        BSP_LED2_TOGGLE();
        BSP_LED3_TOGGLE();
        BSP_LED4_TOGGLE();

        /* Delay 500ms */
        SysTick_DelayMs(500);
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

