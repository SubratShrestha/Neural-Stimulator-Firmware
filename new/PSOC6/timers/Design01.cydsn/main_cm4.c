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
#include <stdlib.h>

#define TIMER_PERIOD_MSEC   1000U   /* Timer period in milliseconds */


void TimerInterruptHandler(void)
{
    /* Clear the terminal count interrupt */
    Cy_TCPWM_ClearInterrupt(Timer_HW, Timer_CNT_NUM, CY_TCPWM_INT_ON_TC);
    
    /* Print to output */
    printf("Hello\r\n");
    
    /* Trigger a software reload on the counter instance. This is required when 
     * no other hardware input signal is connected to the component to act as
     * a trigger source. */
    Cy_TCPWM_TriggerReloadOrIndex(Timer_1_HW, Timer_1_CNT_MASK);
}


void TimerInterruptHandler_1(void)
{
    /* Clear the terminal count interrupt */
    Cy_TCPWM_ClearInterrupt(Timer_1_HW, Timer_1_CNT_NUM, CY_TCPWM_INT_ON_TC);
    
    /* Print to output */
    printf("bad\r\n");
    
    /* Trigger a software reload on the counter instance. This is required when 
     * no other hardware input signal is connected to the component to act as
     * a trigger source. */
    Cy_TCPWM_TriggerReloadOrIndex(Timer_HW, Timer_CNT_MASK); 
    
}


int main(void)
{
    /* Initialize the interrupt vector table with the timer interrupt handler
     * address and assign priority. */
    Cy_SysInt_Init(&isrTimer_cfg, TimerInterruptHandler);
    NVIC_ClearPendingIRQ(isrTimer_cfg.intrSrc);/* Clears the interrupt */
    NVIC_EnableIRQ(isrTimer_cfg.intrSrc); /* Enable the core interrupt */
    
    Cy_SysInt_Init(&isrTimer_1_cfg, TimerInterruptHandler_1);
    NVIC_ClearPendingIRQ(isrTimer_1_cfg.intrSrc);/* Clears the interrupt */
    NVIC_EnableIRQ(isrTimer_1_cfg.intrSrc); /* Enable the core interrupt */
    
    __enable_irq(); /* Enable global interrupts. */
    
    
    UART_Start();
    setvbuf ( stdin, NULL, _IONBF, 0);
    /* Start the TCPWM component in timer/counter mode. The return value of the
     * function indicates whether the arguments are valid or not. It is not used
     * here for simplicity. */
    (void)Cy_TCPWM_Counter_Init(Timer_HW, Timer_CNT_NUM, &Timer_config);
    Cy_TCPWM_Enable_Multiple(Timer_HW, Timer_CNT_MASK); /* Enable the counter instance */
    
    
    (void)Cy_TCPWM_Counter_Init(Timer_1_HW, Timer_1_CNT_NUM, &Timer_1_config);
    Cy_TCPWM_Enable_Multiple(Timer_1_HW, Timer_1_CNT_MASK); /* Enable the counter instance */
    
    /* Set the timer period in milliseconds. To count N cycles, period should be
     * set to N-1. */
    Cy_TCPWM_Counter_SetPeriod(Timer_HW, Timer_CNT_NUM, TIMER_PERIOD_MSEC - 1);
    Cy_TCPWM_Counter_SetPeriod(Timer_1_HW, Timer_1_CNT_NUM, TIMER_PERIOD_MSEC -1);

    Cy_TCPWM_TriggerReloadOrIndex(Timer_HW, Timer_CNT_MASK);
    Cy_TCPWM_TriggerReloadOrIndex(Timer_1_HW, Timer_1_CNT_MASK);
    for(;;)
    {
        /* Put the CPU into sleep mode to save power. CPU wakes up whenever 
         * timer interrupt occurs and enters into sleep again after servicing
         * the ISR. */
        Cy_SysPm_Sleep(CY_SYSPM_WAIT_FOR_INTERRUPT);
    }
}

/* [] END OF FILE */
