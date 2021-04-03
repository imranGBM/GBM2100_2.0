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
//Communication
#include <stdio.h>
#include "bmi160.h"     //proviens du driver sur github du capteur
#include "motionTask.h"
#include "event_groups.h"
EventGroupHandle_t systemInputMode;     //make a variable for the event group called systemInputMode.
//END


volatile int CompteurSW2=0;
volatile bool AffichageGraph=false;
void isr_SW2()
{
    Cy_GPIO_ClearInterrupt(PinSW2_0_PORT,PinSW2_0_NUM);
    NVIC_ClearPendingIRQ(SW2_cfg.intrSrc);
    CompteurSW2++; 
    AffichageGraph=true; 
}

int main(void)
{   
    __enable_irq();
    
    //Communication
    UART_1_Start();
    systemInputMode = xEventGroupCreate();          //initialize event group
    Cy_GPIO_Write(LED8_PORT,LED8_NUM,1);            //Turn off LED on startup
    xEventGroupSetBits(systemInputMode,MODE_CAPSENSE);      //set the current mode to CapSense
    xTaskCreate(motionTask,"motionTask",1024,0,1,0);        //Start motionTask
    //END
    
    Cy_SysInt_Init(&SW2_cfg, isr_SW2);
    NVIC_ClearPendingIRQ(SW2_cfg.intrSrc);
    NVIC_EnableIRQ(SW2_cfg.intrSrc);
    CapSense_Start();
    CapSense_ScanAllWidgets();
    GUI_Init(); Cy_EINK_Start(20); Cy_EINK_Power(1); GUI_SetColor(GUI_BLACK); GUI_SetBkColor(GUI_WHITE); GUI_Clear(); 
    PWMAlarmG_Start(); 
    xTaskCreate(CapSense_ChangeMenu,"CapSense_ChangeMenu",configMINIMAL_STACK_SIZE,NULL,1,NULL);
    xTaskCreate(ChangeGraph,"ChangeGraph",configMINIMAL_STACK_SIZE,NULL,1,NULL);
    vTaskStartScheduler();
    for(;;)
    {
    }
}

/* [] END OF FILE */
