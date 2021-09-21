/*******************************************************************************
* File Name: cyfitter_sysint_cfg.h
* 
* PSoC Creator  4.4
*
* Description:
* 
* This file is automatically generated by PSoC Creator.
*
********************************************************************************
* Copyright (c) 2007-2020 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#ifndef INCLUDED_CYFITTER_SYSINT_CFG_H
#define INCLUDED_CYFITTER_SYSINT_CFG_H

#include "sysint/cy_sysint.h"

/* ARM CM4 */
#if (((__CORTEX_M == 4) && (CY_CORE_ID == 0)))
    #define UART_SCB_IRQ__INTC_ASSIGNED 1u
    extern const cy_stc_sysint_t UART_SCB_IRQ_cfg;

    #define isrTimer__INTC_ASSIGNED 1u
    extern const cy_stc_sysint_t isrTimer_cfg;

#endif /* ((__CORTEX_M == 4) && (CY_CORE_ID == 0)) */


#endif /* INCLUDED_CYFITTER_SYSINT_CFG_H */
