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

    /* Enable Alternate Function */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_1);

    /* I2C GPIO PB6(SCL) PB7(SDA) Configuration */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_20MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
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

    /* Set Slave Address */
    I2C_Send7bitAddress(I2C1, Addr, I2C_Direction_Transmitter);

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
void I2C_DMA_TxData(uint8_t *TxData, uint16_t TxSize)
{
    DMA_InitTypeDef DMA_InitStructure;

    /* Enable DMA1 Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_DeInit(DMA1_Channel2);

    /* DMA1 Channel2 I2C1_TX Channel Configuration */
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (uint32_t)&(I2C1->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr        = (uint32_t)TxData;
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize            = TxSize;
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable ;
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority              = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;
    DMA_InitStructure.DMA_Auto_reload           = DMA_Auto_Reload_Disable;
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);

    /* I2C1 Transmit DMA Enable */
    I2C1->DMA |=  TDMAE_SET;

    /* Enable DMA1 Channel2 */
    DMA_Cmd(DMA1_Channel2, ENABLE);

    /* Wait DMA1 Channel2 Transfer Completed */
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC2));

    /* I2C1 Transmit DMA Disable */
    I2C1->DMA &= ~TDMAE_SET;

    /* Disable DMA1 Channel2 */
    DMA_Cmd(DMA1_Channel2, DISABLE);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void I2C_DMA_RxData(uint8_t *RxData, uint16_t RxSize)
{
    unsigned int TxData = 0x100;

    DMA_InitTypeDef DMA_InitStructure;

    /* Enable DMA1 Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* DMA1 Channel2 I2C1_TX Channel Configuration */
    DMA_DeInit(DMA1_Channel2);
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (uint32_t)&(I2C1->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr        = (uint32_t)&TxData;
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize            = RxSize;
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Disable  ;
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority              = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;
    DMA_InitStructure.DMA_Auto_reload           = DMA_Auto_Reload_Disable;
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);

    /* DMA1 Channel3 I2C1_RC Channel Configuration */
    DMA_DeInit(DMA1_Channel3);
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (uint32_t)&(I2C1->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr        = (uint32_t)RxData;
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize            = RxSize;
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable ;
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority              = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;
    DMA_InitStructure.DMA_Auto_reload           = DMA_Auto_Reload_Disable;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);

    /* I2C1 Receive And Transmit DMA Enable */
    I2C1->DMA |=  (RDMAE_SET | TDMAE_SET);

    /* Enable DMA1 Channel2 & Channel3 */
    DMA_Cmd(DMA1_Channel2, ENABLE);
    DMA_Cmd(DMA1_Channel3, ENABLE);

    /* Wait DMA1 Channel3 Transfer Completed */
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC3));

    /* I2C1 Receive And Transmit DMA Disable */
    I2C1->DMA &= ~(RDMAE_SET | TDMAE_SET);

    /* Disable DMA1 Channel2 & Channel3 */
    DMA_Cmd(DMA1_Channel3, DISABLE);
    DMA_Cmd(DMA1_Channel2, DISABLE);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void I2C_ReadBytes(uint8_t Addr, uint8_t *pBuf, uint8_t Len)
{
    /* Send EEPROM Memory Address */
    I2C_DMA_TxData((uint8_t *)&Addr, 1);

    /* Read Data From EEPROM By DMA */
    I2C_DMA_RxData(pBuf, Len);

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
    /* Send EEPROM Memory Address */
    I2C_DMA_TxData((uint8_t *)&Addr, 1);

    /* Write Data To EEPROM */
    I2C_DMA_TxData(pBuf, Len);

    /* Wait Write Completed */
    while(!I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_TFE));

    /* Generate STOP Single */
    I2C_GenerateSTOP(I2C1, ENABLE);
    while(!I2C_GetITStatus(I2C1, I2C_IT_STOP_DET));
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

