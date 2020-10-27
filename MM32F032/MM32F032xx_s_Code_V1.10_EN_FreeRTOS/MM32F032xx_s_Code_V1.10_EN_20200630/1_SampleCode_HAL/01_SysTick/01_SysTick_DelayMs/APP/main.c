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
#include "FreeRTOS.h"
#include "task.h"

#define TASK1_STACK_SIZE          128
#define TASK1_PRIORITY            1

#define TASK2_STACK_SIZE          128
#define TASK2_PRIORITY            2

#define TASK3_STACK_SIZE          128
#define TASK3_PRIORITY            3

#define TASK4_STACK_SIZE          128
#define TASK4_PRIORITY            4

/************************************************
函数名称 ： vAppTask1
功    能 ： 应用任务1
参    数 ： pvParameters --- 可选参数
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void vAppTask1(void *pvParameters)
{
  for(;;)
  {
    BSP_LED1_TOGGLE();
    vTaskDelay(100);
  }
}

/************************************************
函数名称 ： vAppTask2
功    能 ： 应用任务2
参    数 ： pvParameters --- 可选参数
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void vAppTask2(void *pvParameters)
{
  for(;;)
  {
    BSP_LED2_TOGGLE();
    vTaskDelay(100);
  }
}

/************************************************
函数名称 ： vAppTask3
功    能 ： 应用任务3
参    数 ： pvParameters --- 可选参数
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void vAppTask3(void *pvParameters)
{
  for(;;)
  {
    BSP_LED3_TOGGLE();
    vTaskDelay(100);
  }
}

/************************************************
函数名称 ： vAppTask4
功    能 ： 应用任务4
参    数 ： pvParameters --- 可选参数
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void vAppTask4(void *pvParameters)
{
  for(;;)
  {
    BSP_LED4_TOGGLE();
    vTaskDelay(100);
  }
}
/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/


/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/

void AppTaskCreate(void)
{
  xTaskCreate(vAppTask1, "Task1", TASK1_STACK_SIZE, NULL, TASK1_PRIORITY, NULL);
  xTaskCreate(vAppTask2, "Task2", TASK2_STACK_SIZE, NULL, TASK2_PRIORITY, NULL);
  xTaskCreate(vAppTask3, "Task3", TASK3_STACK_SIZE, NULL, TASK3_PRIORITY, NULL);
  xTaskCreate(vAppTask4, "Task4", TASK4_STACK_SIZE, NULL, TASK4_PRIORITY, NULL);
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
    BSP_LED_Init();
    /* Config UART1 with parameter(115200, N, 8, 1) for printf */
    UARTx_Configure(DEBUG_UART, 115200, UART_WordLength_8b, UART_StopBits_1, UART_Parity_No);
    printf("MM32TEST");
    AppTaskCreate();

  /* 3、开启任务 */
  vTaskStartScheduler();

  return 0;		
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

