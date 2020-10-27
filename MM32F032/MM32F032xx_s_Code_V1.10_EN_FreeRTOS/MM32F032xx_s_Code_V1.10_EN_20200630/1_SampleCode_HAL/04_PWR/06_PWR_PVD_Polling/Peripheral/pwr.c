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
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void PWR_Configure(void)
{
    /* Enable PWR Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Configure the PVD Level to 1.8 ~ 4.8V */
//    PWR_PVDLevelConfig(PWR_CR_PLS_1V8);
//    PWR_PVDLevelConfig(PWR_CR_PLS_2V1);
//    PWR_PVDLevelConfig(PWR_CR_PLS_2V4);
//    PWR_PVDLevelConfig(PWR_CR_PLS_2V7);
//    PWR_PVDLevelConfig(PWR_CR_PLS_3V0);
//    PWR_PVDLevelConfig(PWR_CR_PLS_3V3);
//    PWR_PVDLevelConfig(PWR_CR_PLS_3V6);
//    PWR_PVDLevelConfig(PWR_CR_PLS_3V9);
//    PWR_PVDLevelConfig(PWR_CR_PLS_4V2);
//    PWR_PVDLevelConfig(PWR_CR_PLS_4V5);
    PWR_PVDLevelConfig(PWR_CR_PLS_4V8);

    /* Enable PVD Output */
    PWR_PVDCmd(ENABLE);
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

