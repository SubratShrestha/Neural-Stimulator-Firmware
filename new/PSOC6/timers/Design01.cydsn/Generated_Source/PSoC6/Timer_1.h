/*******************************************************************************
* File Name: Timer_1.h
* Version 1.0
*
* Description:
*  This file provides constants and parameter values for the Timer_1
*  component.
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(Timer_1_CY_TCPWM_COUNTER_PDL_H)
#define Timer_1_CY_TCPWM_COUNTER_PDL_H

#include "cyfitter.h"
#include "tcpwm/cy_tcpwm_counter.h"

   
/*******************************************************************************
* Variables
*******************************************************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t  Timer_1_initVar;
extern cy_stc_tcpwm_counter_config_t const Timer_1_config;
/** @} group_globals */


/***************************************
*  Conditional Compilation Parameters
***************************************/

#define Timer_1_INIT_COMPARE_OR_CAPTURE    (2UL)


/***************************************
*        Function Prototypes
****************************************/
/**
* \addtogroup group_general
* @{
*/
void Timer_1_Start(void);
__STATIC_INLINE cy_en_tcpwm_status_t Timer_1_Init(cy_stc_tcpwm_counter_config_t const *config);
__STATIC_INLINE void Timer_1_DeInit(void);
__STATIC_INLINE void Timer_1_Enable(void);
__STATIC_INLINE void Timer_1_Disable(void);
__STATIC_INLINE uint32_t Timer_1_GetStatus(void);

#if(CY_TCPWM_COUNTER_MODE_CAPTURE == Timer_1_INIT_COMPARE_OR_CAPTURE)
    __STATIC_INLINE uint32_t Timer_1_GetCapture(void);
    __STATIC_INLINE uint32_t Timer_1_GetCaptureBuf(void);
#else
    __STATIC_INLINE void Timer_1_SetCompare0(uint32_t compare0);
    __STATIC_INLINE uint32_t Timer_1_GetCompare0(void);
    __STATIC_INLINE void Timer_1_SetCompare1(uint32_t compare1);
    __STATIC_INLINE uint32_t Timer_1_GetCompare1(void);
    __STATIC_INLINE void Timer_1_EnableCompareSwap(bool enable);
#endif /* (CY_TCPWM_COUNTER_MODE_CAPTURE == Timer_1_INIT_COMPARE_OR_CAPTURE) */

__STATIC_INLINE void Timer_1_SetCounter(uint32_t count);
__STATIC_INLINE uint32_t Timer_1_GetCounter(void);
__STATIC_INLINE void Timer_1_SetPeriod(uint32_t period);
__STATIC_INLINE uint32_t Timer_1_GetPeriod(void);
__STATIC_INLINE void Timer_1_TriggerStart(void);
__STATIC_INLINE void Timer_1_TriggerReload(void);
__STATIC_INLINE void Timer_1_TriggerStop(void);
__STATIC_INLINE void Timer_1_TriggerCapture(void);
__STATIC_INLINE uint32_t Timer_1_GetInterruptStatus(void);
__STATIC_INLINE void Timer_1_ClearInterrupt(uint32_t source);
__STATIC_INLINE void Timer_1_SetInterrupt(uint32_t source);
__STATIC_INLINE void Timer_1_SetInterruptMask(uint32_t mask);
__STATIC_INLINE uint32_t Timer_1_GetInterruptMask(void);
__STATIC_INLINE uint32_t Timer_1_GetInterruptStatusMasked(void);
/** @} general */


/***************************************
*           API Constants
***************************************/

/**
* \addtogroup group_macros
* @{
*/
/** This is a ptr to the base address of the TCPWM instance */
#define Timer_1_HW                 (Timer_1_TCPWM__HW)

/** This is a ptr to the base address of the TCPWM CNT instance */
#define Timer_1_CNT_HW             (Timer_1_TCPWM__CNT_HW)

/** This is the counter instance number in the selected TCPWM */
#define Timer_1_CNT_NUM            (Timer_1_TCPWM__CNT_IDX)

/** This is the bit field representing the counter instance in the selected TCPWM */
#define Timer_1_CNT_MASK           (1UL << Timer_1_CNT_NUM)
/** @} group_macros */

#define Timer_1_INPUT_MODE_MASK    (0x3U)
#define Timer_1_INPUT_DISABLED     (7U)


/*******************************************************************************
* Function Name: Timer_1_Init
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_tcpwm_status_t Timer_1_Init(cy_stc_tcpwm_counter_config_t const *config)
{
    return(Cy_TCPWM_Counter_Init(Timer_1_HW, Timer_1_CNT_NUM, config));
}


/*******************************************************************************
* Function Name: Timer_1_DeInit
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_1_DeInit(void)                   
{
    Cy_TCPWM_Counter_DeInit(Timer_1_HW, Timer_1_CNT_NUM, &Timer_1_config);
}

/*******************************************************************************
* Function Name: Timer_1_Enable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Enable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_1_Enable(void)                   
{
    Cy_TCPWM_Enable_Multiple(Timer_1_HW, Timer_1_CNT_MASK);
}


/*******************************************************************************
* Function Name: Timer_1_Disable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Disable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_1_Disable(void)                  
{
    Cy_TCPWM_Disable_Multiple(Timer_1_HW, Timer_1_CNT_MASK);
}


/*******************************************************************************
* Function Name: Timer_1_GetStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_GetStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Timer_1_GetStatus(void)                
{
    return(Cy_TCPWM_Counter_GetStatus(Timer_1_HW, Timer_1_CNT_NUM));
}


#if(CY_TCPWM_COUNTER_MODE_CAPTURE == Timer_1_INIT_COMPARE_OR_CAPTURE)
    /*******************************************************************************
    * Function Name: Timer_1_GetCapture
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_GetCapture() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE uint32_t Timer_1_GetCapture(void)               
    {
        return(Cy_TCPWM_Counter_GetCapture(Timer_1_HW, Timer_1_CNT_NUM));
    }


    /*******************************************************************************
    * Function Name: Timer_1_GetCaptureBuf
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_GetCaptureBuf() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE uint32_t Timer_1_GetCaptureBuf(void)            
    {
        return(Cy_TCPWM_Counter_GetCaptureBuf(Timer_1_HW, Timer_1_CNT_NUM));
    }
#else
    /*******************************************************************************
    * Function Name: Timer_1_SetCompare0
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_SetCompare0() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE void Timer_1_SetCompare0(uint32_t compare0)      
    {
        Cy_TCPWM_Counter_SetCompare0(Timer_1_HW, Timer_1_CNT_NUM, compare0);
    }


    /*******************************************************************************
    * Function Name: Timer_1_GetCompare0
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_GetCompare0() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE uint32_t Timer_1_GetCompare0(void)              
    {
        return(Cy_TCPWM_Counter_GetCompare0(Timer_1_HW, Timer_1_CNT_NUM));
    }


    /*******************************************************************************
    * Function Name: Timer_1_SetCompare1
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_SetCompare1() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE void Timer_1_SetCompare1(uint32_t compare1)      
    {
        Cy_TCPWM_Counter_SetCompare1(Timer_1_HW, Timer_1_CNT_NUM, compare1);
    }


    /*******************************************************************************
    * Function Name: Timer_1_GetCompare1
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_GetCompare1() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE uint32_t Timer_1_GetCompare1(void)              
    {
        return(Cy_TCPWM_Counter_GetCompare1(Timer_1_HW, Timer_1_CNT_NUM));
    }


    /*******************************************************************************
    * Function Name: Timer_1_EnableCompareSwap
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_EnableCompareSwap() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE void Timer_1_EnableCompareSwap(bool enable)  
    {
        Cy_TCPWM_Counter_EnableCompareSwap(Timer_1_HW, Timer_1_CNT_NUM, enable);
    }
#endif /* (CY_TCPWM_COUNTER_MODE_CAPTURE == Timer_1_INIT_COMPARE_OR_CAPTURE) */


/*******************************************************************************
* Function Name: Timer_1_SetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_SetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_1_SetCounter(uint32_t count)          
{
    Cy_TCPWM_Counter_SetCounter(Timer_1_HW, Timer_1_CNT_NUM, count);
}


/*******************************************************************************
* Function Name: Timer_1_GetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_GetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Timer_1_GetCounter(void)               
{
    return(Cy_TCPWM_Counter_GetCounter(Timer_1_HW, Timer_1_CNT_NUM));
}


/*******************************************************************************
* Function Name: Timer_1_SetPeriod
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_SetPeriod() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_1_SetPeriod(uint32_t period)          
{
    Cy_TCPWM_Counter_SetPeriod(Timer_1_HW, Timer_1_CNT_NUM, period);
}


/*******************************************************************************
* Function Name: Timer_1_GetPeriod
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_GetPeriod() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Timer_1_GetPeriod(void)                
{
    return(Cy_TCPWM_Counter_GetPeriod(Timer_1_HW, Timer_1_CNT_NUM));
}


/*******************************************************************************
* Function Name: Timer_1_TriggerStart
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerStart() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_1_TriggerStart(void)             
{
    Cy_TCPWM_TriggerStart(Timer_1_HW, Timer_1_CNT_MASK);
}


/*******************************************************************************
* Function Name: Timer_1_TriggerReload
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerReloadOrIndex() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_1_TriggerReload(void)     
{
    Cy_TCPWM_TriggerReloadOrIndex(Timer_1_HW, Timer_1_CNT_MASK);
}


/*******************************************************************************
* Function Name: Timer_1_TriggerStop
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerStopOrKill() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_1_TriggerStop(void)
{
    Cy_TCPWM_TriggerStopOrKill(Timer_1_HW, Timer_1_CNT_MASK);
}


/*******************************************************************************
* Function Name: Timer_1_TriggerCapture
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerCaptureOrSwap() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_1_TriggerCapture(void)     
{
    Cy_TCPWM_TriggerCaptureOrSwap(Timer_1_HW, Timer_1_CNT_MASK);
}


/*******************************************************************************
* Function Name: Timer_1_GetInterruptStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Timer_1_GetInterruptStatus(void)       
{
    return(Cy_TCPWM_GetInterruptStatus(Timer_1_HW, Timer_1_CNT_NUM));
}


/*******************************************************************************
* Function Name: Timer_1_ClearInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_ClearInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_1_ClearInterrupt(uint32_t source)     
{
    Cy_TCPWM_ClearInterrupt(Timer_1_HW, Timer_1_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: Timer_1_SetInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_1_SetInterrupt(uint32_t source)
{
    Cy_TCPWM_SetInterrupt(Timer_1_HW, Timer_1_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: Timer_1_SetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_1_SetInterruptMask(uint32_t mask)     
{
    Cy_TCPWM_SetInterruptMask(Timer_1_HW, Timer_1_CNT_NUM, mask);
}


/*******************************************************************************
* Function Name: Timer_1_GetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Timer_1_GetInterruptMask(void)         
{
    return(Cy_TCPWM_GetInterruptMask(Timer_1_HW, Timer_1_CNT_NUM));
}


/*******************************************************************************
* Function Name: Timer_1_GetInterruptStatusMasked
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatusMasked() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Timer_1_GetInterruptStatusMasked(void)
{
    return(Cy_TCPWM_GetInterruptStatusMasked(Timer_1_HW, Timer_1_CNT_NUM));
}

#endif /* Timer_1_CY_TCPWM_COUNTER_PDL_H */


/* [] END OF FILE */
