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

#include "project.h"
#include "GUI.h"
#include "pervasive_eink_hardware_driver.h"
#include "cy_eink_library.h"
#include "LCDConf.h"
#include <stdlib.h>
#include <arm_math.h>
#include<core_cm4.h>
#include "TasksInterface.h"

#define BLOCK_SIZE 750
#define NUM_TAPS 50


//void filtre(int32_t signal[],float32_t Coefficients[]);

void traitement_signal();



/* [] END OF FILE */
