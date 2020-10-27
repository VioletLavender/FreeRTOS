/******************************************************************************
 * @file    i2c.c
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
#define __I2C_C__


/* Includes -----------------------------------------------------------------*/
#include "i2c.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/


/* Private variables --------------------------------------------------------*/
uint8_t I2C_SlaveAddress = 0;


/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/


/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/


/******************************************************************************
 * @brief       Simulate I2C Software Delay
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
static void SimulateI2C_Delay(uint32_t Cnt)
{
    while(Cnt--);
}


/******************************************************************************
 * @brief       Set Simulate I2C SDA Input/Output Mode 
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
static void SimulateI2C_SDA_SetDirection(SimulateI2C_t *SimulateI2C, uint8_t Dir)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable Simulate I2C SDA Clock */
    RCC_AHBPeriphClockCmd(SimulateI2C->SDA_RCC, ENABLE);

    /* Simulate I2C SDA Configuration */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = SimulateI2C->SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    if(Dir) /* Input */
    {
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    }
    else    /* Output */
    {
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    }

    GPIO_Init(SimulateI2C->SDA_PORT, &GPIO_InitStructure);
}


/******************************************************************************
 * @brief       Set Simulate I2C SCL Input/Output Mode 
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
static void SimulateI2C_SCL_SetDirection(SimulateI2C_t* SimulateI2C, uint8_t Dir)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable Simulate I2C SCL Clock */
    RCC_AHBPeriphClockCmd(SimulateI2C->SCL_RCC, ENABLE);

    /* Simulate I2C SCL Configuration */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = SimulateI2C->SCL_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    if(Dir) /* Input */
    {
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    }
    else    /* Output */
    {
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    }

    GPIO_Init(SimulateI2C->SCL_PORT, &GPIO_InitStructure);
}


/******************************************************************************
 * @brief       Simulate I2C Generate Start Signal
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
static void SimulateI2C_GenerateStart(SimulateI2C_t* SimulateI2C)
{
    SimulateI2C_SDA_H(SimulateI2C);
    SimulateI2C_Delay(SimulateI2C->TIME);

    SimulateI2C_SCL_H(SimulateI2C);
    SimulateI2C_Delay(SimulateI2C->TIME);

    SimulateI2C_SDA_L(SimulateI2C);
    SimulateI2C_Delay(SimulateI2C->TIME);

    SimulateI2C_SCL_L(SimulateI2C);
    SimulateI2C_Delay(SimulateI2C->TIME);
}


/******************************************************************************
 * @brief       Simulate I2C Generate Stop Signal
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
static void SimulateI2C_GenerateStop(SimulateI2C_t* SimulateI2C)
{
    SimulateI2C_SDA_L(SimulateI2C);
    SimulateI2C_Delay(SimulateI2C->TIME);

    SimulateI2C_SCL_H(SimulateI2C);
    SimulateI2C_Delay(SimulateI2C->TIME);

    SimulateI2C_SDA_H(SimulateI2C);
    SimulateI2C_Delay(SimulateI2C->TIME);

    SimulateI2C_Delay(I2C_STOP_WAIT_VALUE); /* Must delay before next start */
}


/******************************************************************************
 * @brief       Simulate I2C Generate ACK/NACK
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
static void SimulateI2C_PutACK(SimulateI2C_t* SimulateI2C, uint8_t ack)
{
    if(ack)
    {
        SimulateI2C_SDA_H(SimulateI2C); /* NACK */
    }
    else
    {
        SimulateI2C_SDA_L(SimulateI2C); /* ACK  */
    }

    SimulateI2C_Delay(SimulateI2C->TIME);

    SimulateI2C_SCL_H(SimulateI2C);
    SimulateI2C_Delay(SimulateI2C->TIME);

    SimulateI2C_SCL_L(SimulateI2C);
    SimulateI2C_Delay(SimulateI2C->TIME);
}


/******************************************************************************
 * @brief       Simulate I2C Read ACK/NACK Signal
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
static uint8_t SimulateI2C_GetACK(SimulateI2C_t* SimulateI2C)
{
    uint8_t ack = 0;

    SimulateI2C_SDA_H(SimulateI2C);
    SimulateI2C_Delay(SimulateI2C->TIME);

    /* Change Simulate I2C SDA Direction To Input */
    SimulateI2C_SDA_SetDirection(SimulateI2C, 1);

    SimulateI2C_SCL_H(SimulateI2C);
    SimulateI2C_Delay(SimulateI2C->TIME);

    /* Read ACK Signal */
    ack = SimulateI2C_SDA_GET(SimulateI2C);

    SimulateI2C_SCL_L(SimulateI2C);
    SimulateI2C_Delay(SimulateI2C->TIME);

    /* Change Simulate I2C SDA Direction To Output */
    SimulateI2C_SDA_SetDirection(SimulateI2C, 0);

    return ack;
}


/******************************************************************************
 * @brief       Simulate I2C Read Byte Sequence
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
static uint8_t SimulateI2C_ReadByte(SimulateI2C_t* SimulateI2C)
{
    uint8_t i = 0, Data = 0;

    SimulateI2C_SDA_H(SimulateI2C); /* Must set SDA before read */

    /* Change Simulate I2C SDA Direction To Input */
    SimulateI2C_SDA_SetDirection(SimulateI2C, 1);

    for(i = 0; i < 8; i++)
    {
        SimulateI2C_SCL_H(SimulateI2C);
        SimulateI2C_Delay(SimulateI2C->TIME);

        Data <<= 1;

        if(SimulateI2C_SDA_GET(SimulateI2C))
        {
            Data |= 0x01;
        }

        SimulateI2C_SCL_L(SimulateI2C);
        SimulateI2C_Delay(SimulateI2C->TIME);
    }

    /* Change Simulate I2C SDA Direction To Output */
    SimulateI2C_SDA_SetDirection(SimulateI2C, 0);

    return Data;
}


/******************************************************************************
 * @brief       Simulate I2C Write Byte Sequence
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
static void SimulateI2C_WriteByte(SimulateI2C_t* SimulateI2C, uint8_t Data)
{
    uint8_t i = 8;

    for(i = 0; i < 8; i++)
    {
        if(Data & 0x80)
        {
            SimulateI2C_SDA_H(SimulateI2C);
        }
        else
        {
            SimulateI2C_SDA_L(SimulateI2C);
        }

        Data <<= 1;

        SimulateI2C_SCL_H(SimulateI2C);
        SimulateI2C_Delay(SimulateI2C->TIME);

        SimulateI2C_SCL_L(SimulateI2C);
        SimulateI2C_Delay(SimulateI2C->TIME);
    }
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void SimulateI2C_Configure(SimulateI2C_t* SimulateI2C, uint8_t Addr)
{
    /* Configure Simulate I2C SCL & SDA In Output Mode */
    SimulateI2C_SDA_SetDirection(SimulateI2C, 0);
    SimulateI2C_SCL_SetDirection(SimulateI2C, 0);

    SimulateI2C_SCL_H(SimulateI2C);
    SimulateI2C_Delay(SimulateI2C->TIME);

    SimulateI2C_SDA_H(SimulateI2C);
    SimulateI2C_Delay(SimulateI2C->TIME);

    I2C_SlaveAddress = Addr;
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
uint8_t SimulateI2C_ReadBytes(SimulateI2C_t* SimulateI2C, uint8_t Addr, uint8_t *pBuf, uint8_t Len)
{
    if(Len == 0)
    {
        return 0;
    }

    /* Generate START Signal */
    SimulateI2C_GenerateStart(SimulateI2C);

    /* Write Slave Device Addr And Write Operation */
    SimulateI2C_WriteByte(SimulateI2C, I2C_SlaveAddress);

    if(SimulateI2C_GetACK(SimulateI2C))
    {
        SimulateI2C_GenerateStop(SimulateI2C);
        return 1;
    }

    /* Write Slave Memory Addr */
    SimulateI2C_WriteByte(SimulateI2C, Addr);

    if(SimulateI2C_GetACK(SimulateI2C))
    {
        SimulateI2C_GenerateStop(SimulateI2C);
        return 1;
    }

    /* Generate Restart Signal */
    SimulateI2C_GenerateStart(SimulateI2C);

    /* Write Slave Device Addr And Read Operation */
    SimulateI2C_WriteByte(SimulateI2C, I2C_SlaveAddress + 1);

    if(SimulateI2C_GetACK(SimulateI2C))
    {
        SimulateI2C_GenerateStop(SimulateI2C);
        return 1;
    }

    /* Read Len Bytes Data To pBuf */
    while(1)
    {
        *pBuf++ = SimulateI2C_ReadByte(SimulateI2C);

        if(--Len == 0)
        {
            SimulateI2C_PutACK(SimulateI2C, 1);
            break;
        }

        SimulateI2C_PutACK(SimulateI2C, 0);
    }

    /* Generate STOP Signal */
    SimulateI2C_GenerateStop(SimulateI2C);

    return 0;
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
uint8_t SimulateI2C_WriteBytes(SimulateI2C_t *SimulateI2C, uint8_t Addr, uint8_t *pBuf, uint8_t Len)
{
    uint8_t i = 0;

    if(Len == 0) return 0;

    /* Generate START Signal */
    SimulateI2C_GenerateStart(SimulateI2C);

    /* Write Slave Device Addr And Write Operation */
    SimulateI2C_WriteByte(SimulateI2C, I2C_SlaveAddress);

    if(SimulateI2C_GetACK(SimulateI2C))
    {
        SimulateI2C_GenerateStop(SimulateI2C);
        return 1;
    }

    /* Write Slave Memory Addr */
    SimulateI2C_WriteByte(SimulateI2C, Addr);

    if(SimulateI2C_GetACK(SimulateI2C))
    {
        SimulateI2C_GenerateStop(SimulateI2C);
        return 1;
    }

    /* Write pBuf Data To EEPROM */
    for(i = 0; i < Len; i++)
    {
        SimulateI2C_WriteByte(SimulateI2C, *pBuf++);

        if(SimulateI2C_GetACK(SimulateI2C))
        {
            break;
        }
    }

    /* Generate STOP Signal */
    SimulateI2C_GenerateStop(SimulateI2C);

    if(i == Len)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

