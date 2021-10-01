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
    printf("Project starting ...\n\r");
    UART_1_Start();
    VDAC_1_Start();
    ADC_1_Start();

    setvbuf(stdin, NULL, _IONBF, 0);

    // Cy_SAR_StartConvert(SAR, CY_SAR_START_CONVERT_SINGLE_SHOT);
    // volatile float value = Cy_SAR_GetResult16(SAR, 0);
    // volatile float volts = Cy_SAR_CountsTo_mVolts(SAR, 0, value);
    // printf("value in mv = %f\n", volts);
    
    CyDelay(100);
    

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        Cy_SAR_StartConvert(SAR, CY_SAR_START_CONVERT_CONTINUOUS);
        volatile float value = Cy_SAR_GetResult16(SAR, 0);
        volatile float volts = Cy_SAR_CountsTo_Volts(SAR, 0, value);
        printf("value in v = %f\n\r", volts);
    }
}

/* [] END OF FILE */
