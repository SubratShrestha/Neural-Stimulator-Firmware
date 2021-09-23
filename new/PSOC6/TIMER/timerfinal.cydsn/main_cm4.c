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

int triggered = 0;

void TimerInterruptHandler(void)
{
    /* Clear the terminal count interrupt */
    Cy_TCPWM_ClearInterrupt(Timer_HW, Timer_CNT_NUM, CY_TCPWM_INT_ON_TC);
    
    /* Toggle the LED */
    triggered = 1;
 
    printf("in the interrupt\r\n");
}


int main(void)
{
    /* Initialize the interrupt vector table with the timer interrupt handler
     * address and assign priority. */
    Cy_SysInt_Init(&isrTimer_cfg, TimerInterruptHandler);
    NVIC_ClearPendingIRQ(isrTimer_cfg.intrSrc);/* Clears the interrupt */
    NVIC_EnableIRQ(isrTimer_cfg.intrSrc); /* Enable the core interrupt */
    __enable_irq(); /* Enable global interrupts. */
    
    UART_Start();
    setvbuf ( stdin, NULL, _IONBF, 0 );
    /* Start the TCPWM component in timer/counter mode. The return value of the
     * function indicates whether the arguments are valid or not. It is not used
     * here for simplicity. */
    (void)Cy_TCPWM_Counter_Init(Timer_HW, Timer_CNT_NUM, &Timer_config);
    Cy_TCPWM_Enable_Multiple(Timer_HW, Timer_CNT_MASK); /* Enable the counter instance */
    
    
    /* phase timings */
    uint32_t phase_1 = 5000; /* defuault to 10us for all values */
    uint32_t phase_2 = 1000;
    uint32_t inter_phase_gap = 500;
    uint32_t inter_stim_delay = 100;
    int num_pulses = 10;
    uint32_t phases[4] = {phase_1, phase_2, inter_phase_gap, inter_stim_delay};

    /* condition checks */
    int current_phase = 0;
    int curr_num_pulses = 0;
    
    /* Set the timer period in milliseconds. To count N cycles, period should be
     * set to N-1. */
    Cy_TCPWM_Counter_SetPeriod(Timer_HW, Timer_CNT_NUM, phases[0] - 1); // timer is in miliseconds
    
    /* Trigger a software reload on the counter instance. This is required when 
     * no other hardware input signal is connected to the component to act as
     * a trigger source. */
    Cy_TCPWM_TriggerReloadOrIndex(Timer_HW, Timer_CNT_MASK);
    for(;;)
    {
        /* Place your application code here. */
        if (triggered) {
            if (current_phase == 3) {
                current_phase = 0;
                curr_num_pulses++;
                printf("%d\r\n", curr_num_pulses);
            } else if (current_phase != 3) {
                current_phase++;
            }
            Cy_TCPWM_Counter_SetPeriod(Timer_HW, Timer_CNT_NUM, phases[current_phase] - 1);
            Cy_TCPWM_TriggerReloadOrIndex(Timer_HW, Timer_CNT_MASK);
            triggered = 0;
            if (curr_num_pulses == num_pulses && num_pulses != 0) {
                // number of pulses reached stop outputting
                Cy_TCPWM_TriggerStopOrKill(Timer_HW, Timer_CNT_MASK);
            }
        }
 
    }
}

/* [] END OF FILE */
