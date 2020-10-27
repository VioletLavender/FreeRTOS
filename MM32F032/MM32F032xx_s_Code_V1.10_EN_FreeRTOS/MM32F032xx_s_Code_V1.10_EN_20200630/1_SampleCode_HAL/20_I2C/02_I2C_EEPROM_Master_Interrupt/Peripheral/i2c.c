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
volatile uint8_t I2C_TxFlag = 0;
volatile uint8_t I2C_RxFlag = 0;
volatile uint8_t I2C_RxData = 0;


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
void I2C_GPIO_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIOB Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    /* I2C GPIO PB6(SCL) PB7(SDA) Configuration */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_20MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Enable Alternate Function */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_1);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void I2C_ReleaseBus(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIOB Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    /* Configure I2C GPIO In Input Mode , Release I2C Bus Control */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_20MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void I2C_Configure(uint8_t Addr)
{
    I2C_InitTypeDef  I2C_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Release I2C Bus */
    I2C_ReleaseBus();

    /* Enable I2C1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    /* I2C1 Configuration : Master Mode At 100kHz */
    I2C_StructInit(&I2C_InitStructure);
    I2C_InitStructure.Mode          = I2C_Mode_MASTER;
    I2C_InitStructure.OwnAddress    = 0;
    I2C_InitStructure.Speed         = I2C_Speed_STANDARD;
    I2C_InitStructure.ClockSpeed    = 100000;
    I2C_Init(I2C1, &I2C_InitStructure);

    /* I2C1 Interrupt Configuration */
    NVIC_InitStructure.NVIC_IRQChannel = I2C1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Set Slave Address */
    I2C_Send7bitAddress(I2C1, Addr, I2C_Direction_Transmitter);

    /* Enable I2C1 RX_FULL Interrupt */
    I2C_ITConfig(I2C1, I2C_IT_RX_FULL, ENABLE);

    /* Enable I2C1 */
    I2C_Cmd(I2C1, ENABLE);

    /* Configure I2C1 GPIO */
    I2C_GPIO_Configure();  
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void I2C1_IRQHandler(void)
{
    /* I2C1 Transmit Buffer Empty Interrupt Flag */
    if(I2C_GetITStatus(I2C1, I2C_IT_TX_EMPTY))
    {
        /* Clear TX_EMPTY Interrupt Flag */
        I2C_ClearITPendingBit(I2C1, I2C_IT_TX_EMPTY);

        /* Disable I2C1 TX_EMPTY Interrupt */
        I2C_ITConfig(I2C1, I2C_IT_TX_EMPTY, DISABLE);

        /* Set I2C Transmit Completed Flag */
        I2C_TxFlag = 1;
    }

    /* I2C1 Receive Buffer Not Empty Interrupt Flag */
    if(I2C_GetITStatus(I2C1, I2C_IT_RX_FULL))
    {
        /* Clear RX_FULL Interrupt Falg */
        I2C_ClearITPendingBit(I2C1, I2C_IT_RX_FULL);

        /* Receicve Data */
        I2C_RxData = I2C_ReceiveData(I2C1);

        /* Set I2C Receive Completed Flag */
        I2C_RxFlag = 1;
    }
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void I2C_TxByte(uint8_t data)
{
    /* Clear Tx Flag */
    I2C_TxFlag = 0x00;

    /* Send Data */
    I2C_SendData(I2C1, data);

    /* Enable I2C TX_EMPTY Interrupt */
    I2C_ITConfig(I2C1, I2C_IT_TX_EMPTY, ENABLE);

    /* Wait Transmit Completed */
    while(I2C_TxFlag == 0x0);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
uint8_t I2C_RxByte(void)
{
    /* Clear Rx Flag */
    I2C_RxFlag = 0x00;

    /* Send Read Command */
    I2C_ReadCmd(I2C1);

    /* Wait Receive Completed */
    while(I2C_RxFlag == 0x0);

    /* Return Received Data */
    return I2C_RxData;
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void I2C_ReadBytes(uint8_t Addr, uint8_t *pBuf, uint8_t Len)
{
    uint8_t i = 0;

    /* Send 24C02 Memory Address */
    I2C_TxByte(Addr);

    /* Read Data */
    for(i = 0; i < Len; i++)
    {
        pBuf[i] = I2C_RxByte();
    }

    /* Generate STOP Single */
    I2C_GenerateSTOP(I2C1, ENABLE);
    while(!I2C_GetITStatus(I2C1, I2C_IT_STOP_DET));
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void I2C_WriteBytes(uint8_t Addr, uint8_t *pBuf, uint8_t Len)
{
    uint8_t i = 0;

    /* Send 24C02 Memory Address */
    I2C_TxByte(Addr);

    /* Write Data */
    for(i = 0; i < Len; i++)
    {
        I2C_TxByte(pBuf[i]); 
    }

    /* Generate STOP Single */
    I2C_GenerateSTOP(I2C1, ENABLE);
    while(!I2C_GetITStatus(I2C1, I2C_IT_STOP_DET));
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/
