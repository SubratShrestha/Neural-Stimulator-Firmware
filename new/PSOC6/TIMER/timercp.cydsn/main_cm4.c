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

#include "main.h"

int triggered = 1;

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
    } else if (current_phase == 1) {
        dacWrite = 0x000;   
    } else if (current_phase == 2) {
        dacWrite = 0x7FF;
    } else if (current_phase == 3) {
        dacWrite = 0x000;   
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
    
    
    /* phase timings sample dummy for now */
    PHASE_ONE_TIME = 10; /* defuault to 10us for all values */
    PHASE_TWO_TIME = 10;
    INTER_PHASE_GAP = 40;
    INTER_STIM_DELAY = 200;
    PULSE_NUM = 200; /* default 0 (infinite number of pulses) */
    uint32_t phases[4] = {PHASE_ONE_TIME, PHASE_TWO_TIME, INTER_PHASE_GAP, INTER_STIM_DELAY};

    /* condition checks */
    int curr_num_pulses = 0;
    current_phase = 0;
    
    Cy_TCPWM_TriggerReloadOrIndex(Timer_HW, Timer_CNT_MASK);
    
    /* Command parser - Changes values of global variables to affect the hardware */
    
    /* Command Queue - used to buffer incoming packets via BLE to avoid potential packet loss*/
    
    for(;;)
    {
        /* Place your application code here. */
        
        // execute any commands in the queue
        if (!IsEmptyQueue(&COMMAND_QUEUE)) {
            uint8_t *command_buffer = calloc(5,sizeof(uint8_t));
            DeQueue(&COMMAND_QUEUE, command_buffer);
		    execute_command(command_buffer);
        }
        
        if (triggered) {
            triggered = 0;
            Cy_TCPWM_Counter_SetPeriod(Timer_HW, Timer_CNT_NUM, phases[current_phase] - 1);
            if (current_phase < 3) {
                current_phase++;
            } else if (current_phase == 3) {
                current_phase = 0;
                curr_num_pulses++;
                printf("%d\r\n", curr_num_pulses);
            }
            if (curr_num_pulses == PULSE_NUM && PULSE_NUM != 0) {
                // number of pulses reached stop outputting
                Cy_TCPWM_TriggerStopOrKill(Timer_HW, Timer_CNT_MASK);
                VDAC_1_Stop();
            }
        }
        
 
    }
}

/* [] END OF FILE */
