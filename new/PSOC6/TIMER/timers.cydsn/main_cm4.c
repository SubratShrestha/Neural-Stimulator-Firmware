/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <stdio.h>


#define MY_TCPWM_CNT_1          (1UL)
#define MY_TCPWM_CNT_5          (5UL)
#define MY_TCPWM_CNT_6          (6UL)
#define MY_TCPWM_CNT_MULT_MASK  (uint32_t)((1UL << MY_TCPWM_CNT_1) |\
                                           (1UL << MY_TCPWM_CNT_5) |\
                                           (1UL << MY_TCPWM_CNT_6))
#define MY_TCPWM_CNT_NUM (0UL)                                        


void TimerInterruptHandler(void)
{
    /* Clear the terminal count interrupt */
    Cy_TCPWM_ClearInterrupt(Timer_HW, Timer_CNT_NUM, CY_TCPWM_INT_ON_TC);
    
    /* Toggle the LED */
    printf("hello world\r\n");
}


int main(void)
{
    Cy_SysInt_Init(&isrTimer_cfg, TimerInterruptHandler);
    NVIC_ClearPendingIRQ(isrTimer_cfg.intrSrc);/* Clears the interrupt */
    NVIC_EnableIRQ(isrTimer_cfg.intrSrc); /* Enable the core interrupt */
    __enable_irq(); /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    UART_Start();
    setvbuf ( stdin, NULL, _IONBF, 0 );
    
    /* Start the TCPWM component in timer/counter mode. The return value of the
     * function indicates whether the arguments are valid or not. It is not used
     * here for simplicity. */
    (void)Cy_TCPWM_Counter_Init(Timer_HW, Timer_CNT_NUM, &Timer_config);
    Cy_TCPWM_Enable_Multiple(Timer_HW, MY_TCPWM_CNT_MULT_MASK); /* Enable the counter instance */
    Cy_TCPWM_TriggerStart(Timer_HW, MY_TCPWM_CNT_MULT_MASK);
    Cy_TCPWM_SetInterrupt(Timer_HW, MY_TCPWM_CNT_NUM, CY_TCPWM_INT_ON_CC);
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
