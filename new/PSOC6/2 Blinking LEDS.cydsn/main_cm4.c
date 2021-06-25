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

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
        Cy_GPIO_Write(BLINKY2_PORT, BLINKY2_NUM, 1);
        CyDelay(500);
        Cy_GPIO_Write(BLINKY2_PORT, BLINKY2_NUM, 0);
        CyDelay(500);
    }
}

/* [] END OF FILE */
