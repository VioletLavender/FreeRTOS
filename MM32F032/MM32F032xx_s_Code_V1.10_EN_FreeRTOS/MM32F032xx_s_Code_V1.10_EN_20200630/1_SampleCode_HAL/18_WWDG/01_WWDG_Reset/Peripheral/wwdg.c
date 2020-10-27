/******************************************************************************
 * @file    wwdg.c
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
#define __WWDG_C__


/* Includes -----------------------------------------------------------------*/
#include "wwdg.h"


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
void WWDG_Configure(void)
{
    /* Enable WWDG Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

    /* Set The WWDG Prescaler */
    WWDG_SetPrescaler(WWDG_Prescaler_8);

    /* Set The WWDG Window Value */
    WWDG_SetWindowValue(80);

    /* Enable WWDG And Load The Counter Value, PCLK1 = 24MHz , WWDT Timeout = 1365us * 64 = 87.38ms
     * 1365us * (127 - 80) = 64.17ms < refresh window < 1365us * 64 = 87.38ms
     */
    WWDG_Enable(127);
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

