/*****************************************************************************
* File Name: main_cm0p.c
*
* Version: 1.10
*
* Description: Demonstrates blinking an LED under firmware control. This is done
* by the Cortex-M0+ in a dual-CPU PSoC 6, while at the same time the Cortex-M4
* in the same device blinks a different LED at a different rate.
*
* Note: The main.c files for both of the CPUs in the PSoC 6 are part of the
* same PSoC Creator project. They are both compiled at project build time.
* The resultant .hex file has the code for both CPUs. When the project
* executes, the code in main_cm0p.c starts executing first. The Cortex-M0+
* code then turns on the Cortex-M4 CPU, and the code in main_cm4.c starts
* executing.
*
* Related Document: Code example CE216795
*
* Hardware Dependency: See code example CE216795
*
******************************************************************************
* Copyright (2017), Cypress Semiconductor Corporation.
******************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress) and is
* protected by and subject to worldwide patent protection (United States and
* foreign), United States copyright laws and international treaty provisions.
* Cypress hereby grants to licensee a personal, non-exclusive, non-transferable
* license to copy, use, modify, create derivative works of, and compile the
* Cypress Source Code and derivative works for the sole purpose of creating
* custom software in support of licensee product to be used only in conjunction
* with a Cypress integrated circuit as specified in the applicable agreement.
* Any reproduction, modification, translation, compilation, or representation of
* this software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the
* materials described herein. Cypress does not assume any liability arising out
* of the application or use of any product or circuit described herein. Cypress
* does not authorize its products for use as critical components in life-support
* systems where a malfunction or failure may reasonably be expected to result in
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of
* such use and in doing so indemnifies Cypress against all charges. Use may be
* limited by and subject to the applicable Cypress software license agreement.
*****************************************************************************/
#include "project.h"

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */
    /* Enable CM4.  CY_CORTEX_M4_APPL_ADDR must be updated if CM4 memory layout is changed. */
    Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR); 

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
        Cy_GPIO_Inv(Pin_CM0p_BlueLED_0_PORT, Pin_CM0p_BlueLED_0_NUM); /* toggle the pin */
        Cy_SysLib_Delay(1023/*msec*/);
    }
}

/* [] END OF FILE */
