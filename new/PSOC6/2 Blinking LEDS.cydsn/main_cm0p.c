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

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */
    /* Enable CM4.  CY_CORTEX_M4_APPL_ADDR must be updated if CM4 memory layout is changed. */
    
    // this just turns on the M4 chip
    Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR); 
    
    

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
        Cy_GPIO_Write(BLINKY1_PORT, BLINKY1_NUM, 1);
        CyDelay(500);
        Cy_GPIO_Write(BLINKY1_PORT, BLINKY1_NUM, 0);
        CyDelay(500);
    }
}

/* [] END OF FILE */
