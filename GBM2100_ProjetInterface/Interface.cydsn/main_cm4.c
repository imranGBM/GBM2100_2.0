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
