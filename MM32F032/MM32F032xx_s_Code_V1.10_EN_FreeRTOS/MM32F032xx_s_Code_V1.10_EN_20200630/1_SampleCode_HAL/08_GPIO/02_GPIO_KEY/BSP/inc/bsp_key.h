/******************************************************************************
 * @file    bsp_key.h
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
#ifndef __KEY_H__
#define __KEY_H__


#ifdef __cplusplus
extern "C" {
#endif


#undef  EXTERN


#ifdef  __KEY_C__
#define EXTERN
#else
#define EXTERN extern
#endif


/* Includes -----------------------------------------------------------------*/
#include "config.h"


/* Exported constants -------------------------------------------------------*/
#define KEY1_GPIO           GPIOB
#define KEY1_PIN            GPIO_Pin_1

#define KEY2_GPIO           GPIOB
#define KEY2_PIN            GPIO_Pin_2

#define KEY3_GPIO           GPIOB
#define KEY3_PIN            GPIO_Pin_10

#define KEY4_GPIO           GPIOB
#define KEY4_PIN            GPIO_Pin_11


/* Exported types -----------------------------------------------------------*/


/* Exported macro -----------------------------------------------------------*/
#define BSP_KEY1_READ()     GPIO_ReadInputDataBit(KEY1_GPIO, KEY1_PIN)
#define BSP_KEY2_READ()     GPIO_ReadInputDataBit(KEY2_GPIO, KEY2_PIN)
#define BSP_KEY3_READ()     GPIO_ReadInputDataBit(KEY3_GPIO, KEY3_PIN)
#define BSP_KEY4_READ()     GPIO_ReadInputDataBit(KEY4_GPIO, KEY4_PIN)


/* Exported functions -------------------------------------------------------*/
EXTERN void BSP_KEY_Init(void);


#ifdef __cplusplus
}
#endif


#endif


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

