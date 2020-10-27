/**
  *****************************************************************************
  * @file    bsp_led.c 
  * @author  King
  * @version V1.00
  * @date    20-May-2020
  * @brief   ......
  *****************************************************************************
  * @attention
  *
  * ......
  *
  *****************************************************************************
  */


/* Define to prevent recursive inclusion ------------------------------------*/
#define __BSP_LED_C__


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


/**
  * @brief  
  * @param  None
  * @retval None
  */
void BSP_LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* GPIOA and GPIOB Peripheral clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);

    /* Configure PA15 in output pushpull mode */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Reset PA15 : turn LED1 on */
    GPIO_ResetBits(GPIOA, GPIO_Pin_15);

    /* Configure PB3 PB4 and PB5 in output pushpull mode */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Reset PB3 PB4 and PB5 : turn LED2 LED3 and LED4 on */
    GPIO_ResetBits(GPIOB, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

