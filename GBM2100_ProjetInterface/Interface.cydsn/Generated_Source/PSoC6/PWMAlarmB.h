/*******************************************************************************
* File Name: PWMAlarmB.h
* Version 1.0
*
* Description:
*  This file provides constants and parameter values for the PWMAlarmB
*  component.
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(PWMAlarmB_CY_TCPWM_PWM_PDL_H)
#define PWMAlarmB_CY_TCPWM_PWM_PDL_H

#include "cyfitter.h"
#include "tcpwm/cy_tcpwm_pwm.h"

   
/*******************************************************************************
* Variables
*******************************************************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t  PWMAlarmB_initVar;
extern cy_stc_tcpwm_pwm_config_t const PWMAlarmB_config;
/** @} group_globals */


/***************************************
*        Function Prototypes
****************************************/
/**
* \addtogroup group_general
* @{
*/
void PWMAlarmB_Start(void);
__STATIC_INLINE cy_en_tcpwm_status_t PWMAlarmB_Init(cy_stc_tcpwm_pwm_config_t const *config);
__STATIC_INLINE void PWMAlarmB_DeInit(void);
__STATIC_INLINE void PWMAlarmB_Enable(void);
__STATIC_INLINE void PWMAlarmB_Disable(void);
__STATIC_INLINE uint32_t PWMAlarmB_GetStatus(void);
__STATIC_INLINE void PWMAlarmB_SetCompare0(uint32_t compare0);
__STATIC_INLINE uint32_t PWMAlarmB_GetCompare0(void);
__STATIC_INLINE void PWMAlarmB_SetCompare1(uint32_t compare1);
__STATIC_INLINE uint32_t PWMAlarmB_GetCompare1(void);
__STATIC_INLINE void PWMAlarmB_EnableCompareSwap(bool enable);
__STATIC_INLINE void PWMAlarmB_SetCounter(uint32_t count);
__STATIC_INLINE uint32_t PWMAlarmB_GetCounter(void);
__STATIC_INLINE void PWMAlarmB_SetPeriod0(uint32_t period0);
__STATIC_INLINE uint32_t PWMAlarmB_GetPeriod0(void);
__STATIC_INLINE void PWMAlarmB_SetPeriod1(uint32_t period1);
__STATIC_INLINE uint32_t PWMAlarmB_GetPeriod1(void);
__STATIC_INLINE void PWMAlarmB_EnablePeriodSwap(bool enable);
__STATIC_INLINE void PWMAlarmB_TriggerStart(void);
__STATIC_INLINE void PWMAlarmB_TriggerReload(void);
__STATIC_INLINE void PWMAlarmB_TriggerKill(void);
__STATIC_INLINE void PWMAlarmB_TriggerSwap(void);
__STATIC_INLINE uint32_t PWMAlarmB_GetInterruptStatus(void);
__STATIC_INLINE void PWMAlarmB_ClearInterrupt(uint32_t source);
__STATIC_INLINE void PWMAlarmB_SetInterrupt(uint32_t source);
__STATIC_INLINE void PWMAlarmB_SetInterruptMask(uint32_t mask);
__STATIC_INLINE uint32_t PWMAlarmB_GetInterruptMask(void);
__STATIC_INLINE uint32_t PWMAlarmB_GetInterruptStatusMasked(void);
/** @} general */


/***************************************
*           API Constants
***************************************/

/**
* \addtogroup group_macros
* @{
*/
/** This is a ptr to the base address of the TCPWM instance */
#define PWMAlarmB_HW                 (PWMAlarmB_TCPWM__HW)

/** This is a ptr to the base address of the TCPWM CNT instance */
#define PWMAlarmB_CNT_HW             (PWMAlarmB_TCPWM__CNT_HW)

/** This is the counter instance number in the selected TCPWM */
#define PWMAlarmB_CNT_NUM            (PWMAlarmB_TCPWM__CNT_IDX)

/** This is the bit field representing the counter instance in the selected TCPWM */
#define PWMAlarmB_CNT_MASK           (1UL << PWMAlarmB_CNT_NUM)
/** @} group_macros */

#define PWMAlarmB_INPUT_MODE_MASK    (0x3U)
#define PWMAlarmB_INPUT_DISABLED     (7U)


/*******************************************************************************
* Function Name: PWMAlarmB_Init
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_tcpwm_status_t PWMAlarmB_Init(cy_stc_tcpwm_pwm_config_t const *config)
{
    return(Cy_TCPWM_PWM_Init(PWMAlarmB_HW, PWMAlarmB_CNT_NUM, config));
}


/*******************************************************************************
* Function Name: PWMAlarmB_DeInit
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmB_DeInit(void)                   
{
    Cy_TCPWM_PWM_DeInit(PWMAlarmB_HW, PWMAlarmB_CNT_NUM, &PWMAlarmB_config);
}

/*******************************************************************************
* Function Name: PWMAlarmB_Enable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Enable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmB_Enable(void)                   
{
    Cy_TCPWM_Enable_Multiple(PWMAlarmB_HW, PWMAlarmB_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarmB_Disable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Disable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmB_Disable(void)                  
{
    Cy_TCPWM_Disable_Multiple(PWMAlarmB_HW, PWMAlarmB_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarmB_GetStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmB_GetStatus(void)                
{
    return(Cy_TCPWM_PWM_GetStatus(PWMAlarmB_HW, PWMAlarmB_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmB_SetCompare0
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetCompare0() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmB_SetCompare0(uint32_t compare0)      
{
    Cy_TCPWM_PWM_SetCompare0(PWMAlarmB_HW, PWMAlarmB_CNT_NUM, compare0);
}


/*******************************************************************************
* Function Name: PWMAlarmB_GetCompare0
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetCompare0() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmB_GetCompare0(void)              
{
    return(Cy_TCPWM_PWM_GetCompare0(PWMAlarmB_HW, PWMAlarmB_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmB_SetCompare1
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetCompare1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmB_SetCompare1(uint32_t compare1)      
{
    Cy_TCPWM_PWM_SetCompare1(PWMAlarmB_HW, PWMAlarmB_CNT_NUM, compare1);
}


/*******************************************************************************
* Function Name: PWMAlarmB_GetCompare1
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetCompare1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmB_GetCompare1(void)              
{
    return(Cy_TCPWM_PWM_GetCompare1(PWMAlarmB_HW, PWMAlarmB_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmB_EnableCompareSwap
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_EnableCompareSwap() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmB_EnableCompareSwap(bool enable)  
{
    Cy_TCPWM_PWM_EnableCompareSwap(PWMAlarmB_HW, PWMAlarmB_CNT_NUM, enable);
}


/*******************************************************************************
* Function Name: PWMAlarmB_SetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmB_SetCounter(uint32_t count)          
{
    Cy_TCPWM_PWM_SetCounter(PWMAlarmB_HW, PWMAlarmB_CNT_NUM, count);
}


/*******************************************************************************
* Function Name: PWMAlarmB_GetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmB_GetCounter(void)               
{
    return(Cy_TCPWM_PWM_GetCounter(PWMAlarmB_HW, PWMAlarmB_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmB_SetPeriod0
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetPeriod0() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmB_SetPeriod0(uint32_t period0)          
{
    Cy_TCPWM_PWM_SetPeriod0(PWMAlarmB_HW, PWMAlarmB_CNT_NUM, period0);
}


/*******************************************************************************
* Function Name: PWMAlarmB_GetPeriod0
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetPeriod0() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmB_GetPeriod0(void)                
{
    return(Cy_TCPWM_PWM_GetPeriod0(PWMAlarmB_HW, PWMAlarmB_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmB_SetPeriod1
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetPeriod1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmB_SetPeriod1(uint32_t period1)
{
    Cy_TCPWM_PWM_SetPeriod1(PWMAlarmB_HW, PWMAlarmB_CNT_NUM, period1);
}


/*******************************************************************************
* Function Name: PWMAlarmB_GetPeriod1
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetPeriod1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmB_GetPeriod1(void)                
{
    return(Cy_TCPWM_PWM_GetPeriod1(PWMAlarmB_HW, PWMAlarmB_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmB_EnablePeriodSwap
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_EnablePeriodSwap() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmB_EnablePeriodSwap(bool enable)
{
    Cy_TCPWM_PWM_EnablePeriodSwap(PWMAlarmB_HW, PWMAlarmB_CNT_NUM, enable);
}


/*******************************************************************************
* Function Name: PWMAlarmB_TriggerStart
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerStart() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmB_TriggerStart(void)             
{
    Cy_TCPWM_TriggerStart(PWMAlarmB_HW, PWMAlarmB_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarmB_TriggerReload
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerReloadOrIndex() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmB_TriggerReload(void)     
{
    Cy_TCPWM_TriggerReloadOrIndex(PWMAlarmB_HW, PWMAlarmB_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarmB_TriggerKill
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerStopOrKill() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmB_TriggerKill(void)
{
    Cy_TCPWM_TriggerStopOrKill(PWMAlarmB_HW, PWMAlarmB_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarmB_TriggerSwap
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerCaptureOrSwap() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmB_TriggerSwap(void)     
{
    Cy_TCPWM_TriggerCaptureOrSwap(PWMAlarmB_HW, PWMAlarmB_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarmB_GetInterruptStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmB_GetInterruptStatus(void)       
{
    return(Cy_TCPWM_GetInterruptStatus(PWMAlarmB_HW, PWMAlarmB_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmB_ClearInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_ClearInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmB_ClearInterrupt(uint32_t source)     
{
    Cy_TCPWM_ClearInterrupt(PWMAlarmB_HW, PWMAlarmB_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: PWMAlarmB_SetInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmB_SetInterrupt(uint32_t source)
{
    Cy_TCPWM_SetInterrupt(PWMAlarmB_HW, PWMAlarmB_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: PWMAlarmB_SetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmB_SetInterruptMask(uint32_t mask)     
{
    Cy_TCPWM_SetInterruptMask(PWMAlarmB_HW, PWMAlarmB_CNT_NUM, mask);
}


/*******************************************************************************
* Function Name: PWMAlarmB_GetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmB_GetInterruptMask(void)         
{
    return(Cy_TCPWM_GetInterruptMask(PWMAlarmB_HW, PWMAlarmB_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmB_GetInterruptStatusMasked
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatusMasked() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmB_GetInterruptStatusMasked(void)
{
    return(Cy_TCPWM_GetInterruptStatusMasked(PWMAlarmB_HW, PWMAlarmB_CNT_NUM));
}

#endif /* PWMAlarmB_CY_TCPWM_PWM_PDL_H */


/* [] END OF FILE */
