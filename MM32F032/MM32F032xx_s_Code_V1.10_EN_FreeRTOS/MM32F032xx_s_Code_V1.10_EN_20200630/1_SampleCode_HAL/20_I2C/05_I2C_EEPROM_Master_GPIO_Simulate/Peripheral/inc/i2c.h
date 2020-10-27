/******************************************************************************
 * @file    i2c.h
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
#ifndef __I2C_H__
#define __I2C_H__


#ifdef __cplusplus
extern "C" {
#endif


#undef  EXTERN


#ifdef  __I2C_C__
#define EXTERN
#else
#define EXTERN extern
#endif


/* Includes -----------------------------------------------------------------*/
#include "config.h"


/* Exported constants -------------------------------------------------------*/
#define I2C_DELAY_VALUE         100
#define I2C_STOP_WAIT_VALUE     100


/* Exported types -----------------------------------------------------------*/
typedef struct
{
    uint32_t      SCL_RCC;
    GPIO_TypeDef *SCL_PORT;
    uint16_t      SCL_PIN;

    uint32_t      SDA_RCC;
    GPIO_TypeDef *SDA_PORT;
    uint16_t      SDA_PIN;

    uint32_t      TIME;
} SimulateI2C_t;


/* Exported macro -----------------------------------------------------------*/
#define SimulateI2C_SCL_H(SimulateI2C)    GPIO_WriteBit(SimulateI2C->SCL_PORT, SimulateI2C->SCL_PIN, Bit_SET)
#define SimulateI2C_SCL_L(SimulateI2C)    GPIO_WriteBit(SimulateI2C->SCL_PORT, SimulateI2C->SCL_PIN, Bit_RESET)

#define SimulateI2C_SDA_H(SimulateI2C)    GPIO_WriteBit(SimulateI2C->SDA_PORT, SimulateI2C->SDA_PIN, Bit_SET)
#define SimulateI2C_SDA_L(SimulateI2C)    GPIO_WriteBit(SimulateI2C->SDA_PORT, SimulateI2C->SDA_PIN, Bit_RESET)

#define SimulateI2C_SCL_GET(SimulateI2C)  GPIO_ReadOutputDataBit(SimulateI2C->SCL_PORT, SimulateI2C->SCL_PIN)
#define SimulateI2C_SDA_GET(SimulateI2C)  GPIO_ReadInputDataBit( SimulateI2C->SDA_PORT, SimulateI2C->SDA_PIN)


/* Exported functions -------------------------------------------------------*/
EXTERN void    SimulateI2C_Configure(SimulateI2C_t *SimulateI2C, uint8_t Addr);
EXTERN uint8_t SimulateI2C_ReadBytes(SimulateI2C_t *SimulateI2C, uint8_t Addr, uint8_t *pBuf, uint8_t Len);
EXTERN uint8_t SimulateI2C_WriteBytes(SimulateI2C_t *SimulateI2C, uint8_t Addr, uint8_t *pBuf, uint8_t Len);


#ifdef __cplusplus
}
#endif


#endif


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

