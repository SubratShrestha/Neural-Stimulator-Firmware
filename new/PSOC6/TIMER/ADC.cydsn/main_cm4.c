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
#include "inttypes.h"
#include "stdint.h"


uint32_t dacWrite = 0x000;
uint32_t comparevalue = 0;
uint32_t phase_1 = 1;
uint32_t inter_stim_gap = 1;
uint32_t phase_2 = 1;
uint32_t inter_stim_delay = 1;
uint32_t phases[4];
int num_pulses = 0; /* could need to change to uint32_t based on how data format is sent/recieved */
int curr_num_pulses = 0;
int current_phase = 0;

void TimerInterruptHandler(void)
{
    Cy_TCPWM_ClearInterrupt(Timer_HW, Timer_CNT_NUM, CY_TCPWM_INT_ON_CC);
    comparevalue = Cy_TCPWM_Counter_GetCounter(Timer_HW, Timer_CNT_NUM);
    
    if (current_phase == 0) {//curren tphase 1
        dacWrite = 0xFFF;   
    } else if (current_phase == 1) { //current phase
        dacWrite = 0x000;   
    } else if (current_phase == 2) {
        dacWrite = 0xFFF;
    } else if (current_phase == 3) {
        dacWrite = 0x800;   
    }
    VDAC_1_SetValue(dacWrite);
    ///ADC IS HERE << ADC start in main
    
    
    Cy_SAR_StartConvert(SAR, CY_SAR_START_CONVERT_SINGLE_SHOT);
    
    volatile float value = Cy_SAR_GetResult16(SAR, 0);
    volatile float volts = Cy_SAR_CountsTo_Volts(SAR, 0, value);
    printf("Value in V = %f\r\n", volts);
    
    ///ADC STOPS
    
    //current phase goes 0,1,2,3 : phase_1, inter_stim_gap, phase_2, inter_stim_delay
    while (1) {
        if (phases[current_phase] == 0) {
            if (current_phase == 3) {
                current_phase = 0;
                curr_num_pulses++;
            } else if (current_phase < 3) {
                current_phase++;
            }
        } else {
            break;
        }
    }
    
    Cy_TCPWM_Counter_SetCompare0(Timer_HW, Timer_CNT_NUM, phases[current_phase] - 1);
    
    if (current_phase == 3) {
        curr_num_pulses++;
        current_phase = 0;   
    } else {
        current_phase++;
    }
    
    Cy_TCPWM_TriggerReloadOrIndex(Timer_HW, Timer_CNT_MASK);
    if (curr_num_pulses == num_pulses && num_pulses != 0) {
        Cy_TCPWM_TriggerStopOrKill(Timer_HW, Timer_CNT_MASK);   
    }
}

int main(void)
{
    Cy_SysInt_Init(&isrTimer_cfg, TimerInterruptHandler);
    NVIC_ClearPendingIRQ(isrTimer_cfg.intrSrc);/* Clears the interrupt */
    NVIC_EnableIRQ(isrTimer_cfg.intrSrc); /* Enable the core interrupt */
    __enable_irq(); /* Enable global interrupts. */
    
    VDAC_1_Start();
    ///ADC start here
    ADC_Start();
    UART_1_Start();
    setvbuf ( stdin, NULL, _IONBF, 0 );
    /* Start the TCPWM component in timer/counter mode. The return value of the
     * function indicates whether the arguments are valid or not. It is not used
     * here for simplicity. */
    (void)Cy_TCPWM_Counter_Init(Timer_HW, Timer_CNT_NUM, &Timer_config);
    Cy_TCPWM_Enable_Multiple(Timer_HW, Timer_CNT_MASK); /* Enable the counter instance */

    phases[0] = phase_1;
    phases[1] = inter_stim_gap;
    phases[2] = phase_2;
    phases[3] = inter_stim_delay;
  
    
    Cy_TCPWM_Counter_SetCompare0(Timer_HW, Timer_CNT_NUM, phases[0]);
    Cy_TCPWM_TriggerReloadOrIndex(Timer_HW, Timer_CNT_MASK);
    
    
    
    
    /*
    int i = 0;
    while (i<600)
    {
        volatile float value = Cy_SAR_GetResult16(SAR, 0);
        volatile float volts = Cy_SAR_CountsTo_Volts(SAR, 0, value);
        printf("Value in V = %f\r\n", volts);
        i++;
    } */
    for(;;)
    {
        /* Place your application code here. */
        //volatile float value = Cy_SAR_GetResult16(SAR, 0);
        //volatile float volts = Cy_SAR_CountsTo_Volts(SAR, 0, value);
        //printf("value in mv = %f\n", volts);
        //printf("%" PRIu32 "\r\n", comparevalue);
    }
}

/* [] END OF FILE */














