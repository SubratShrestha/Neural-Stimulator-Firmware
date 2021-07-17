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
#include "FreeRTOS.h"
#include "task.h"

// Make an RTOS task which makes the dac send biphasic pulses

void start_dac(void *p1/*, void *p2, void *p3, void *p4*/) {
    /* uint16_t *phase1; // time stuck in phase 1
    uint16_t *phase2; // time stuck in phase 2
    uint16_t *inter_stim_delay; // delay between phase 1 and phase 2
    uint16_t *stim_gap; // time delay between biphasic stimulation
    phase1 = (uint16_t *) p1;
    phase2 = (uint16_t *) p2;
    inter_stim_delay = (uint16_t *) p3;
    stim_gap = (uint16_t *) p4; */
    
    uint16_t phase1 = 15;
    uint16_t phase2 = 15;
    uint16_t inter_stim_delay = 15;
    uint16_t stim_gap = 50;
    
    // continous biphasic
    for (;;) {
        // delay in microseconds
        CyDelayUs(stim_gap);

        Cy_CTDAC_SetValueBuffered(VDAC_CTDAC_HW, 0x800);
        Cy_CTDAC_ClearInterrupt(VDAC_CTDAC_HW);

        Cy_CTDAC_SetValueBuffered(VDAC_CTDAC_HW, -0x7FF);
        CyDelayUs(phase1);
        Cy_CTDAC_ClearInterrupt(VDAC_CTDAC_HW);

        // Setting back to 0
        Cy_CTDAC_SetValueBuffered(VDAC_CTDAC_HW, 0x800);
        CyDelayUs(inter_stim_delay);
        Cy_CTDAC_ClearInterrupt(VDAC_CTDAC_HW);

        // Setting to positive (phase 2)
        Cy_CTDAC_SetValueBuffered(VDAC_CTDAC_HW, 0x7FF);
        CyDelayUs(phase2); // phase 2 time
        Cy_CTDAC_ClearInterrupt(VDAC_CTDAC_HW);

        // Setting back to 0
        Cy_CTDAC_SetValueBuffered(VDAC_CTDAC_HW, 0x800);
        Cy_CTDAC_ClearInterrupt(VDAC_CTDAC_HW);

        CyDelayUs(stim_gap); // This could be our stimulation gap
    }
    
    
}

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    UART_Start();
    setvbuf (stdin, NULL, _IONBF, 0);
    // initialise the dac -- Customisability on datasheet ~ leave for now
    VDAC_Start();
    
    //////////////////////////////////////////////
    //              biphasic pulse              //
    //////////////////////////////////////////////
    
    // delay in microseconds
    CyDelayUs(500);
    
    // 0x800 sets DAC to 0 for 12-bit two's complement
    Cy_CTDAC_SetValueBuffered(VDAC_CTDAC_HW, 0x800);
    Cy_CTDAC_ClearInterrupt(VDAC_CTDAC_HW);
    
    
    // 0x7FF sets to =~ vref value (3) just below it
    // going in the negative (phase 1) lasts approx 10 u sec
    // Not sure about -0x7FF
    Cy_CTDAC_SetValueBuffered(VDAC_CTDAC_HW, -0x7FF);
    CyDelayUs(10);
    Cy_CTDAC_ClearInterrupt(VDAC_CTDAC_HW);
   
    // Setting back to 0
    Cy_CTDAC_SetValueBuffered(VDAC_CTDAC_HW, 0x800);
    // delay here for inter_phase_gap
    Cy_CTDAC_ClearInterrupt(VDAC_CTDAC_HW);
    
    // Setting to positive (phase 2)
    Cy_CTDAC_SetValueBuffered(VDAC_CTDAC_HW, 0x7FF);
    CyDelayUs(10); // phase 2 time
    Cy_CTDAC_ClearInterrupt(VDAC_CTDAC_HW);
    
    // Setting back to 0
    Cy_CTDAC_SetValueBuffered(VDAC_CTDAC_HW, 0x800);
    Cy_CTDAC_ClearInterrupt(VDAC_CTDAC_HW);

    CyDelayUs(500); // This could be our stimulation gap

    
    CyDelay(500);
    
    //////////////////////////////////////////////////////////
    // interphase gap (IPG) -- approx ~ 10u sec in our case //
    //////////////////////////////////////////////////////////
    
    // delay in microseconds
    CyDelayUs(500);
    
    Cy_CTDAC_SetValueBuffered(VDAC_CTDAC_HW, 0x800);
    Cy_CTDAC_ClearInterrupt(VDAC_CTDAC_HW);
    
    // phase 1
    Cy_CTDAC_SetValueBuffered(VDAC_CTDAC_HW, -0x7FF);
    CyDelayUs(10);
    Cy_CTDAC_ClearInterrupt(VDAC_CTDAC_HW);
   
    // Setting back to 0
    Cy_CTDAC_SetValueBuffered(VDAC_CTDAC_HW, 0x800);
    CyDelayUs(10); // default inter_stim_delay = 10 us
    Cy_CTDAC_ClearInterrupt(VDAC_CTDAC_HW);
    
    // phase 2
    Cy_CTDAC_SetValueBuffered(VDAC_CTDAC_HW, 0x7FF);
    CyDelayUs(10);
    Cy_CTDAC_ClearInterrupt(VDAC_CTDAC_HW);
    
    // Setting back to 0
    Cy_CTDAC_SetValueBuffered(VDAC_CTDAC_HW, 0x800);
    Cy_CTDAC_ClearInterrupt(VDAC_CTDAC_HW);

    CyDelayUs(500);
    
    
    // DMA????
    // RTOS task for DAC
    xTaskCreate(start_dac, "Start digital-analogue", 400, (void *) 1, 1, NULL);
    vTaskStartScheduler();
    
    
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
