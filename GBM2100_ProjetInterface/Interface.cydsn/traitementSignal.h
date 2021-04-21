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

#define BLOCK_SIZE 750 // Longueur du vecteur traité 
#define NUM_TAPS 50 // Nombre de coefficients dans le vecteur de coefficients fir des filtres

void traitement_signal();



/* [] END OF FILE */
