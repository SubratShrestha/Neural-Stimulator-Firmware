/*******************************************************************************
* File Name: Timer.h
* Version 1.0
*
* Description:
*  This file provides constants and parameter values for the Timer
*  component.
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(Timer_CY_TCPWM_PWM_PDL_H)
#define Timer_CY_TCPWM_PWM_PDL_H

#include "cyfitter.h"
#include "tcpwm/cy_tcpwm_pwm.h"

   
/*******************************************************************************
* Variables
*******************************************************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t  Timer_initVar;
extern cy_stc_tcpwm_pwm_config_t const Timer_config;
/** @} group_globals */


/***************************************
*        Function Prototypes
****************************************/
/**
* \addtogroup group_general
* @{
*/
void Timer_Start(void);
__STATIC_INLINE cy_en_tcpwm_status_t Timer_Init(cy_stc_tcpwm_pwm_config_t const *config);
__STATIC_INLINE void Timer_DeInit(void);
__STATIC_INLINE void Timer_Enable(void);
__STATIC_INLINE void Timer_Disable(void);
__STATIC_INLINE uint32_t Timer_GetStatus(void);
__STATIC_INLINE void Timer_SetCompare0(uint32_t compare0);
__STATIC_INLINE uint32_t Timer_GetCompare0(void);
__STATIC_INLINE void Timer_SetCompare1(uint32_t compare1);
__STATIC_INLINE uint32_t Timer_GetCompare1(void);
__STATIC_INLINE void Timer_EnableCompareSwap(bool enable);
__STATIC_INLINE void Timer_SetCounter(uint32_t count);
__STATIC_INLINE uint32_t Timer_GetCounter(void);
__STATIC_INLINE void Timer_SetPeriod0(uint32_t period0);
__STATIC_INLINE uint32_t Timer_GetPeriod0(void);
__STATIC_INLINE void Timer_SetPeriod1(uint32_t period1);
__STATIC_INLINE uint32_t Timer_GetPeriod1(void);
__STATIC_INLINE void Timer_EnablePeriodSwap(bool enable);
__STATIC_INLINE void Timer_TriggerStart(void);
__STATIC_INLINE void Timer_TriggerReload(void);
__STATIC_INLINE void Timer_TriggerKill(void);
__STATIC_INLINE void Timer_TriggerSwap(void);
__STATIC_INLINE uint32_t Timer_GetInterruptStatus(void);
__STATIC_INLINE void Timer_ClearInterrupt(uint32_t source);
__STATIC_INLINE void Timer_SetInterrupt(uint32_t source);
__STATIC_INLINE void Timer_SetInterruptMask(uint32_t mask);
__STATIC_INLINE uint32_t Timer_GetInterruptMask(void);
__STATIC_INLINE uint32_t Timer_GetInterruptStatusMasked(void);
/** @} general */


/***************************************
*           API Constants
***************************************/

/**
* \addtogroup group_macros
* @{
*/
/** This is a ptr to the base address of the TCPWM instance */
#define Timer_HW                 (Timer_TCPWM__HW)

/** This is a ptr to the base address of the TCPWM CNT instance */
#define Timer_CNT_HW             (Timer_TCPWM__CNT_HW)

/** This is the counter instance number in the selected TCPWM */
#define Timer_CNT_NUM            (Timer_TCPWM__CNT_IDX)

/** This is the bit field representing the counter instance in the selected TCPWM */
#define Timer_CNT_MASK           (1UL << Timer_CNT_NUM)
/** @} group_macros */

#define Timer_INPUT_MODE_MASK    (0x3U)
#define Timer_INPUT_DISABLED     (7U)


/*******************************************************************************
* Function Name: Timer_Init
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_tcpwm_status_t Timer_Init(cy_stc_tcpwm_pwm_config_t const *config)
{
    return(Cy_TCPWM_PWM_Init(Timer_HW, Timer_CNT_NUM, config));
}


/*******************************************************************************
* Function Name: Timer_DeInit
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_DeInit(void)                   
{
    Cy_TCPWM_PWM_DeInit(Timer_HW, Timer_CNT_NUM, &Timer_config);
}

/*******************************************************************************
* Function Name: Timer_Enable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Enable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_Enable(void)                   
{
    Cy_TCPWM_Enable_Multiple(Timer_HW, Timer_CNT_MASK);
}


/*******************************************************************************
* Function Name: Timer_Disable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Disable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_Disable(void)                  
{
    Cy_TCPWM_Disable_Multiple(Timer_HW, Timer_CNT_MASK);
}


/*******************************************************************************
* Function Name: Timer_GetStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Timer_GetStatus(void)                
{
    return(Cy_TCPWM_PWM_GetStatus(Timer_HW, Timer_CNT_NUM));
}


/*******************************************************************************
* Function Name: Timer_SetCompare0
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetCompare0() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_SetCompare0(uint32_t compare0)      
{
    Cy_TCPWM_PWM_SetCompare0(Timer_HW, Timer_CNT_NUM, compare0);
}


/*******************************************************************************
* Function Name: Timer_GetCompare0
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetCompare0() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Timer_GetCompare0(void)              
{
    return(Cy_TCPWM_PWM_GetCompare0(Timer_HW, Timer_CNT_NUM));
}


/*******************************************************************************
* Function Name: Timer_SetCompare1
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetCompare1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_SetCompare1(uint32_t compare1)      
{
    Cy_TCPWM_PWM_SetCompare1(Timer_HW, Timer_CNT_NUM, compare1);
}


/*******************************************************************************
* Function Name: Timer_GetCompare1
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetCompare1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Timer_GetCompare1(void)              
{
    return(Cy_TCPWM_PWM_GetCompare1(Timer_HW, Timer_CNT_NUM));
}


/*******************************************************************************
* Function Name: Timer_EnableCompareSwap
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_EnableCompareSwap() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_EnableCompareSwap(bool enable)  
{
    Cy_TCPWM_PWM_EnableCompareSwap(Timer_HW, Timer_CNT_NUM, enable);
}


/*******************************************************************************
* Function Name: Timer_SetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_SetCounter(uint32_t count)          
{
    Cy_TCPWM_PWM_SetCounter(Timer_HW, Timer_CNT_NUM, count);
}


/*******************************************************************************
* Function Name: Timer_GetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Timer_GetCounter(void)               
{
    return(Cy_TCPWM_PWM_GetCounter(Timer_HW, Timer_CNT_NUM));
}


/*******************************************************************************
* Function Name: Timer_SetPeriod0
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetPeriod0() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_SetPeriod0(uint32_t period0)          
{
    Cy_TCPWM_PWM_SetPeriod0(Timer_HW, Timer_CNT_NUM, period0);
}


/*******************************************************************************
* Function Name: Timer_GetPeriod0
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetPeriod0() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Timer_GetPeriod0(void)                
{
    return(Cy_TCPWM_PWM_GetPeriod0(Timer_HW, Timer_CNT_NUM));
}


/*******************************************************************************
* Function Name: Timer_SetPeriod1
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetPeriod1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_SetPeriod1(uint32_t period1)
{
    Cy_TCPWM_PWM_SetPeriod1(Timer_HW, Timer_CNT_NUM, period1);
}


/*******************************************************************************
* Function Name: Timer_GetPeriod1
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetPeriod1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Timer_GetPeriod1(void)                
{
    return(Cy_TCPWM_PWM_GetPeriod1(Timer_HW, Timer_CNT_NUM));
}


/*******************************************************************************
* Function Name: Timer_EnablePeriodSwap
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_EnablePeriodSwap() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_EnablePeriodSwap(bool enable)
{
    Cy_TCPWM_PWM_EnablePeriodSwap(Timer_HW, Timer_CNT_NUM, enable);
}


/*******************************************************************************
* Function Name: Timer_TriggerStart
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerStart() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_TriggerStart(void)             
{
    Cy_TCPWM_TriggerStart(Timer_HW, Timer_CNT_MASK);
}


/*******************************************************************************
* Function Name: Timer_TriggerReload
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerReloadOrIndex() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_TriggerReload(void)     
{
    Cy_TCPWM_TriggerReloadOrIndex(Timer_HW, Timer_CNT_MASK);
}


/*******************************************************************************
* Function Name: Timer_TriggerKill
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerStopOrKill() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_TriggerKill(void)
{
    Cy_TCPWM_TriggerStopOrKill(Timer_HW, Timer_CNT_MASK);
}


/*******************************************************************************
* Function Name: Timer_TriggerSwap
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerCaptureOrSwap() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_TriggerSwap(void)     
{
    Cy_TCPWM_TriggerCaptureOrSwap(Timer_HW, Timer_CNT_MASK);
}


/*******************************************************************************
* Function Name: Timer_GetInterruptStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Timer_GetInterruptStatus(void)       
{
    return(Cy_TCPWM_GetInterruptStatus(Timer_HW, Timer_CNT_NUM));
}


/*******************************************************************************
* Function Name: Timer_ClearInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_ClearInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_ClearInterrupt(uint32_t source)     
{
    Cy_TCPWM_ClearInterrupt(Timer_HW, Timer_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: Timer_SetInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_SetInterrupt(uint32_t source)
{
    Cy_TCPWM_SetInterrupt(Timer_HW, Timer_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: Timer_SetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Timer_SetInterruptMask(uint32_t mask)     
{
    Cy_TCPWM_SetInterruptMask(Timer_HW, Timer_CNT_NUM, mask);
}


/*******************************************************************************
* Function Name: Timer_GetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Timer_GetInterruptMask(void)         
{
    return(Cy_TCPWM_GetInterruptMask(Timer_HW, Timer_CNT_NUM));
}


/*******************************************************************************
* Function Name: Timer_GetInterruptStatusMasked
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatusMasked() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Timer_GetInterruptStatusMasked(void)
{
    return(Cy_TCPWM_GetInterruptStatusMasked(Timer_HW, Timer_CNT_NUM));
}

#endif /* Timer_CY_TCPWM_PWM_PDL_H */


/* [] END OF FILE */
