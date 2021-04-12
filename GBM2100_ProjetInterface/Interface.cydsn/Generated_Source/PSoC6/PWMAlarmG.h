/*******************************************************************************
* File Name: PWMAlarmG.h
* Version 1.0
*
* Description:
*  This file provides constants and parameter values for the PWMAlarmG
*  component.
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(PWMAlarmG_CY_TCPWM_PWM_PDL_H)
#define PWMAlarmG_CY_TCPWM_PWM_PDL_H

#include "cyfitter.h"
#include "tcpwm/cy_tcpwm_pwm.h"

   
/*******************************************************************************
* Variables
*******************************************************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t  PWMAlarmG_initVar;
extern cy_stc_tcpwm_pwm_config_t const PWMAlarmG_config;
/** @} group_globals */


/***************************************
*        Function Prototypes
****************************************/
/**
* \addtogroup group_general
* @{
*/
void PWMAlarmG_Start(void);
__STATIC_INLINE cy_en_tcpwm_status_t PWMAlarmG_Init(cy_stc_tcpwm_pwm_config_t const *config);
__STATIC_INLINE void PWMAlarmG_DeInit(void);
__STATIC_INLINE void PWMAlarmG_Enable(void);
__STATIC_INLINE void PWMAlarmG_Disable(void);
__STATIC_INLINE uint32_t PWMAlarmG_GetStatus(void);
__STATIC_INLINE void PWMAlarmG_SetCompare0(uint32_t compare0);
__STATIC_INLINE uint32_t PWMAlarmG_GetCompare0(void);
__STATIC_INLINE void PWMAlarmG_SetCompare1(uint32_t compare1);
__STATIC_INLINE uint32_t PWMAlarmG_GetCompare1(void);
__STATIC_INLINE void PWMAlarmG_EnableCompareSwap(bool enable);
__STATIC_INLINE void PWMAlarmG_SetCounter(uint32_t count);
__STATIC_INLINE uint32_t PWMAlarmG_GetCounter(void);
__STATIC_INLINE void PWMAlarmG_SetPeriod0(uint32_t period0);
__STATIC_INLINE uint32_t PWMAlarmG_GetPeriod0(void);
__STATIC_INLINE void PWMAlarmG_SetPeriod1(uint32_t period1);
__STATIC_INLINE uint32_t PWMAlarmG_GetPeriod1(void);
__STATIC_INLINE void PWMAlarmG_EnablePeriodSwap(bool enable);
__STATIC_INLINE void PWMAlarmG_TriggerStart(void);
__STATIC_INLINE void PWMAlarmG_TriggerReload(void);
__STATIC_INLINE void PWMAlarmG_TriggerKill(void);
__STATIC_INLINE void PWMAlarmG_TriggerSwap(void);
__STATIC_INLINE uint32_t PWMAlarmG_GetInterruptStatus(void);
__STATIC_INLINE void PWMAlarmG_ClearInterrupt(uint32_t source);
__STATIC_INLINE void PWMAlarmG_SetInterrupt(uint32_t source);
__STATIC_INLINE void PWMAlarmG_SetInterruptMask(uint32_t mask);
__STATIC_INLINE uint32_t PWMAlarmG_GetInterruptMask(void);
__STATIC_INLINE uint32_t PWMAlarmG_GetInterruptStatusMasked(void);
/** @} general */


/***************************************
*           API Constants
***************************************/

/**
* \addtogroup group_macros
* @{
*/
/** This is a ptr to the base address of the TCPWM instance */
#define PWMAlarmG_HW                 (PWMAlarmG_TCPWM__HW)

/** This is a ptr to the base address of the TCPWM CNT instance */
#define PWMAlarmG_CNT_HW             (PWMAlarmG_TCPWM__CNT_HW)

/** This is the counter instance number in the selected TCPWM */
#define PWMAlarmG_CNT_NUM            (PWMAlarmG_TCPWM__CNT_IDX)

/** This is the bit field representing the counter instance in the selected TCPWM */
#define PWMAlarmG_CNT_MASK           (1UL << PWMAlarmG_CNT_NUM)
/** @} group_macros */

#define PWMAlarmG_INPUT_MODE_MASK    (0x3U)
#define PWMAlarmG_INPUT_DISABLED     (7U)


/*******************************************************************************
* Function Name: PWMAlarmG_Init
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_tcpwm_status_t PWMAlarmG_Init(cy_stc_tcpwm_pwm_config_t const *config)
{
    return(Cy_TCPWM_PWM_Init(PWMAlarmG_HW, PWMAlarmG_CNT_NUM, config));
}


/*******************************************************************************
* Function Name: PWMAlarmG_DeInit
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmG_DeInit(void)                   
{
    Cy_TCPWM_PWM_DeInit(PWMAlarmG_HW, PWMAlarmG_CNT_NUM, &PWMAlarmG_config);
}

/*******************************************************************************
* Function Name: PWMAlarmG_Enable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Enable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmG_Enable(void)                   
{
    Cy_TCPWM_Enable_Multiple(PWMAlarmG_HW, PWMAlarmG_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarmG_Disable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Disable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmG_Disable(void)                  
{
    Cy_TCPWM_Disable_Multiple(PWMAlarmG_HW, PWMAlarmG_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarmG_GetStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmG_GetStatus(void)                
{
    return(Cy_TCPWM_PWM_GetStatus(PWMAlarmG_HW, PWMAlarmG_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmG_SetCompare0
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetCompare0() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmG_SetCompare0(uint32_t compare0)      
{
    Cy_TCPWM_PWM_SetCompare0(PWMAlarmG_HW, PWMAlarmG_CNT_NUM, compare0);
}


/*******************************************************************************
* Function Name: PWMAlarmG_GetCompare0
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetCompare0() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmG_GetCompare0(void)              
{
    return(Cy_TCPWM_PWM_GetCompare0(PWMAlarmG_HW, PWMAlarmG_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmG_SetCompare1
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetCompare1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmG_SetCompare1(uint32_t compare1)      
{
    Cy_TCPWM_PWM_SetCompare1(PWMAlarmG_HW, PWMAlarmG_CNT_NUM, compare1);
}


/*******************************************************************************
* Function Name: PWMAlarmG_GetCompare1
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetCompare1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmG_GetCompare1(void)              
{
    return(Cy_TCPWM_PWM_GetCompare1(PWMAlarmG_HW, PWMAlarmG_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmG_EnableCompareSwap
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_EnableCompareSwap() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmG_EnableCompareSwap(bool enable)  
{
    Cy_TCPWM_PWM_EnableCompareSwap(PWMAlarmG_HW, PWMAlarmG_CNT_NUM, enable);
}


/*******************************************************************************
* Function Name: PWMAlarmG_SetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmG_SetCounter(uint32_t count)          
{
    Cy_TCPWM_PWM_SetCounter(PWMAlarmG_HW, PWMAlarmG_CNT_NUM, count);
}


/*******************************************************************************
* Function Name: PWMAlarmG_GetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmG_GetCounter(void)               
{
    return(Cy_TCPWM_PWM_GetCounter(PWMAlarmG_HW, PWMAlarmG_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmG_SetPeriod0
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetPeriod0() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmG_SetPeriod0(uint32_t period0)          
{
    Cy_TCPWM_PWM_SetPeriod0(PWMAlarmG_HW, PWMAlarmG_CNT_NUM, period0);
}


/*******************************************************************************
* Function Name: PWMAlarmG_GetPeriod0
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetPeriod0() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmG_GetPeriod0(void)                
{
    return(Cy_TCPWM_PWM_GetPeriod0(PWMAlarmG_HW, PWMAlarmG_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmG_SetPeriod1
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetPeriod1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmG_SetPeriod1(uint32_t period1)
{
    Cy_TCPWM_PWM_SetPeriod1(PWMAlarmG_HW, PWMAlarmG_CNT_NUM, period1);
}


/*******************************************************************************
* Function Name: PWMAlarmG_GetPeriod1
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetPeriod1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmG_GetPeriod1(void)                
{
    return(Cy_TCPWM_PWM_GetPeriod1(PWMAlarmG_HW, PWMAlarmG_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmG_EnablePeriodSwap
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_EnablePeriodSwap() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmG_EnablePeriodSwap(bool enable)
{
    Cy_TCPWM_PWM_EnablePeriodSwap(PWMAlarmG_HW, PWMAlarmG_CNT_NUM, enable);
}


/*******************************************************************************
* Function Name: PWMAlarmG_TriggerStart
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerStart() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmG_TriggerStart(void)             
{
    Cy_TCPWM_TriggerStart(PWMAlarmG_HW, PWMAlarmG_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarmG_TriggerReload
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerReloadOrIndex() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmG_TriggerReload(void)     
{
    Cy_TCPWM_TriggerReloadOrIndex(PWMAlarmG_HW, PWMAlarmG_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarmG_TriggerKill
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerStopOrKill() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmG_TriggerKill(void)
{
    Cy_TCPWM_TriggerStopOrKill(PWMAlarmG_HW, PWMAlarmG_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarmG_TriggerSwap
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerCaptureOrSwap() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmG_TriggerSwap(void)     
{
    Cy_TCPWM_TriggerCaptureOrSwap(PWMAlarmG_HW, PWMAlarmG_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarmG_GetInterruptStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmG_GetInterruptStatus(void)       
{
    return(Cy_TCPWM_GetInterruptStatus(PWMAlarmG_HW, PWMAlarmG_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmG_ClearInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_ClearInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmG_ClearInterrupt(uint32_t source)     
{
    Cy_TCPWM_ClearInterrupt(PWMAlarmG_HW, PWMAlarmG_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: PWMAlarmG_SetInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmG_SetInterrupt(uint32_t source)
{
    Cy_TCPWM_SetInterrupt(PWMAlarmG_HW, PWMAlarmG_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: PWMAlarmG_SetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmG_SetInterruptMask(uint32_t mask)     
{
    Cy_TCPWM_SetInterruptMask(PWMAlarmG_HW, PWMAlarmG_CNT_NUM, mask);
}


/*******************************************************************************
* Function Name: PWMAlarmG_GetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmG_GetInterruptMask(void)         
{
    return(Cy_TCPWM_GetInterruptMask(PWMAlarmG_HW, PWMAlarmG_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmG_GetInterruptStatusMasked
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatusMasked() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmG_GetInterruptStatusMasked(void)
{
    return(Cy_TCPWM_GetInterruptStatusMasked(PWMAlarmG_HW, PWMAlarmG_CNT_NUM));
}

#endif /* PWMAlarmG_CY_TCPWM_PWM_PDL_H */


/* [] END OF FILE */
