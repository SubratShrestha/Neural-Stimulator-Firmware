/*******************************************************************************
* File Name: Timer_1.c
* Version 1.0
*
* Description:
*  This file provides the source code to the API for the Timer_1
*  component
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Timer_1.h"

/** Indicates whether or not the Timer_1 has been initialized. 
*  The variable is initialized to 0 and set to 1 the first time 
*  Timer_1_Start() is called. This allows the Component to 
*  restart without reinitialization after the first call to 
*  the Timer_1_Start() routine.
*/
uint8_t Timer_1_initVar = 0U;

/** The instance-specific configuration structure. This should be used in the 
*  associated Timer_1_Init() function.
*/ 
cy_stc_tcpwm_counter_config_t const Timer_1_config =
{
        .period = 32768UL,
        .clockPrescaler = 0UL,
        .runMode = 0UL,
        .countDirection = 0UL,
        .compareOrCapture = 2UL,
        .compare0 = 16384UL,
        .compare1 = 16384UL,
        .enableCompareSwap = false,
        .interruptSources = 1UL,
        .captureInputMode = 3UL,
        .captureInput = CY_TCPWM_INPUT_CREATOR,
        .reloadInputMode = 3UL,
        .reloadInput = CY_TCPWM_INPUT_CREATOR,
        .startInputMode = 3UL,
        .startInput = CY_TCPWM_INPUT_CREATOR,
        .stopInputMode = 3UL,
        .stopInput = CY_TCPWM_INPUT_CREATOR,
        .countInputMode = 3UL,
        .countInput = CY_TCPWM_INPUT_CREATOR,
};


/*******************************************************************************
* Function Name: Timer_1_Start
****************************************************************************//**
*
*  Calls the Timer_1_Init() when called the first time and enables 
*  the Timer_1. For subsequent calls the configuration is left 
*  unchanged and the component is just enabled.
*
* \globalvars
*  \ref Timer_1_initVar
*
*******************************************************************************/
void Timer_1_Start(void)
{
    if (0U == Timer_1_initVar)
    {
        (void)Cy_TCPWM_Counter_Init(Timer_1_HW, Timer_1_CNT_NUM, &Timer_1_config); 

        Timer_1_initVar = 1U;
    }

    Cy_TCPWM_Enable_Multiple(Timer_1_HW, Timer_1_CNT_MASK);
    
    #if (Timer_1_INPUT_DISABLED == 7UL)
        Cy_TCPWM_TriggerStart(Timer_1_HW, Timer_1_CNT_MASK);
    #endif /* (Timer_1_INPUT_DISABLED == 7UL) */    
}


/* [] END OF FILE */
