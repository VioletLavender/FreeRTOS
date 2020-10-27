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
#include "queue.h"


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
void I2C_Configure(uint8_t Addr)
{
    I2C_InitTypeDef  I2C_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable I2C1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    /* I2C1 Configuration : Slave Mode */
    I2C_StructInit(&I2C_InitStructure);
    I2C_InitStructure. Mode       = I2C_Mode_SLAVE;
    I2C_InitStructure. OwnAddress = 0;
    I2C_InitStructure. Speed      = I2C_Speed_STANDARD;
    I2C_InitStructure. ClockSpeed = 100000;
    I2C_Init(I2C1, &I2C_InitStructure);

    /* Enable I2C1 Read Request Interrupt */
    I2C_ITConfig(I2C1, I2C_IT_RD_REQ,  ENABLE);

    /* Enable I2C1 Receive Interrupt */
    I2C_ITConfig(I2C1, I2C_IT_RX_FULL, ENABLE);

    /* Enable I2C1 */
    I2C_Cmd(I2C1, ENABLE);

    /* Enable GPIOB Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,    ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* Enable Alternate Function */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_1);

    /* I2C GPIO PB6(SCL) Configuration */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* I2C GPIO PB7(SDA) Configuration */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* I2C1 Interrupt Configuration */
    NVIC_InitStructure.NVIC_IRQChannel = I2C1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Set Slave Address */
    I2C_SendSlaveAddress(I2C1, Addr);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void I2C1_IRQHandler(void)
{
    static uint8_t Data = 0;

    /* Master Read Request */
    if(I2C_GetITStatus(I2C1, I2C_IT_RD_REQ)  != RESET)
    {
        /* Send Data To Master */
        I2C_SendData(I2C1, Data++);

        /* Clear Read Request Interrupt Flag */
        I2C_ClearITPendingBit(I2C1, I2C_IT_RD_REQ);
    }

    /* I2C1 Receive Buffer Not Empty */
    if(I2C_GetITStatus(I2C1, I2C_IT_RX_FULL) != RESET)
    {
        /* Receive Data */
        QUEUE_WRITE(QUEUE_I2C_RX_IDX, I2C_ReceiveData(I2C1));
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

