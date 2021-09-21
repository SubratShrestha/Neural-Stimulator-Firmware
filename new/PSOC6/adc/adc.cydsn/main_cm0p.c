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
#include <stdio.h>
#include "project.h"

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */
    /* Enable CM4.  CY_CORTEX_M4_APPL_ADDR must be updated if CM4 memory layout is changed. */
    Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR); 
    UART_1_Start();
    setvbuf(stdin, NULL, _IONBF, 0);

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    printf("Starting ... ");
    ADC_1_Start();
    Cy_SAR_StartConvert(SAR, CY_SAR_START_CONVERT_SINGLE_SHOT);
    volatile float value = Cy_SAR_GetResult16(SAR, 0);
    volatile float volts = Cy_SAR_CountsTo_mVolts(SAR, 0, value);
    printf("value in mv = %f\n", volts);
    
    CyDelay(100);
   
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
