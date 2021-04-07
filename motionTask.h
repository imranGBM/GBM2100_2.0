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
#pragma once

#include "event_groups.h"

extern EventGroupHandle_t systemInputMode;  //This will actually be instantiated in main cm4.c

//Cette partie est importante si on veut swap entre mode capsense et motion
#define MODE_MOTION (1<<0)              // shifts of 0
#define MODE_CAPSENSE (1<<1)           //<< is the shift operator      shift of 1 so value changes

void motionTask(void *arg);
/* [] END OF FILE */
