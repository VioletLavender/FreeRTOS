/**
  *****************************************************************************
  * @file    adc.h
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
#ifndef __ADC_H__
#define __ADC_H__


#ifdef __cplusplus
extern "C" {
#endif


#undef  EXTERN


#ifdef  __ADC_C__
#define EXTERN
#else
#define EXTERN extern
#endif


/* Includes -----------------------------------------------------------------*/
#include "config.h"


/* Exported constants -------------------------------------------------------*/
/* Exported types -----------------------------------------------------------*/
/* Exported macro -----------------------------------------------------------*/


/* Exported functions -------------------------------------------------------*/
EXTERN void ADC_Configure(void);
EXTERN void ADC_EXTI_TriggerEntry(void);


#ifdef __cplusplus
}
#endif


#endif


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

