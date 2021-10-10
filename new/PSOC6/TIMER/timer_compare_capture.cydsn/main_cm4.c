im/* ========================================
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

int triggered = 1;
uint32_t dacWrite = 0x800;
int current_phase = 0;
uint32_t phase_1 = 10; /* defuault to 10us for all values */
uint32_t phase_2 = 10;
uint32_t inter_phase_gap = 1;
uint32_t inter_stim_delay = 10;
int num_pulses = 0;

void TimerInterruptHandler(void)
{
    /* Clear the terminal count interrupt */
    Cy_TCPWM_ClearInterrupt(Timer_HW, Timer_CNT_NUM, CY_TCPWM_INT_ON_TC);
    
    /* Toggle the LED */
    triggered = 1;
 
    printf("in the interrupt\r\n");
    VDAC_1_SetValue(dacWrite);
    if (current_phase == 0) {
        dacWrite = 0x800;
        current_phase = 1;
        // Cy_TCPWM_Counter_SetPeriod(Timer_HW, Timer_CNT_NUM, phase_1 - 1);
    } else if (current_phase == 1) {
        dacWrite = 0x000;
        current_phase = 2;
        // Cy_TCPWM_Counter_SetPeriod(Timer_HW, Timer_CNT_NUM, inter_phase_gap - 1);
    } else if (current_phase == 2) {
        dacWrite = 0x7FF;
        current_phase = 3;
        // Cy_TCPWM_Counter_SetPeriod(Timer_HW, Timer_CNT_NUM, phase_2 - 1);
    } else if (current_phase == 3) {
        dacWrite = 0x000;   
        current_phase = 0;
        // Cy_TCPWM_Counter_SetPeriod(Timer_HW, Timer_CNT_NUM, inter_stim_delay - 1);
    }
}


int main(void)
{
    /* Initialize the interrupt vector table with the timer interrupt handler
     * address and assign priority. */
    Cy_SysInt_Init(&isrTimer_cfg, TimerInterruptHandler);
    NVIC_ClearPendingIRQ(isrTimer_cfg.intrSrc);/* Clears the interrupt */
    NVIC_EnableIRQ(isrTimer_cfg.intrSrc); /* Enable the core interrupt */
    __enable_irq(); /* Enable global interrupts. */
    
    VDAC_1_Start();
    UART_Start();
    setvbuf ( stdin, NULL, _IONBF, 0 );
    /* Start the TCPWM component in timer/counter mode. The return value of the
     * function indicates whether the arguments are valid or not. It is not used
     * here for simplicity. */
    (void)Cy_TCPWM_Counter_Init(Timer_HW, Timer_CNT_NUM, &Timer_config);
    Cy_TCPWM_Enable_Multiple(Timer_HW, Timer_CNT_MASK); /* Enable the counter instance */
    Cy_TCPWM_Counter_SetPeriod(Timer_HW, Timer_CNT_NUM, 10 - 1);
    
    /* phase timings */

    /* condition checks */
    int curr_num_pulses = 0;
    
    Cy_TCPWM_TriggerReloadOrIndex(Timer_HW, Timer_CNT_MASK);
    for(;;)
    {
        /* Place your application code here. */
        
 
    }
}

/* [] END OF FILE */
