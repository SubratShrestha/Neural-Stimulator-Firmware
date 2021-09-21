/*****************************************************************************
* File Name: main_cm0p.c
*
* Version: 1.00
*
* Description: The code in this file is executed by CM0+. CM0+ enables CM4 to
*              start the execution. Refer to main_cm4.c file for the actual
*              implementation of this project. 
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

/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary: This is the main entry for Cortex-M0+. This function enables the 
* Cortex-M4 and puts the Cortex-M0+ in deep sleep mode to save power.
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
    __enable_irq(); /* Enable global interrupts. */
    
    /* Enable CM4.  CY_CORTEX_M4_APPL_ADDR must be updated if CM4 memory layout is changed. */
    Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR); 

    for(;;)
    {
        /* Put CPU into deep sleep mode to save power. This puts only CM0+ into
         * deep sleep. Device enters deep sleep only when both the cores enter
         * deep sleep.
         */
        Cy_SysPm_DeepSleep(CY_SYSPM_WAIT_FOR_INTERRUPT);
    }
}


/* [] END OF FILE */
