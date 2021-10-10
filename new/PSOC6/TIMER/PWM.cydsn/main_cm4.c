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

uint32_t dacWrite = 0x800;

void TimerInterruptHandler(void)
{
    /* Clear the terminal count interrupt */
    printf("%" PRIu32 "\r\n", Cy_TCPWM_Counter_GetCounter(Timer_HW, Timer_CNT_NUM));
    Cy_TCPWM_ClearInterrupt(Timer_HW, Timer_CNT_NUM, CY_TCPWM_INT_ON_TC);

    
    
    VDAC_1_SetValue(dacWrite);
    if (dacWrite == 0x000) {
        dacWrite = 0x800;   
    } else if (dacWrite == 0x800) {
        dacWrite = 0x7FF;
    } else if (dacWrite == 0x7FF) {
        dacWrite = 0x000;   
    }
    
    Cy_TCPWM_TriggerStopOrKill(Timer_HW, Timer_CNT_NUM);
    Cy_TCPWM_Counter_SetCompare0(Timer_HW, Timer_CNT_NUM, 10u);
}

int main(void)
{
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
    Cy_TCPWM_TriggerStopOrKill(Timer_HW, Timer_CNT_NUM);
    Cy_TCPWM_Counter_SetCompare0(Timer_HW, Timer_CNT_NUM, 10u);
    Cy_TCPWM_TriggerReloadOrIndex(Timer_HW, Timer_CNT_MASK);
    
    for(;;)
    {
        /* Place your application code here. */
        /* CyDelay(1000);
        printf("%" PRIu32 "\r\n", Cy_TCPWM_Counter_GetCounter(Timer_HW, Timer_CNT_NUM));*/
    }
}

/* [] END OF FILE */
