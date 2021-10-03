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



int main(void)
{
    __enable_irq(); /* Enable global interrupts. */
    
    UART_1_Start();
    
    setvbuf(stdin, NULL,_IONBF,0);
    int i = 0;
    while (i < 100) {
        printf("literally anything plz");
        CyDelay(1000);
        i++;
    }
    
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
