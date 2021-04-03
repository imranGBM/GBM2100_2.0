/*******************************************************************************
* File Name: PWMAlarm.h
* Version 1.0
*
* Description:
*  This file provides constants and parameter values for the PWMAlarm
*  component.
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(PWMAlarm_CY_TCPWM_PWM_PDL_H)
#define PWMAlarm_CY_TCPWM_PWM_PDL_H

#include "cyfitter.h"
#include "tcpwm/cy_tcpwm_pwm.h"

   
/*******************************************************************************
* Variables
*******************************************************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t  PWMAlarm_initVar;
extern cy_stc_tcpwm_pwm_config_t const PWMAlarm_config;
/** @} group_globals */


/***************************************
*        Function Prototypes
****************************************/
/**
* \addtogroup group_general
* @{
*/
void PWMAlarm_Start(void);
__STATIC_INLINE cy_en_tcpwm_status_t PWMAlarm_Init(cy_stc_tcpwm_pwm_config_t const *config);
__STATIC_INLINE void PWMAlarm_DeInit(void);
__STATIC_INLINE void PWMAlarm_Enable(void);
__STATIC_INLINE void PWMAlarm_Disable(void);
__STATIC_INLINE uint32_t PWMAlarm_GetStatus(void);
__STATIC_INLINE void PWMAlarm_SetCompare0(uint32_t compare0);
__STATIC_INLINE uint32_t PWMAlarm_GetCompare0(void);
__STATIC_INLINE void PWMAlarm_SetCompare1(uint32_t compare1);
__STATIC_INLINE uint32_t PWMAlarm_GetCompare1(void);
__STATIC_INLINE void PWMAlarm_EnableCompareSwap(bool enable);
__STATIC_INLINE void PWMAlarm_SetCounter(uint32_t count);
__STATIC_INLINE uint32_t PWMAlarm_GetCounter(void);
__STATIC_INLINE void PWMAlarm_SetPeriod0(uint32_t period0);
__STATIC_INLINE uint32_t PWMAlarm_GetPeriod0(void);
__STATIC_INLINE void PWMAlarm_SetPeriod1(uint32_t period1);
__STATIC_INLINE uint32_t PWMAlarm_GetPeriod1(void);
__STATIC_INLINE void PWMAlarm_EnablePeriodSwap(bool enable);
__STATIC_INLINE void PWMAlarm_TriggerStart(void);
__STATIC_INLINE void PWMAlarm_TriggerReload(void);
__STATIC_INLINE void PWMAlarm_TriggerKill(void);
__STATIC_INLINE void PWMAlarm_TriggerSwap(void);
__STATIC_INLINE uint32_t PWMAlarm_GetInterruptStatus(void);
__STATIC_INLINE void PWMAlarm_ClearInterrupt(uint32_t source);
__STATIC_INLINE void PWMAlarm_SetInterrupt(uint32_t source);
__STATIC_INLINE void PWMAlarm_SetInterruptMask(uint32_t mask);
__STATIC_INLINE uint32_t PWMAlarm_GetInterruptMask(void);
__STATIC_INLINE uint32_t PWMAlarm_GetInterruptStatusMasked(void);
/** @} general */


/***************************************
*           API Constants
***************************************/

/**
* \addtogroup group_macros
* @{
*/
/** This is a ptr to the base address of the TCPWM instance */
#define PWMAlarm_HW                 (PWMAlarm_TCPWM__HW)

/** This is a ptr to the base address of the TCPWM CNT instance */
#define PWMAlarm_CNT_HW             (PWMAlarm_TCPWM__CNT_HW)

/** This is the counter instance number in the selected TCPWM */
#define PWMAlarm_CNT_NUM            (PWMAlarm_TCPWM__CNT_IDX)

/** This is the bit field representing the counter instance in the selected TCPWM */
#define PWMAlarm_CNT_MASK           (1UL << PWMAlarm_CNT_NUM)
/** @} group_macros */

#define PWMAlarm_INPUT_MODE_MASK    (0x3U)
#define PWMAlarm_INPUT_DISABLED     (7U)


/*******************************************************************************
* Function Name: PWMAlarm_Init
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_tcpwm_status_t PWMAlarm_Init(cy_stc_tcpwm_pwm_config_t const *config)
{
    return(Cy_TCPWM_PWM_Init(PWMAlarm_HW, PWMAlarm_CNT_NUM, config));
}


/*******************************************************************************
* Function Name: PWMAlarm_DeInit
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarm_DeInit(void)                   
{
    Cy_TCPWM_PWM_DeInit(PWMAlarm_HW, PWMAlarm_CNT_NUM, &PWMAlarm_config);
}

/*******************************************************************************
* Function Name: PWMAlarm_Enable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Enable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarm_Enable(void)                   
{
    Cy_TCPWM_Enable_Multiple(PWMAlarm_HW, PWMAlarm_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarm_Disable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Disable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarm_Disable(void)                  
{
    Cy_TCPWM_Disable_Multiple(PWMAlarm_HW, PWMAlarm_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarm_GetStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarm_GetStatus(void)                
{
    return(Cy_TCPWM_PWM_GetStatus(PWMAlarm_HW, PWMAlarm_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarm_SetCompare0
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetCompare0() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarm_SetCompare0(uint32_t compare0)      
{
    Cy_TCPWM_PWM_SetCompare0(PWMAlarm_HW, PWMAlarm_CNT_NUM, compare0);
}


/*******************************************************************************
* Function Name: PWMAlarm_GetCompare0
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetCompare0() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarm_GetCompare0(void)              
{
    return(Cy_TCPWM_PWM_GetCompare0(PWMAlarm_HW, PWMAlarm_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarm_SetCompare1
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetCompare1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarm_SetCompare1(uint32_t compare1)      
{
    Cy_TCPWM_PWM_SetCompare1(PWMAlarm_HW, PWMAlarm_CNT_NUM, compare1);
}


/*******************************************************************************
* Function Name: PWMAlarm_GetCompare1
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetCompare1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarm_GetCompare1(void)              
{
    return(Cy_TCPWM_PWM_GetCompare1(PWMAlarm_HW, PWMAlarm_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarm_EnableCompareSwap
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_EnableCompareSwap() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarm_EnableCompareSwap(bool enable)  
{
    Cy_TCPWM_PWM_EnableCompareSwap(PWMAlarm_HW, PWMAlarm_CNT_NUM, enable);
}


/*******************************************************************************
* Function Name: PWMAlarm_SetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarm_SetCounter(uint32_t count)          
{
    Cy_TCPWM_PWM_SetCounter(PWMAlarm_HW, PWMAlarm_CNT_NUM, count);
}


/*******************************************************************************
* Function Name: PWMAlarm_GetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarm_GetCounter(void)               
{
    return(Cy_TCPWM_PWM_GetCounter(PWMAlarm_HW, PWMAlarm_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarm_SetPeriod0
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetPeriod0() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarm_SetPeriod0(uint32_t period0)          
{
    Cy_TCPWM_PWM_SetPeriod0(PWMAlarm_HW, PWMAlarm_CNT_NUM, period0);
}


/*******************************************************************************
* Function Name: PWMAlarm_GetPeriod0
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetPeriod0() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarm_GetPeriod0(void)                
{
    return(Cy_TCPWM_PWM_GetPeriod0(PWMAlarm_HW, PWMAlarm_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarm_SetPeriod1
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetPeriod1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarm_SetPeriod1(uint32_t period1)
{
    Cy_TCPWM_PWM_SetPeriod1(PWMAlarm_HW, PWMAlarm_CNT_NUM, period1);
}


/*******************************************************************************
* Function Name: PWMAlarm_GetPeriod1
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetPeriod1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarm_GetPeriod1(void)                
{
    return(Cy_TCPWM_PWM_GetPeriod1(PWMAlarm_HW, PWMAlarm_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarm_EnablePeriodSwap
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_EnablePeriodSwap() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarm_EnablePeriodSwap(bool enable)
{
    Cy_TCPWM_PWM_EnablePeriodSwap(PWMAlarm_HW, PWMAlarm_CNT_NUM, enable);
}


/*******************************************************************************
* Function Name: PWMAlarm_TriggerStart
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerStart() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarm_TriggerStart(void)             
{
    Cy_TCPWM_TriggerStart(PWMAlarm_HW, PWMAlarm_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarm_TriggerReload
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerReloadOrIndex() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarm_TriggerReload(void)     
{
    Cy_TCPWM_TriggerReloadOrIndex(PWMAlarm_HW, PWMAlarm_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarm_TriggerKill
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerStopOrKill() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarm_TriggerKill(void)
{
    Cy_TCPWM_TriggerStopOrKill(PWMAlarm_HW, PWMAlarm_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarm_TriggerSwap
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerCaptureOrSwap() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarm_TriggerSwap(void)     
{
    Cy_TCPWM_TriggerCaptureOrSwap(PWMAlarm_HW, PWMAlarm_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarm_GetInterruptStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarm_GetInterruptStatus(void)       
{
    return(Cy_TCPWM_GetInterruptStatus(PWMAlarm_HW, PWMAlarm_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarm_ClearInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_ClearInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarm_ClearInterrupt(uint32_t source)     
{
    Cy_TCPWM_ClearInterrupt(PWMAlarm_HW, PWMAlarm_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: PWMAlarm_SetInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarm_SetInterrupt(uint32_t source)
{
    Cy_TCPWM_SetInterrupt(PWMAlarm_HW, PWMAlarm_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: PWMAlarm_SetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarm_SetInterruptMask(uint32_t mask)     
{
    Cy_TCPWM_SetInterruptMask(PWMAlarm_HW, PWMAlarm_CNT_NUM, mask);
}


/*******************************************************************************
* Function Name: PWMAlarm_GetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarm_GetInterruptMask(void)         
{
    return(Cy_TCPWM_GetInterruptMask(PWMAlarm_HW, PWMAlarm_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarm_GetInterruptStatusMasked
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatusMasked() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarm_GetInterruptStatusMasked(void)
{
    return(Cy_TCPWM_GetInterruptStatusMasked(PWMAlarm_HW, PWMAlarm_CNT_NUM));
}

#endif /* PWMAlarm_CY_TCPWM_PWM_PDL_H */


/* [] END OF FILE */
