#include "project.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "bmi160.h"         //proviens du driver sur github du capteur
// New PART 2 bmi
#include "motionTask.h"
#include "event_groups.h"
//MAX30102
#include "Max30102_task.h"

EventGroupHandle_t systemInputMode;     //make a variable for the event group called systemInputMode.

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */

    UART_1_Start();
    //PART 2
    systemInputMode = xEventGroupCreate();          //initialize event group
    Cy_GPIO_Write(LED8_PORT,LED8_NUM,1);            //Turn off LED on startup
    xEventGroupSetBits(systemInputMode,MODE_CAPSENSE);      //set the current mode to CapSense
    //xTaskCreate(motionTask,"motionTask",1024,0,1,0);        //Start motionTask
    
    //MAX30102
    xTaskCreate(max30102_task,"max30102_task",1024,0,1,0);
    
    
    
    vTaskStartScheduler();

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
