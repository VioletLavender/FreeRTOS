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
 * @brief       ����ģ��I2C��GPIO�˿�, Ĭ�����ó�����ģʽ, ��ʹ����Ӧ���ⲿ����
 *              �жϹ���(�����غ��½���)
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
 * @brief       ����SDA�ź��ߵ������������, 0����Output���, 1����Input����
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
 * @brief       ����SDA�ź��ߵ������ƽ(�ߵ�ƽ / �͵�ƽ)
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
 * @brief       ��SCL�����������ⲿ�ж�ʱ�Ĵ���
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void SimulateI2C_SCL_RiseHandler(void)
{
    /* SCLΪ������, ��������, ���ӻ���SDA�����ϻ�ȡ����λ */
    switch(SimulateI2C_State)
    {
     case SimulateI2C_STATE_ADD:

        /* I2C��������MSB, �ȷ��͸�λ, �ٷ��͵�λ, �����ڽ��յ�ʱ��, ���ݽ������� */

        SimulateI2C_SlaveAddress <<= 1;
        SimulateI2C_ShiftCounter  += 1;

        if(SimulateI2C_READ_SDA() ==  Bit_SET)
        {
            SimulateI2C_SlaveAddress |= 0x01;
        }

        /* �����յ�8λ��ַλ��, �ӻ���Ҫ�ڵ�9��ʱ�Ӹ���ACKӦ��, �ȴ�SCL�½��ص�ʱ�����ACK�ź� */
        if(SimulateI2C_ShiftCounter == 8)
        {
            SimulateI2C_State = SimulateI2C_STATE_ADD_ACK;
        }
        break;

     case SimulateI2C_STATE_ADD_ACK:
        /* �ӻ���ַ��ACK�ظ���, �л����շ�����״̬ */
        SimulateI2C_State = SimulateI2C_STATE_DAT;

        SimulateI2C_ShiftCounter = 0;   /* ������λ���������� */
        SimulateI2C_ReceivedData = 0;   /* SimulateI2C�Ľ����������� */
        break;

     case SimulateI2C_STATE_DAT:
        if((SimulateI2C_SlaveAddress & 0x01) == 0x00)
        {
            /* ����д��������ʱ�ӻ�Ӧ�û�ȡ�������͵�SDA�ź��ߵ�ƽ״̬, ����λ�洢 */
            SimulateI2C_ReceivedData <<= 1;
            SimulateI2C_ShiftCounter  += 1;

            if(SimulateI2C_READ_SDA() == Bit_SET)
            {
                SimulateI2C_ReceivedData |= 0x01;
            }

            /* ���յ�һ��������8λ����ʱ, ���յ������ݴ�ŵ�I2C������Ϣ������, ״̬ת��������������ACKӦ�� */
            if(SimulateI2C_ShiftCounter == 8)
            {
                QUEUE_WRITE(QUEUE_I2C_RX_IDX, SimulateI2C_ReceivedData);

                SimulateI2C_ShiftCounter = 0;  /* ������λ���������� */
                SimulateI2C_ReceivedData = 0;  /* SimulateI2C�Ľ����������� */

                SimulateI2C_State = SimulateI2C_STATE_DAT_ACK;
            }
        }
        else
        {
            /* ������������ ��SCL�����ص�ʱ��, ������ȡ��ǰSDA��״̬λ, ������˵�8����λ��������,
             * �ǽ��������������ظ��ӻ���Ӧ����Ӧ���ź���, ���Խ�״̬�л����ȴ�ACK��״̬, ͬʱ׼����һ����Ҫ���͵�����
             */
            if(SimulateI2C_ShiftCounter == 8)
            {
                SimulateI2C_ShiftCounter = 0;      /* SimulateI2C�Ľ����������� */
                SimulateI2C_TransmitData = SimulateI2C_TransmitBuffer[SimulateI2C_TransmitIndex++];

                SimulateI2C_TransmitIndex %= 16;

                SimulateI2C_State = SimulateI2C_STATE_DAT_ACK;
            }
        }
        break;

     case SimulateI2C_STATE_DAT_ACK:
        if((SimulateI2C_SlaveAddress & 0x01) == 0x00)
        {
            /* ����д�������ӻ�����ACK, �ȴ�������ȡ�ӻ����͵�ACK�ź� */

            SimulateI2C_State = SimulateI2C_STATE_DAT;  /* ״̬�л������ݽ���״̬ */
        }
        else
        {
            /* ��������������������ACK, �ӻ����Զ�ȡ�������͵�ACK�ź� */

            uint8_t ack = SimulateI2C_READ_SDA();

            if(ack == Bit_RESET)
            {
                SimulateI2C_State = SimulateI2C_STATE_DAT;  /* ���յ� ACK, ������������ */
            }
            else
            {
                SimulateI2C_State = SimulateI2C_STATE_STO;  /* ���յ�NACK, ֹͣ�������� */
            }
        }
        break;

     default:
        break;
    }
}


/******************************************************************************
 * @brief       ��SCL�����½����ⲿ�ж�ʱ�Ĵ���
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void SimulateI2C_SCL_FallHandler(void)
{
    /* SCLΪ�½���, ���ݿɱ� */
    switch(SimulateI2C_State)
    {
     case SimulateI2C_STATE_STA:
        /*
         * ��⵽START�źź�, SCL��һ���½��ر�ʾ��ʼ����Slave Address,
         * ����������Ч�ԵĹ���, ��ַ�ĵ�һλ��Ҫ�ȵ�SCL��Ϊ�ߵ�ƽʱ�ſ��Զ�ȡ
         * �л�����ȡSlave Address��״̬, �ȴ�SCL�������ش���
         */
        SimulateI2C_State = SimulateI2C_STATE_ADD;

        SimulateI2C_ShiftCounter = 0;  /* ������λ���������� */
        SimulateI2C_SlaveAddress = 0;  /* SimulateI2C�Ĵӻ���ַ���� */
        SimulateI2C_ReceivedData = 0;  /* SimulateI2C�Ľ����������� */
        break;

     case SimulateI2C_STATE_ADD:
        /*
         * ����������Slave Address��ʱ��, �ӻ�ֻ�Ƕ�ȡSDA״̬, ���е�ַ����, �������û�д���
         */
        break;

     case SimulateI2C_STATE_ADD_ACK:

        /* SCL�͵�ƽ��ʱ��, ��I2C���߷��͵�ַ��Ӧ���ź�, ״̬�������ı�, �ȴ���һ�������ؽ�ACK���ͳ�ȥ */

        SimulateI2C_SDA_SetLevel(0);    /* ��SDA�ź�����, ����������ACK�ź� */
        break;

     case SimulateI2C_STATE_DAT:

        /* ��SCLʱ���źŵ��½���, SDA�ź��ߴ���ɱ��״̬ */

        if((SimulateI2C_SlaveAddress & 0x01) == 0x00)
        {
            /* ����д��������SDA�ź������óɻ�ȡ״̬, �ȴ���һ��SCL������ʱ��ȡ����λ */
            SimulateI2C_SDA_SetDirection(1);
        }
        else
        {
            /* ���������������ݷ��͵�����λ����SDA�ź��ߵ������ƽ, �ȴ���һ��SCL������ʱ��������λ */
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

        /* �ڵ�8��SCLʱ���ź��½��صĴ��� */

        if((SimulateI2C_SlaveAddress & 0x01) == 0x00)
        {
            /* ����д�������ӻ��ڽ��յ����ݺ�, ��Ҫ������һ��ACKӦ���ź�, ״̬�������ı�, �ȴ���һ�������ؽ�ACK���ͳ�ȥ */

            SimulateI2C_SDA_SetLevel(0);    /* ��SDA�ź�����, ����������ACK�ź� */
        }
        else
        {
            /* �������������ӻ���Ҫ�ͷŵ�ǰ��SDA�ź���, �Ա���������ACK��NACK���ӻ�, ״̬�������ı�, �ȴ���һ�������ض�ȡACK�ź� */
            SimulateI2C_SDA_SetDirection(1);
        }
        break;

     default:
        break;
    }
}


/**
  * @brief  ��SDA�����������ⲿ�ж�ʱ�Ĵ���
  * @param  None
  * @retval None
  */
void SimulateI2C_SDA_RiseHandler(void)
{
    if(SimulateI2C_READ_SCL() == Bit_SET)   /* SCLΪ��ʱ,SDAΪ�����أ�STOP */
    {
        SimulateI2C_State = SimulateI2C_STATE_STO;
    }
    else                            /* SCLΪ��ʱ,SDAΪ�����أ����ݵı仯 */
    {
    }
}


/**
  * @brief  ��SDA�����½����ⲿ�ж�ʱ�Ĵ���
  * @param  None
  * @retval None
  */
void SimulateI2C_SDA_FallHandler(void)
{
    if(SimulateI2C_READ_SCL() == Bit_SET)   /* SCLΪ��ʱ,SDAΪ�½��أ�START */
    {
        SimulateI2C_State = SimulateI2C_STATE_STA;
    }
    else                            /* SCLΪ��ʱ,SDAΪ�½��أ����ݵı仯 */
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

