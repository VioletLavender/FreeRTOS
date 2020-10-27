/******************************************************************************
 * @file    pwr.c
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
#define __PWR_C__


/* Includes -----------------------------------------------------------------*/
#include "pwr.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/


/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/


/******************************************************************************
 * @brief       Wake up from standby mode by IWDG
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void PWR_Configure(void)
{
    /* Enable The LSI OSC */
    RCC_LSICmd(ENABLE);

    /* Wait Till LSI Is Ready */
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

    PVU_CheckStatus();

    /* Enable Write Access to IWDG_PR and IWDG_RLR Registers */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    /* IWDG Counter Clock: LSI / 32 */
    IWDG_SetPrescaler(IWDG_Prescaler_32);

    /* Set counter reload value to obtain 250ms IWDG TimeOut.
     Counter Reload Value = 1000ms / IWDG counter clock period
                          = 1000ms / (LSI/32)
                          = 1s     / (LsiFreq/32)
                          = LsiFreq/(32 * 1)
                          = LsiFreq/32
                          = 40000/32
                          = 1248
    */
    IWDG_SetReload(0x4E0);

    /* Reload IWDG Counter */
    IWDG_ReloadCounter();

    /* Enable IWDG */
    IWDG_Enable();
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

