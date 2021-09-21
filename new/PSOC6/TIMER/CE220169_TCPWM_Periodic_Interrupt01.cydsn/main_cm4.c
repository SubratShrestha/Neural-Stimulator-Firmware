/*****************************************************************************
* File Name: main_cm4.c
*
* Version: 1.00
*
* Description: Demonstrates generating a periodic interrupt using the TCPWM
*              component in timer mode. 
*
* Related Document: CE220169.pdf
*
* Hardware Dependency: CY8CKIT-062-BLE PSoC 6 BLE Pioneer Kit
*
******************************************************************************
* Copyright (2017-2019), Cypress Semiconductor Corporation.
******************************************************************************
* This software, including source code, documentation and related materials
* ("Software") is owned by Cypress Semiconductor Corporation (Cypress) and is
* protected by and subject to worldwide patent protection (United States and 
* foreign), United States copyright laws and international treaty provisions. 
* Cypress hereby grants to licensee a personal, non-exclusive, non-transferable
* license to copy, use, modify, create derivative works of, and compile the 
* Cypress source code and derivative works for the sole purpose of creating 
* custom software in support of licensee product, such licensee product to be
* used only in conjunction with Cypress's integrated circuit as specified in the
* applicable agreement. Any reproduction, modification, translation, compilation,
* or representation of this Software except as specified above is prohibited 
* without the express written permission of Cypress.
* 
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, 
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes to the Software without notice. 
* Cypress does not assume any liability arising out of the application or use
* of Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use as critical components in any products 
* where a malfunction or failure may reasonably be expected to result in 
* significant injury or death ("ACTIVE Risk Product"). By including Cypress's 
* product in a ACTIVE Risk Product, the manufacturer of such system or application
* assumes all risk of such use and in doing so indemnifies Cypress against all
* liability. Use of this Software may be limited by and subject to the applicable
* Cypress software license agreement.
*****************************************************************************/

#include "project.h"

#define TIMER_PERIOD_MSEC   1000U   /* Timer period in milliseconds */

/*******************************************************************************
* Function Name: TimerInterruptHandler
********************************************************************************
*
* Summary: Handler function for the timer interrupt that simply toggles the LED.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  None  
*
*******************************************************************************/

void TimerInterruptHandler(void)
{
    /* Clear the terminal count interrupt */
    Cy_TCPWM_ClearInterrupt(Timer_HW, Timer_CNT_NUM, CY_TCPWM_INT_ON_TC);
    
    /* Toggle the LED */
    Cy_GPIO_Inv(LED_PORT, LED_NUM);
}

/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary: This is the main entry for Cortex-M4. This function initializes the
* timer interrupt, starts the TCPWM component, and puts the core into sleep. The 
* component periodically generates an interrupt according to the configured
* period and an LED is toggled inside the ISR. The CPU wakes up whenever the 
* interrupt occurs and enters sleep again after servicing the ISR. 
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  None  
*
*******************************************************************************/

int main(void)
{
    /* Initialize the interrupt vector table with the timer interrupt handler
     * address and assign priority. */
    Cy_SysInt_Init(&isrTimer_cfg, TimerInterruptHandler);
    NVIC_ClearPendingIRQ(isrTimer_cfg.intrSrc);/* Clears the interrupt */
    NVIC_EnableIRQ(isrTimer_cfg.intrSrc); /* Enable the core interrupt */
    __enable_irq(); /* Enable global interrupts. */
    
    /* Start the TCPWM component in timer/counter mode. The return value of the
     * function indicates whether the arguments are valid or not. It is not used
     * here for simplicity. */
    (void)Cy_TCPWM_Counter_Init(Timer_HW, Timer_CNT_NUM, &Timer_config);
    Cy_TCPWM_Enable_Multiple(Timer_HW, Timer_CNT_MASK); /* Enable the counter instance */
    
    /* Set the timer period in milliseconds. To count N cycles, period should be
     * set to N-1. */
    Cy_TCPWM_Counter_SetPeriod(Timer_HW, Timer_CNT_NUM, TIMER_PERIOD_MSEC - 1);
    
    /* Trigger a software reload on the counter instance. This is required when 
     * no other hardware input signal is connected to the component to act as
     * a trigger source. */
    Cy_TCPWM_TriggerReloadOrIndex(Timer_HW, Timer_CNT_MASK); 

    for(;;)
    {
        /* Put the CPU into sleep mode to save power. CPU wakes up whenever 
         * timer interrupt occurs and enters into sleep again after servicing
         * the ISR. */
        Cy_SysPm_Sleep(CY_SYSPM_WAIT_FOR_INTERRUPT);
    }
}

/* [] END OF FILE */
