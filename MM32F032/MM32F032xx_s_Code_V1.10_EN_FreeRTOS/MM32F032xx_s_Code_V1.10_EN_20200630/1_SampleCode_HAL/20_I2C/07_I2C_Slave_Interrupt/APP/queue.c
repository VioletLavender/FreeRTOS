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
#define __QUEUE_C__


/* Includes -----------------------------------------------------------------*/
#include "queue.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/


/* Private variables --------------------------------------------------------*/
static QUEUE_InitTypeDef QUEUE[QUEUE_NUMBER];


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
void QUEUE_INIT(uint8_t index)
{
    QUEUE[index].head = 0;
    QUEUE[index].tail = 0;

    memset(QUEUE[index].data, 0, sizeof(QUEUE[index].data));
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
uint8_t QUEUE_EMPTY(uint8_t index)
{
    if(QUEUE[index].head == QUEUE[index].tail)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
uint8_t QUEUE_READ(uint8_t index)
{
    uint8_t data = QUEUE[index].data[QUEUE[index].head++];

    QUEUE[index].head %= QUEUE_SIZE;

    return data;
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void QUEUE_WRITE(uint8_t index, uint8_t data)
{
    QUEUE[index].data[QUEUE[index].tail++] = data;

    QUEUE[index].tail %= QUEUE_SIZE;
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

