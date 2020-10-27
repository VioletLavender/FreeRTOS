/******************************************************************************
 * @file    rtc.c
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
#define __RTC_C__


/* Includes -----------------------------------------------------------------*/
#include "rtc.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/


/* Private variables --------------------------------------------------------*/
const uint8_t RTC_DayOfMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
CALENDAR_t    RTC_Calendar;


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
uint8_t RTC_LeapYear(uint16_t Year)
{
    if(
        (((Year % 400) == 0)                     ) ||   /* Century Leap Year */
        (((Year % 100) != 0) && ((Year % 4) == 0))      /* Normal  Leay Year */
    )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/******************************************************************************
 * @brief       Calc Week Day By Zeller
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
uint8_t RTC_GetWeek(uint16_t Year, uint8_t Month, uint8_t Day)
{
    int w, c, y;

    /* Month 1 Or 2 of This Year Must Be As Last Month 13 Or 14 */
    if((Month == 1) || (Month == 2))
    {
        Month += 12;
        Year  -= 1;
    }

    w = 0;          /* Weekday */
    c = Year / 100; /* Century */
    y = Year % 100; /* Year    */

    w = y + (y / 4) + (c / 4) - (2 * c) + (26 * (Month + 1) / 10) + Day - 1;

    while(w < 0) w += 7;

    w %= 7;

    return w;
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void RTC_UpdateCalendar(void)
{
    static uint32_t PreTotalDay = 0;

    uint32_t TotalSecond = 0;
    uint32_t TotalDay    = 0;

    uint16_t Year  = 1970;
    uint8_t  Month = 0;

    /* Get The RTC Counter Value */
    TotalSecond = RTC_GetCounter();
    TotalDay    = TotalSecond / 86400;

    if(PreTotalDay != TotalDay)
    {
        PreTotalDay = TotalDay;

        while(TotalDay >= 365)
        {
            if(RTC_LeapYear(Year) == 1)
            {
                if(TotalDay >= 366)
                {
                    TotalDay -= 366;
                }
                else
                {
                    break;
                }
            }
            else
            {
                TotalDay -= 365;
            }

            Year++;
        }

        RTC_Calendar.year = Year;

        while(TotalDay >= 28)
        {
            if((Month == 1) && (RTC_LeapYear(RTC_Calendar.year) == 1))
            {
                if(TotalDay >= 29)
                {
                    TotalDay -= 29;
                }
                else
                {
                    break;
                }
            }
            else
            {
                if(TotalDay >= RTC_DayOfMonth[Month])
                {
                    TotalDay -= RTC_DayOfMonth[Month];
                }
                else
                {
                    break;
                }
            }

            Month++;
        }

        RTC_Calendar.month = Month    + 1;
        RTC_Calendar.day   = TotalDay + 1;

        RTC_Calendar.week  = RTC_GetWeek(RTC_Calendar.year, RTC_Calendar.month, RTC_Calendar.day);
    }

    RTC_Calendar.hour   =  (TotalSecond % 86400) / 3600;
    RTC_Calendar.minute = ((TotalSecond % 86400) % 3600) / 60;
    RTC_Calendar.second = ((TotalSecond % 86400) % 3600) % 60;
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void RTC_SetDateTime(uint16_t Year, uint8_t Month, uint8_t Day,
                     uint8_t  Hour, uint8_t Min,   uint8_t Sec)
{
    uint32_t TotalSecond = 0;

    uint16_t y = 0;
    uint8_t  m = 0;

    if((Year >= 1970) && (Year <= 2099))
    {
        for(y = 1970;  y < Year; y++)
        {
            if(RTC_LeapYear(y) == 1)
            {
                TotalSecond += 31622400;    /* Total Seconds Of Leap   Year */
            }
            else
            {
                TotalSecond += 31536000;    /* Total Seconds Of Normal Year */
            }
        }

        for(m = 0; m < (Month - 1); m++)
        {
            TotalSecond += RTC_DayOfMonth[m] * 86400;   /* Total Seconds Of Month */

            if((RTC_LeapYear(Year) == 1) && (m == 1))
            {
                TotalSecond += 86400;
            }
        }

        TotalSecond += (uint32_t)(Day - 1) * 86400; /* Total Seconds Of Day    */
        TotalSecond += (uint32_t)Hour      * 3600;  /* Total Seconds Of Hour   */
        TotalSecond += (uint32_t)Min       * 60;    /* Total Seconds Of Minute */
        TotalSecond += Sec;

        /* Enable PWR Clock */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

        /* Enable Access To The RTC & Backup Registers */
        PWR_BackupAccessCmd(ENABLE);

        /* Set The RTC Counter Value */
        RTC_SetCounter(TotalSecond);

        /* Wait Until Last Write Operation On RTC REG Has Finished */
        RTC_WaitForLastTask();

        RTC_UpdateCalendar();
    }
    else
    {
        printf("\r\nError Date & Time!!!\r\n");
    }
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void RTC_PrintDateTime(void)
{
    printf("\r\n%04d-%02d-%02d", RTC_Calendar.year, RTC_Calendar.month,  RTC_Calendar.day);

    switch(RTC_Calendar.week)
    {
     case 0 :
        printf(" SUN ");
        break;

     case 1 :
        printf(" MON ");
        break;

     case 2 :
        printf(" TUE ");
        break;

     case 3 :
        printf(" WED ");
        break;

     case 4 :
        printf(" THU ");
        break;

     case 5 :
        printf(" FRI ");
        break;

     case 6 :
        printf(" SAT ");
        break;

     default:
        break;
    }

    printf("%02d:%02d:%02d\r\n", RTC_Calendar.hour, RTC_Calendar.minute, RTC_Calendar.second);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void RTC_Configure(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable PWR Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Enable WKUP pin */
    PWR_WakeUpPinCmd(ENABLE);

    /* Enable Access To The RTC & Backup Registers */
    PWR_BackupAccessCmd(ENABLE);

    if(BKP_ReadBackupRegister(BKP_DR1) != 0x5A5A)
    {
        BKP_DeInit();

        /* Enable LSI Clock Source */
        RCC_LSICmd(ENABLE);

        /* Wait LSI Clock Source Ready */
        while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

        /* Config RTC Clock Source : LSI */
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

        /* Enable RTC Clock */
        RCC_RTCCLKCmd(ENABLE);

        /* Wait For Synchronization */
        RTC_WaitForSynchro();
        /* Wait Until Last Write Operation On RTC REG Has Finished */
        RTC_WaitForLastTask();

        /* Set The RTC Prescaler Value */
        RTC_SetPrescaler(32767);
        /* Wait Until Last Write Operation On RTC REG Has Finished */
        RTC_WaitForLastTask();

        /* Enable RTC Second Interrupt */
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        /* Wait Until Last Write Operation On RTC REG Has Finished */
        RTC_WaitForLastTask();

        /* Exit From The RTC Configuration Mode */
        RTC_ExitConfigMode();

        BKP_WriteBackupRegister(BKP_DR1, 0x5A5A);
        /* Wait Until Last Write Operation On RTC REG Has Finished */
        RTC_WaitForLastTask();
    }
    else
    {
        /* Wait For Synchronization */
        RTC_WaitForSynchro();

        /* Enable RTC Second Interrupt */
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        /* Wait Until Last Write Operation On RTC REG Has Finished */
        RTC_WaitForLastTask();
    }

    /* Config RTC NVIC */
    NVIC_InitStructure.NVIC_IRQChannel  = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Set initial time : 2020-05-20 10:56:10 */
    RTC_SetDateTime(2020, 5, 20, 10, 56, 10);
}


/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void RTC_BKP_IRQHandler(void)
{
    if(RTC_GetITStatus(RTC_IT_SEC) != RESET)
    {
        /* Update Date and Time */
        RTC_UpdateCalendar();

        /* Print current Date and Time */
        RTC_PrintDateTime();

        /* Clear Alarm Flag */
        RTC_ClearITPendingBit(RTC_IT_SEC);
        /* Wait Until Last Write Operation On RTC REG Has Finished */
        RTC_WaitForLastTask();
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

