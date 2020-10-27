/******************************************************************************
 * @file    queue.h
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
#ifndef __QUEUE_H__
#define __QUEUE_H__


#ifdef __cplusplus
extern "C" {
#endif


#undef EXTERN


#ifdef  __QUEUE_C__
#define EXTERN
#else
#define EXTERN extern
#endif


/* Includes -----------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>


/* Exported constants -------------------------------------------------------*/
#define QUEUE_SIZE              500
#define QUEUE_NUMBER            1

#define QUEUE_I2C_RX_IDX        0


/* Exported types -----------------------------------------------------------*/
typedef struct
{
    uint8_t  data[QUEUE_SIZE];
    uint16_t head;
    uint16_t tail;
} QUEUE_InitTypeDef;


/* Exported macro -----------------------------------------------------------*/


/* Exported functions -------------------------------------------------------*/
EXTERN void    QUEUE_INIT(uint8_t index);
EXTERN uint8_t QUEUE_EMPTY(uint8_t index);
EXTERN uint8_t QUEUE_READ(uint8_t index);
EXTERN void    QUEUE_WRITE(uint8_t index, uint8_t data);


#ifdef __cplusplus
}
#endif


#endif


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

