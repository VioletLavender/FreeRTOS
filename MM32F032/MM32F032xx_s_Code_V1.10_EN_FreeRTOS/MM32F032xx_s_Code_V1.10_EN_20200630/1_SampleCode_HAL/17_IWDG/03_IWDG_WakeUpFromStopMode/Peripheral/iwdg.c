/******************************************************************************
 * @file    iwdg.c
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
#define __IWDG_C__


/* Includes -----------------------------------------------------------------*/
#include "iwdg.h"


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
void IWDG_Configure(uint32_t Timeout)
{
    uint16_t Reload = 0;

    /* IWDG Counter Clock = LSI / 32, So.. */
    Reload = LSI_VALUE / 1000 * Timeout / 32;

    /* Enable The LSI OSC */
    RCC_LSICmd(ENABLE);

    /* Wait Till LSI Is Ready */
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

    /* Checks prescaler value has been updated */	
    PVU_CheckStatus();

    /* Enable Write Access to IWDG_PR and IWDG_RLR Registers */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    /* IWDG Counter Clock: LSI / 32 */
    IWDG_SetPrescaler(IWDG_Prescaler_32);

    /* Set counter reload value */
    IWDG_SetReload(Reload);

    /* Reload IWDG Counter */
    IWDG_ReloadCounter();

    /* Enable IWDG */
    IWDG_Enable();
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

