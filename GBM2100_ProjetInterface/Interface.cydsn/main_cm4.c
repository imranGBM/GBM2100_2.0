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
#include "TasksInterface.h"
#include "traitementSignal.h"
//Communication
#include "Max30102_task.h"
#include "motionTask.h"
#include "bmi160.h"
//weird shit
#include "event_groups.h"
EventGroupHandle_t systemInputMode;     //make a variable for the event group called systemInputMode.
//End


int main(void)
{   
    __enable_irq();
    CapSense_Start();
    CapSense_ScanAllWidgets();
    GUI_Init(); Cy_EINK_Start(20); Cy_EINK_Power(1); GUI_SetColor(GUI_BLACK); GUI_SetBkColor(GUI_WHITE); GUI_Clear(); 
    //weird shit communication
    systemInputMode = xEventGroupCreate();          //initialize event group
    Cy_GPIO_Write(LED8_PORT,LED8_NUM,1);            //Turn off LED on startup
    xEventGroupSetBits(systemInputMode,MODE_CAPSENSE);      //set the current mode to CapSense
    //end
    xTaskCreate(CapSense_ChangeMenu,"CapSense_ChangeMenu",configMINIMAL_STACK_SIZE,NULL,3,NULL);
    xTaskCreate(HeartRateAlarm,"HeartRateAlarm",configMINIMAL_STACK_SIZE,NULL,2,NULL);
    xTaskCreate(motionTask,"motionTask",configMINIMAL_STACK_SIZE,NULL,1,NULL);        //Start motionTask
    //xTaskCreate(max30102_task,"max30102_task",1024,0,1,0);  //Start SpO2
    
    vTaskStartScheduler();
    for(;;)
    {
    }
}

/* [] END OF FILE */
