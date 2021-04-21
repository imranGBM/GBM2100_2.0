/*******************************************************************************
* File Name: PWMAlarmR.h
* Version 1.0
*
* Description:
*  This file provides constants and parameter values for the PWMAlarmR
*  component.
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(PWMAlarmR_CY_TCPWM_PWM_PDL_H)
#define PWMAlarmR_CY_TCPWM_PWM_PDL_H

#include "cyfitter.h"
#include "tcpwm/cy_tcpwm_pwm.h"

   
/*******************************************************************************
* Variables
*******************************************************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t  PWMAlarmR_initVar;
extern cy_stc_tcpwm_pwm_config_t const PWMAlarmR_config;
/** @} group_globals */


/***************************************
*        Function Prototypes
****************************************/
/**
* \addtogroup group_general
* @{
*/
void PWMAlarmR_Start(void);
__STATIC_INLINE cy_en_tcpwm_status_t PWMAlarmR_Init(cy_stc_tcpwm_pwm_config_t const *config);
__STATIC_INLINE void PWMAlarmR_DeInit(void);
__STATIC_INLINE void PWMAlarmR_Enable(void);
__STATIC_INLINE void PWMAlarmR_Disable(void);
__STATIC_INLINE uint32_t PWMAlarmR_GetStatus(void);
__STATIC_INLINE void PWMAlarmR_SetCompare0(uint32_t compare0);
__STATIC_INLINE uint32_t PWMAlarmR_GetCompare0(void);
__STATIC_INLINE void PWMAlarmR_SetCompare1(uint32_t compare1);
__STATIC_INLINE uint32_t PWMAlarmR_GetCompare1(void);
__STATIC_INLINE void PWMAlarmR_EnableCompareSwap(bool enable);
__STATIC_INLINE void PWMAlarmR_SetCounter(uint32_t count);
__STATIC_INLINE uint32_t PWMAlarmR_GetCounter(void);
__STATIC_INLINE void PWMAlarmR_SetPeriod0(uint32_t period0);
__STATIC_INLINE uint32_t PWMAlarmR_GetPeriod0(void);
__STATIC_INLINE void PWMAlarmR_SetPeriod1(uint32_t period1);
__STATIC_INLINE uint32_t PWMAlarmR_GetPeriod1(void);
__STATIC_INLINE void PWMAlarmR_EnablePeriodSwap(bool enable);
__STATIC_INLINE void PWMAlarmR_TriggerStart(void);
__STATIC_INLINE void PWMAlarmR_TriggerReload(void);
__STATIC_INLINE void PWMAlarmR_TriggerKill(void);
__STATIC_INLINE void PWMAlarmR_TriggerSwap(void);
__STATIC_INLINE uint32_t PWMAlarmR_GetInterruptStatus(void);
__STATIC_INLINE void PWMAlarmR_ClearInterrupt(uint32_t source);
__STATIC_INLINE void PWMAlarmR_SetInterrupt(uint32_t source);
__STATIC_INLINE void PWMAlarmR_SetInterruptMask(uint32_t mask);
__STATIC_INLINE uint32_t PWMAlarmR_GetInterruptMask(void);
__STATIC_INLINE uint32_t PWMAlarmR_GetInterruptStatusMasked(void);
/** @} general */


/***************************************
*           API Constants
***************************************/

/**
* \addtogroup group_macros
* @{
*/
/** This is a ptr to the base address of the TCPWM instance */
#define PWMAlarmR_HW                 (PWMAlarmR_TCPWM__HW)

/** This is a ptr to the base address of the TCPWM CNT instance */
#define PWMAlarmR_CNT_HW             (PWMAlarmR_TCPWM__CNT_HW)

/** This is the counter instance number in the selected TCPWM */
#define PWMAlarmR_CNT_NUM            (PWMAlarmR_TCPWM__CNT_IDX)

/** This is the bit field representing the counter instance in the selected TCPWM */
#define PWMAlarmR_CNT_MASK           (1UL << PWMAlarmR_CNT_NUM)
/** @} group_macros */

#define PWMAlarmR_INPUT_MODE_MASK    (0x3U)
#define PWMAlarmR_INPUT_DISABLED     (7U)


/*******************************************************************************
* Function Name: PWMAlarmR_Init
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_tcpwm_status_t PWMAlarmR_Init(cy_stc_tcpwm_pwm_config_t const *config)
{
    return(Cy_TCPWM_PWM_Init(PWMAlarmR_HW, PWMAlarmR_CNT_NUM, config));
}


/*******************************************************************************
* Function Name: PWMAlarmR_DeInit
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmR_DeInit(void)                   
{
    Cy_TCPWM_PWM_DeInit(PWMAlarmR_HW, PWMAlarmR_CNT_NUM, &PWMAlarmR_config);
}

/*******************************************************************************
* Function Name: PWMAlarmR_Enable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Enable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmR_Enable(void)                   
{
    Cy_TCPWM_Enable_Multiple(PWMAlarmR_HW, PWMAlarmR_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarmR_Disable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Disable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmR_Disable(void)                  
{
    Cy_TCPWM_Disable_Multiple(PWMAlarmR_HW, PWMAlarmR_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarmR_GetStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmR_GetStatus(void)                
{
    return(Cy_TCPWM_PWM_GetStatus(PWMAlarmR_HW, PWMAlarmR_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmR_SetCompare0
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetCompare0() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmR_SetCompare0(uint32_t compare0)      
{
    Cy_TCPWM_PWM_SetCompare0(PWMAlarmR_HW, PWMAlarmR_CNT_NUM, compare0);
}


/*******************************************************************************
* Function Name: PWMAlarmR_GetCompare0
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetCompare0() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmR_GetCompare0(void)              
{
    return(Cy_TCPWM_PWM_GetCompare0(PWMAlarmR_HW, PWMAlarmR_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmR_SetCompare1
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetCompare1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmR_SetCompare1(uint32_t compare1)      
{
    Cy_TCPWM_PWM_SetCompare1(PWMAlarmR_HW, PWMAlarmR_CNT_NUM, compare1);
}


/*******************************************************************************
* Function Name: PWMAlarmR_GetCompare1
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetCompare1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmR_GetCompare1(void)              
{
    return(Cy_TCPWM_PWM_GetCompare1(PWMAlarmR_HW, PWMAlarmR_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmR_EnableCompareSwap
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_EnableCompareSwap() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmR_EnableCompareSwap(bool enable)  
{
    Cy_TCPWM_PWM_EnableCompareSwap(PWMAlarmR_HW, PWMAlarmR_CNT_NUM, enable);
}


/*******************************************************************************
* Function Name: PWMAlarmR_SetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmR_SetCounter(uint32_t count)          
{
    Cy_TCPWM_PWM_SetCounter(PWMAlarmR_HW, PWMAlarmR_CNT_NUM, count);
}


/*******************************************************************************
* Function Name: PWMAlarmR_GetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmR_GetCounter(void)               
{
    return(Cy_TCPWM_PWM_GetCounter(PWMAlarmR_HW, PWMAlarmR_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmR_SetPeriod0
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetPeriod0() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmR_SetPeriod0(uint32_t period0)          
{
    Cy_TCPWM_PWM_SetPeriod0(PWMAlarmR_HW, PWMAlarmR_CNT_NUM, period0);
}


/*******************************************************************************
* Function Name: PWMAlarmR_GetPeriod0
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetPeriod0() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmR_GetPeriod0(void)                
{
    return(Cy_TCPWM_PWM_GetPeriod0(PWMAlarmR_HW, PWMAlarmR_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmR_SetPeriod1
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_SetPeriod1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmR_SetPeriod1(uint32_t period1)
{
    Cy_TCPWM_PWM_SetPeriod1(PWMAlarmR_HW, PWMAlarmR_CNT_NUM, period1);
}


/*******************************************************************************
* Function Name: PWMAlarmR_GetPeriod1
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_GetPeriod1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmR_GetPeriod1(void)                
{
    return(Cy_TCPWM_PWM_GetPeriod1(PWMAlarmR_HW, PWMAlarmR_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmR_EnablePeriodSwap
****************************************************************************//**
*
* Invokes the Cy_TCPWM_PWM_EnablePeriodSwap() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmR_EnablePeriodSwap(bool enable)
{
    Cy_TCPWM_PWM_EnablePeriodSwap(PWMAlarmR_HW, PWMAlarmR_CNT_NUM, enable);
}


/*******************************************************************************
* Function Name: PWMAlarmR_TriggerStart
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerStart() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmR_TriggerStart(void)             
{
    Cy_TCPWM_TriggerStart(PWMAlarmR_HW, PWMAlarmR_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarmR_TriggerReload
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerReloadOrIndex() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmR_TriggerReload(void)     
{
    Cy_TCPWM_TriggerReloadOrIndex(PWMAlarmR_HW, PWMAlarmR_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarmR_TriggerKill
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerStopOrKill() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmR_TriggerKill(void)
{
    Cy_TCPWM_TriggerStopOrKill(PWMAlarmR_HW, PWMAlarmR_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarmR_TriggerSwap
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerCaptureOrSwap() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmR_TriggerSwap(void)     
{
    Cy_TCPWM_TriggerCaptureOrSwap(PWMAlarmR_HW, PWMAlarmR_CNT_MASK);
}


/*******************************************************************************
* Function Name: PWMAlarmR_GetInterruptStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmR_GetInterruptStatus(void)       
{
    return(Cy_TCPWM_GetInterruptStatus(PWMAlarmR_HW, PWMAlarmR_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmR_ClearInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_ClearInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmR_ClearInterrupt(uint32_t source)     
{
    Cy_TCPWM_ClearInterrupt(PWMAlarmR_HW, PWMAlarmR_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: PWMAlarmR_SetInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmR_SetInterrupt(uint32_t source)
{
    Cy_TCPWM_SetInterrupt(PWMAlarmR_HW, PWMAlarmR_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: PWMAlarmR_SetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void PWMAlarmR_SetInterruptMask(uint32_t mask)     
{
    Cy_TCPWM_SetInterruptMask(PWMAlarmR_HW, PWMAlarmR_CNT_NUM, mask);
}


/*******************************************************************************
* Function Name: PWMAlarmR_GetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmR_GetInterruptMask(void)         
{
    return(Cy_TCPWM_GetInterruptMask(PWMAlarmR_HW, PWMAlarmR_CNT_NUM));
}


/*******************************************************************************
* Function Name: PWMAlarmR_GetInterruptStatusMasked
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatusMasked() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t PWMAlarmR_GetInterruptStatusMasked(void)
{
    return(Cy_TCPWM_GetInterruptStatusMasked(PWMAlarmR_HW, PWMAlarmR_CNT_NUM));
}

#endif /* PWMAlarmR_CY_TCPWM_PWM_PDL_H */


/* [] END OF FILE */
