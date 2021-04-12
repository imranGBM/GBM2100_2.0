#include "project.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "GUI.h"
#include "pervasive_eink_hardware_driver.h"
#include "cy_eink_library.h"
#include "LCDConf.h"
#include <stdlib.h>
#include "math.h"

void UpdateDisplay(cy_eink_update_t updateMethod, bool powerCycle);
void ClearScreen();
void drawGraph(float*vector150elements);
void updateParameters(int param1, int param2); 
void updateLowHeartRateLimit(int HeartRate); 
void updateHighHeartRateLimit(int HeartRate);
void DisplayPage(int PageNumber, int ParamP1, int ParamP2);
void CapSense_ChangeLedIntensity();
void CapSense_ChangeLowHeartRateLimit();
void CapSense_ChangeHighHeartRateLimit();
void updateMotionAlarm();
void CapSense_EnableDisableMotionAlarm();
void ChangeGraph();
void HeartRateAlarm();
void CapSense_ChangeMenu();
