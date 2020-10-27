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
#include "bsp_led.h"
#include "queue.h"
#include "systick.h"


/* Private typedef ----------------------------------------------------------*/


/* Private define -----------------------------------------------------------*/
#define SimulateI2C_STATE_NA        0
#define SimulateI2C_STATE_STA       1
#define SimulateI2C_STATE_ADD       2
#define SimulateI2C_STATE_ADD_ACK   3
#define SimulateI2C_STATE_DAT       4
#define SimulateI2C_STATE_DAT_ACK   5
#define SimulateI2C_STATE_STO       6


/* Private define -----------------------------------------------------------*/
#define SimulateI2C_SCL_GPIO        GPIOB
#define SimulateI2C_SCL_PIN         GPIO_Pin_6

#define SimulateI2C_SDA_GPIO        GPIOB
#define SimulateI2C_SDA_PIN         GPIO_Pin_7


/* Private macro ------------------------------------------------------------*/
#define SimulateI2C_READ_SCL()      GPIO_ReadInputDataBit(SimulateI2C_SCL_GPIO, SimulateI2C_SCL_PIN)
#define SimulateI2C_READ_SDA()      GPIO_ReadInputDataBit(SimulateI2C_SDA_GPIO, SimulateI2C_SDA_PIN)


/* Private variables --------------------------------------------------------*/
uint8_t SimulateI2C_State = SimulateI2C_STATE_NA;

uint8_t SimulateI2C_ShiftCounter = 0;
uint8_t SimulateI2C_SlaveAddress = 0;
uint8_t SimulateI2C_ReceivedData = 0;
uint8_t SimulateI2C_TransmitData = 0x50;

uint8_t SimulateI2C_TransmitBuffer[16] = 
{
    0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78,
    0x89, 0x9A, 0xAB, 0xBC, 0xCD, 0xDE, 0xEF, 0xF0,
};
uint8_t SimulateI2C_TransmitIndex = 0;


/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/


/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/


/******************************************************************************
 * @brief       配置模拟I2C的GPIO端口, 默认设置成输入模式, 并使能相应的外部触发
 *              中断功能(上升沿和下降沿)
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void SimulateI2C_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable GPIOB Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    /* Configure PB6(I2C_SCLK) PB7(I2C_SDA) as pull down input */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Selects the PB6(I2C_SCLK) used as EXTI Line */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource6);

    /* Configure PB6(I2C_SCLK) EXTI Line */
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line    = EXTI_Line6;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Selects the PB7(I2C_SDA) used as EXTI Line */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource7);

    /* Configure PB7(I2C_SDA) EXTI Line */
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line    = EXTI_Line7;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable EXTI Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority  = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd     = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/******************************************************************************
 * @brief       设置SDA信号线的输入输出方便, 0代表Output输出, 1代表Input输入
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void SimulateI2C_SDA_SetDirection(uint8_t Dir)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    if(Dir) /* Input */
    {
        GPIO_StructInit(&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = SimulateI2C_SDA_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
        GPIO_Init(SimulateI2C_SDA_GPIO, &GPIO_InitStructure);
    }
    else    /* Output */
    {
        GPIO_StructInit(&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = SimulateI2C_SDA_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;
        GPIO_Init(SimulateI2C_SDA_GPIO, &GPIO_InitStructure);
    }
}


/******************************************************************************
 * @brief       设置SDA信号线的输出电平(高电平 / 低电平)
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void SimulateI2C_SDA_SetLevel(uint8_t level)
{
    SimulateI2C_SDA_SetDirection(0);

    if(level)
    {
        GPIO_WriteBit(SimulateI2C_SDA_GPIO, SimulateI2C_SDA_PIN, Bit_SET);
    }
    else
    {
        GPIO_WriteBit(SimulateI2C_SDA_GPIO, SimulateI2C_SDA_PIN, Bit_RESET);
    }
}


/******************************************************************************
 * @brief       当SCL触发上升沿外部中断时的处理
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void SimulateI2C_SCL_RiseHandler(void)
{
    /* SCL为上升沿, 数据锁定, 主从机从SDA总线上获取数据位 */
    switch(SimulateI2C_State)
    {
     case SimulateI2C_STATE_ADD:

        /* I2C发送遵义MSB, 先发送高位, 再发送低位, 所以在接收的时候, 数据进行左移 */

        SimulateI2C_SlaveAddress <<= 1;
        SimulateI2C_ShiftCounter  += 1;

        if(SimulateI2C_READ_SDA() ==  Bit_SET)
        {
            SimulateI2C_SlaveAddress |= 0x01;
        }

        /* 当接收到8位地址位后, 从机需要在第9个时钟给出ACK应答, 等待SCL下降沿的时候给出ACK信号 */
        if(SimulateI2C_ShiftCounter == 8)
        {
            SimulateI2C_State = SimulateI2C_STATE_ADD_ACK;
        }
        break;

     case SimulateI2C_STATE_ADD_ACK:
        /* 从机地址的ACK回复后, 切换到收发数据状态 */
        SimulateI2C_State = SimulateI2C_STATE_DAT;

        SimulateI2C_ShiftCounter = 0;   /* 数据移位计数器清零 */
        SimulateI2C_ReceivedData = 0;   /* SimulateI2C的接收数据清零 */
        break;

     case SimulateI2C_STATE_DAT:
        if((SimulateI2C_SlaveAddress & 0x01) == 0x00)
        {
            /* 主机写操作：此时从机应该获取主机发送的SDA信号线电平状态, 进行位存储 */
            SimulateI2C_ReceivedData <<= 1;
            SimulateI2C_ShiftCounter  += 1;

            if(SimulateI2C_READ_SDA() == Bit_SET)
            {
                SimulateI2C_ReceivedData |= 0x01;
            }

            /* 当收到一个完整的8位数据时, 将收到的数据存放到I2C接收消息队列中, 状态转换到给主机发送ACK应答 */
            if(SimulateI2C_ShiftCounter == 8)
            {
                QUEUE_WRITE(QUEUE_I2C_RX_IDX, SimulateI2C_ReceivedData);

                SimulateI2C_ShiftCounter = 0;  /* 数据移位计数器清零 */
                SimulateI2C_ReceivedData = 0;  /* SimulateI2C的接收数据清零 */

                SimulateI2C_State = SimulateI2C_STATE_DAT_ACK;
            }
        }
        else
        {
            /* 主机读操作： 在SCL上升沿的时候, 主机获取当前SDA的状态位, 如果到了第8个数位的上升沿,
             * 那接下来就是主机回复从机的应答或非应答信号了, 所以将状态切换到等待ACK的状态, 同时准备下一个需要发送的数据
             */
            if(SimulateI2C_ShiftCounter == 8)
            {
                SimulateI2C_ShiftCounter = 0;      /* SimulateI2C的接收数据清零 */
                SimulateI2C_TransmitData = SimulateI2C_TransmitBuffer[SimulateI2C_TransmitIndex++];

                SimulateI2C_TransmitIndex %= 16;

                SimulateI2C_State = SimulateI2C_STATE_DAT_ACK;
            }
        }
        break;

     case SimulateI2C_STATE_DAT_ACK:
        if((SimulateI2C_SlaveAddress & 0x01) == 0x00)
        {
            /* 主机写操作：从机发送ACK, 等待主机读取从机发送的ACK信号 */

            SimulateI2C_State = SimulateI2C_STATE_DAT;  /* 状态切换到数据接收状态 */
        }
        else
        {
            /* 主机读操作：主机发送ACK, 从机可以读取主机发送的ACK信号 */

            uint8_t ack = SimulateI2C_READ_SDA();

            if(ack == Bit_RESET)
            {
                SimulateI2C_State = SimulateI2C_STATE_DAT;  /* 接收到 ACK, 继续发送数据 */
            }
            else
            {
                SimulateI2C_State = SimulateI2C_STATE_STO;  /* 接收到NACK, 停止发送数据 */
            }
        }
        break;

     default:
        break;
    }
}


/******************************************************************************
 * @brief       当SCL触发下降沿外部中断时的处理
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void SimulateI2C_SCL_FallHandler(void)
{
    /* SCL为下降沿, 数据可变 */
    switch(SimulateI2C_State)
    {
     case SimulateI2C_STATE_STA:
        /*
         * 检测到START信号后, SCL第一个下降沿表示开始传输Slave Address,
         * 根据数据有效性的规则, 地址的第一位需要等到SCL变为高电平时才可以读取
         * 切换到获取Slave Address的状态, 等待SCL的上升沿触发
         */
        SimulateI2C_State = SimulateI2C_STATE_ADD;

        SimulateI2C_ShiftCounter = 0;  /* 数据移位计数器清零 */
        SimulateI2C_SlaveAddress = 0;  /* SimulateI2C的从机地址清零 */
        SimulateI2C_ReceivedData = 0;  /* SimulateI2C的接收数据清零 */
        break;

     case SimulateI2C_STATE_ADD:
        /*
         * 在主机发送Slave Address的时候, 从机只是读取SDA状态, 进行地址解析, 所以这边没有处理
         */
        break;

     case SimulateI2C_STATE_ADD_ACK:

        /* SCL低电平的时候, 给I2C总线发送地址的应答信号, 状态不发生改变, 等待下一个上升沿将ACK发送出去 */

        SimulateI2C_SDA_SetLevel(0);    /* 将SDA信号拉低, 向主机发送ACK信号 */
        break;

     case SimulateI2C_STATE_DAT:

        /* 在SCL时钟信号的下降沿, SDA信号线处理可变的状态 */

        if((SimulateI2C_SlaveAddress & 0x01) == 0x00)
        {
            /* 主机写操作：将SDA信号线设置成获取状态, 等待下一个SCL上升沿时获取数据位 */
            SimulateI2C_SDA_SetDirection(1);
        }
        else
        {
            /* 主机读操作：根据发送的数据位设置SDA信号线的输出电平, 等待下一个SCL上升沿时发送数据位 */
            if(SimulateI2C_TransmitData & 0x80)
            {
                SimulateI2C_SDA_SetLevel(1);
            }
            else
            {
                SimulateI2C_SDA_SetLevel(0);
            }

            SimulateI2C_TransmitData <<= 1;
            SimulateI2C_ShiftCounter  += 1;
        }
        break;

     case SimulateI2C_STATE_DAT_ACK:

        /* 在第8个SCL时钟信号下降沿的处理 */

        if((SimulateI2C_SlaveAddress & 0x01) == 0x00)
        {
            /* 主机写操作：从机在接收到数据后, 需要给主机一个ACK应答信号, 状态不发生改变, 等待下一个上升沿将ACK发送出去 */

            SimulateI2C_SDA_SetLevel(0);    /* 将SDA信号拉低, 向主机发送ACK信号 */
        }
        else
        {
            /* 主机读操作：从机需要释放当前的SDA信号线, 以便主机发送ACK或NACK给从机, 状态不发生改变, 等待下一个上升沿读取ACK信号 */
            SimulateI2C_SDA_SetDirection(1);
        }
        break;

     default:
        break;
    }
}


/**
  * @brief  当SDA触发上升沿外部中断时的处理
  * @param  None
  * @retval None
  */
void SimulateI2C_SDA_RiseHandler(void)
{
    if(SimulateI2C_READ_SCL() == Bit_SET)   /* SCL为高时,SDA为上升沿：STOP */
    {
        SimulateI2C_State = SimulateI2C_STATE_STO;
    }
    else                            /* SCL为低时,SDA为上升沿：数据的变化 */
    {
    }
}


/**
  * @brief  当SDA触发下降沿外部中断时的处理
  * @param  None
  * @retval None
  */
void SimulateI2C_SDA_FallHandler(void)
{
    if(SimulateI2C_READ_SCL() == Bit_SET)   /* SCL为高时,SDA为下降沿：START */
    {
        SimulateI2C_State = SimulateI2C_STATE_STA;
    }
    else                            /* SCL为低时,SDA为下降沿：数据的变化 */
    {
    }
}


/**
  * @brief  EXTI4_15 interrupt handles.
  * @param  None
  * @retval None
  */
void EXTI4_15_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line6) != RESET)   /* I2C1_SCL */
    {
        if(SimulateI2C_READ_SCL() == Bit_SET)
        {
            SimulateI2C_SCL_RiseHandler();
        }
        else
        {
            SimulateI2C_SCL_FallHandler();
        }

        EXTI_ClearITPendingBit(EXTI_Line6);
    }

    if(EXTI_GetITStatus(EXTI_Line7) != RESET)   /* I2C1_SDA */
    {
        if(SimulateI2C_READ_SDA() == Bit_SET)
        {
            SimulateI2C_SDA_RiseHandler();
        }
        else
        {
            SimulateI2C_SDA_FallHandler();
        }

        EXTI_ClearITPendingBit(EXTI_Line7);
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

