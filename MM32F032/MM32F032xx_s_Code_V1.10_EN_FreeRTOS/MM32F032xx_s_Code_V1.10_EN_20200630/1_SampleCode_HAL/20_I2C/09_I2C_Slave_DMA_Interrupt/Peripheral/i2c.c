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
volatile uint8_t I2C_DMAChannel2Flag = 0;
volatile uint8_t I2C_DMAChannel3Flag = 0;


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

    /* Enable I2C1 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    /* I2C1 Configuration : Slave Mode */
    I2C_StructInit(&I2C_InitStructure);
    I2C_InitStructure. Mode       = I2C_Mode_SLAVE;
    I2C_InitStructure. OwnAddress = 0;
    I2C_InitStructure. Speed      = I2C_Speed_STANDARD;
    I2C_InitStructure. ClockSpeed = 100000;
    I2C_Init(I2C1, &I2C_InitStructure);

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

    /* Set Slave Address */
    I2C_SendSlaveAddress(I2C1, Addr);
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

    DMA_InitTypeDef  DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

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

    /* DMA1 Interrupt Configure */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable DMA1 Channel3's TC Interrupt */
    DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);

    /* I2C1 Receive And Transmit DMA Enable */
    I2C1->DMA |=  (RDMAE_SET | TDMAE_SET);

    I2C_DMAChannel3Flag = 0;

    /* Enable DMA1 Channel2 & Channel3 */
    DMA_Cmd(DMA1_Channel2, ENABLE);
    DMA_Cmd(DMA1_Channel3, ENABLE);

    /* Wait DMA1 Channel3 Transfer Completed */
    while(I2C_DMAChannel3Flag == 0);

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
void DMA1_Channel2_3_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC2) != RESET)
    {
        I2C_DMAChannel2Flag = 1;

        DMA_ClearITPendingBit(DMA1_IT_TC2);
    }

    if(DMA_GetITStatus(DMA1_IT_TC3) != RESET)
    {
        I2C_DMAChannel3Flag = 1;

        DMA_ClearITPendingBit(DMA1_IT_TC3);
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

