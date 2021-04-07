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

//---------------------------------------------------- Fonctions d'affichage / Fonctions appelées dans les tâches ------------------------------------------------------

// 1)Fonction pour mettre à jour l'écran
uint8 imageBufferCache[CY_EINK_FRAME_SIZE] = {0};
void UpdateDisplay(cy_eink_update_t updateMethod, bool powerCycle)
{
    cy_eink_frame_t* pEmwinBuffer;
    pEmwinBuffer = (cy_eink_frame_t*)LCD_GetDisplayBuffer();
    Cy_EINK_ShowFrame(imageBufferCache, pEmwinBuffer, updateMethod, powerCycle);
    memcpy(imageBufferCache, pEmwinBuffer, CY_EINK_FRAME_SIZE);
}

// 2)Fonction pour effacer l'écran
void ClearScreen(void)
{
    GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}

// 3)Fonction pour tracer le graphe d'un vecteur de 750 éléments
void drawGraph(float *vector750elements) 
{
    int x=0; uint32_t max=0;
    float vector250elements[250];
    for (int i=0; i < 250; i++)
    {
        vector250elements[i]=vector750elements[3*i];
        if (vector250elements[i]>max){
            max=vector250elements[i];
        }
    }
    for (int i=0; i < 250; i++)
    {
        int y0=round((vector250elements[i])/max); 
        GUI_DrawPoint(x,y0); 
        if (i<249)
        {
            int y1=round((vector250elements[i+1])/max);
            GUI_DrawLine(x,y0,(x+1),y1); 
        }
        x++;   
    }
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}

// 4)Fonction pour afficher des données à jour (param1 et param2) sur l'écran
void updateParameters(int param1, int param2) 
{
    char Parametre1[6]; char Parametre2[6];
    sprintf(Parametre1,"%d",param1); sprintf(Parametre2,"%d",param2); 
    GUI_SetPenSize(8); GUI_DrawRect(10,155,255,175); 
    GUI_SetPenSize(6); GUI_SetFont(GUI_FONT_10_1); 
    GUI_DispStringAt("Oxygen saturation:",20,160); GUI_DispStringAt(Parametre1,90,160); GUI_DispStringAt("%",100,160); 
    GUI_DispStringAt("Heart rate:",150,160); GUI_DispStringAt(Parametre2,210,160); GUI_DispStringAt("bpm",230,160); 
    UpdateDisplay(CY_EINK_FULL_4STAGE, true); 
}

// 5)Fonction pour afficher la valeur inférieure du rythme cardiaque définie par l'utilisateur
void updateLowHeartRateLimit(int HeartRate)
{
    ClearScreen();
    char HeartRateLow[6];
    sprintf(HeartRateLow,"%d",HeartRate);
    GUI_SetPenSize(18); GUI_SetFont(GUI_FONT_24_1); 
    GUI_DispStringAt(HeartRateLow,115,90); 
    GUI_SetPenSize(7); GUI_SetFont(GUI_FONT_13_1); 
    GUI_DispStringAt("Low heart rate limit",80,20);
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}

// 6)Fonction pour afficher la valeur supérieure du rythme cardiaque définie par l'utilisatuer
void updateHighHeartRateLimit(int HeartRate)
{
    ClearScreen();
    char HeartRateHigh[6];
    sprintf(HeartRateHigh,"%d",HeartRate);
    GUI_SetPenSize(18); GUI_SetFont(GUI_FONT_24_1); 
    GUI_DispStringAt(HeartRateHigh,115,90); 
    GUI_SetPenSize(7); GUI_SetFont(GUI_FONT_13_1); 
    GUI_DispStringAt("High heart rate limit",80,20);
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}

// 7)Fonction pour afficher la page actuelle et le curseur définissant le paramètre à modifier dans la page
void DisplayPage(int PageNumber, int ParamP1, int ParamP2){
    ClearScreen();
    GUI_SetPenSize(7); GUI_SetFont(GUI_FONT_13_1);
    if (PageNumber==0)
    {
        GUI_DispStringAt("Press SW2 to show graphs",20,20);
        GUI_DispStringAt("Use the CapSense buttons to modify parameters",20,50);
    }
    if (PageNumber==1)
    {
        GUI_DispStringAt("Parameters page 1",80,0);
        GUI_DispStringAt("1)Change LED intensity",30,50);
        GUI_DispStringAt("2)Enable/Disable motion sensor alarm",30,80);
        GUI_DispStringAt("3)Enable/Disable heart rate alarm",30,110);
        GUI_DispStringAt("4)Enable/Disable oxygen saturation alarm",30,140);
        if (ParamP1==1)
        {
            GUI_DrawCircle(15,55,5);
        }
        if (ParamP1==2)
        {
            GUI_DrawCircle(15,85,5);
        }
        if (ParamP1==3)
        {
            GUI_DrawCircle(15,115,5);
        }
        if (ParamP1==4)
        {
            GUI_DrawCircle(15,145,5);
        }
    }
    if (PageNumber==2)
    {
        
        GUI_DispStringAt("Parameters page 2",80,0);
        GUI_DispStringAt("1)Set low heart rate limit",30,50);
        GUI_DispStringAt("2)Set high heart rate limit",30,80);
        if (ParamP2==1)
        {
            GUI_DrawCircle(15,55,5);
        }
        if (ParamP2==2)
        {
            GUI_DrawCircle(15,85,5);
        }
    }
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}

// 8)Fonction pour changer l'intensité de la LED du capteur SpO2
volatile uint32_t SliderPosI=0;
uint32_t CapSense_ChangeLedIntensity()
{      
        CapSense_ProcessAllWidgets();
        SliderPosI=CapSense_GetCentroidPos(CapSense_LINEARSLIDER0_WDGT_ID);
        if (SliderPosI<65534)
        {
            SliderPosI=100-SliderPosI;
        }
        SliderPosI=0;
        CapSense_UpdateAllBaselines();
        CapSense_ScanAllWidgets();
        if (CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID)){
            return 1;
        }
        else{
            return 0;
        }
}

// 9)Fonction pour changer la valeur inférieure du rythme cardiaque définie par l'utilisateur
volatile int LowHeartRateI=60; volatile int LowHeartRateF=60; volatile uint32_t SliderPosIII=50; volatile bool StopChangeLowHeartRate=false;
void CapSense_ChangeLowHeartRateLimit()
{      
    CapSense_ProcessAllWidgets();
    SliderPosIII=CapSense_GetCentroidPos(CapSense_LINEARSLIDER0_WDGT_ID);
    if (SliderPosIII<65534&&SliderPosIII>=80)
    {
        LowHeartRateI=LowHeartRateI+2;
    }
    if (SliderPosIII<80&&SliderPosIII>50)
    {
        LowHeartRateI++;
    }
    if (SliderPosIII<50&&SliderPosIII>20)
    {
        LowHeartRateI--;
    }
    if (SliderPosIII<=20)
    {
        LowHeartRateI=LowHeartRateI-2;
    }
    SliderPosIII=50;
    CapSense_UpdateAllBaselines();
    CapSense_ScanAllWidgets();
    if (LowHeartRateI != LowHeartRateF)
    {
        updateLowHeartRateLimit(LowHeartRateI);
    }
    LowHeartRateF=LowHeartRateI;
    CapSense_ProcessAllWidgets();
    if (CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID)){
        StopChangeLowHeartRate=true;
    }
}

// 10)Fonction pour changer la valeur supérieure du rythme cardiaque définie par l'utilisateur
volatile int HighHeartRateI=70; volatile int HighHeartRateF=70; volatile uint32_t SliderPosIV=50; volatile bool StopChangeHighHeartRate=false;
void CapSense_ChangeHighHeartRateLimit()
{      
    CapSense_ProcessAllWidgets();
    SliderPosIV=CapSense_GetCentroidPos(CapSense_LINEARSLIDER0_WDGT_ID);
    if (SliderPosIV<65534&&SliderPosIV>=80)
    {
        HighHeartRateI=HighHeartRateI+2;
    }
    if (SliderPosIV<80&&SliderPosIV>50)
    {
        HighHeartRateI++;
    }
    if (SliderPosIV<50&&SliderPosIV>20)
    {
        HighHeartRateI--;
    }
    if (SliderPosIV<=20)
    {
        HighHeartRateI=HighHeartRateI-2;
    }
    SliderPosIV=50;
    CapSense_UpdateAllBaselines();
    CapSense_ScanAllWidgets();
    if (HighHeartRateI != HighHeartRateF)
    {
        updateHighHeartRateLimit(HighHeartRateI);
    }
    HighHeartRateF=HighHeartRateI;
    CapSense_ProcessAllWidgets();
    if (CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID)){
        StopChangeHighHeartRate=true;
    }
}

// 11)Fonction pour afficher l'état de l'alarme (enabled/disabled) issue de l'accéléromètre
volatile bool MotionAlarmEnable=true;
void updateMotionAlarm(){
    ClearScreen();GUI_SetPenSize(7); GUI_SetFont(GUI_FONT_13_1);
    if (MotionAlarmEnable==true)
    {
        GUI_DispStringAt("Alarm enabled",80,80);
    }
    if (MotionAlarmEnable==false)
    {
        GUI_DispStringAt("Alarm disabled",80,80);
    }
    UpdateDisplay(CY_EINK_FULL_4STAGE, true);
}

// 12)Fonction pour changer un flag correspondant à l'état de l'alarme (enabled/disabled) issue de l'accéléromètre
volatile uint32_t SliderPosV=50; volatile bool StopDisableAlarm=false;
void CapSense_EnableDisableMotionAlarm()
{      
    CapSense_ProcessAllWidgets();
    SliderPosV=CapSense_GetCentroidPos(CapSense_LINEARSLIDER0_WDGT_ID);
    if (SliderPosV<65534&&SliderPosV>50)
    {
        MotionAlarmEnable=true;
        updateMotionAlarm();
    }
    if (SliderPosV<50)
    {
        MotionAlarmEnable=false;
        updateMotionAlarm();
    }
    SliderPosV=50;
    CapSense_UpdateAllBaselines();
    CapSense_ScanAllWidgets();
    CapSense_ProcessAllWidgets();
    if (CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID)){
        StopDisableAlarm=true;
    }
}

//------------------------------------------------------------------- Tâches ----------------------------------------------------------------

// 1)Tâche pour afficher les graphiques ainsi que pour changer la courbe affichée 
volatile int CompteurSW2; volatile bool AffichageGraph; float bufferInfrarouge[750]; float bufferRouge[750]; int Saturation=0; int RythmeCardiaque=65;
void ChangeGraph()
{
    for (;;)
    {
        //Remplissage des buffers des courbes rouge et infrarouge et des paramètres
        if (AffichageGraph==true){
            ClearScreen();
            if (CompteurSW2%2==0)
            {
                drawGraph(bufferInfrarouge);
                updateParameters(Saturation, RythmeCardiaque);
                GUI_SetPenSize(10); GUI_SetFont(GUI_FONT_16_1); GUI_DispStringAt("Red curve:",80,0);
            }
            else
            {
                drawGraph(bufferRouge);
                updateParameters(Saturation, RythmeCardiaque);
                GUI_SetPenSize(10); GUI_SetFont(GUI_FONT_16_1); GUI_DispStringAt("Infrared curve:",60,0);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

// 2)Tâche pour allumer ou éteindre une alarme lorsque le rythme cardiaque est à l'extérieur des bornes définies par l'usager 
void HeartRateAlarm()
{
    for(;;)
    {
        if (RythmeCardiaque>HighHeartRateI || RythmeCardiaque<LowHeartRateI)
        {
            Cy_GPIO_Write(Blue_0_PORT,Blue_0_NUM,0);
        }
        else
        {
            Cy_GPIO_Write(Blue_0_PORT,Blue_0_NUM,1);
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

// 3)Tâche pour naviguer entre les pages et les paramètres en fonction des boutons appuyés par l'utilisateur
/*
Page1: 4 paramètres; 1)ChangeLedIntensity 2)Enable/Disable GLed 3)Enable/Disable BLed 4)Enable/Disable RLed
Page2: 2 paramètres; 1)Borne inférieure bpm 2)Borne supérieure bpm 
*/
volatile int Page1Params=0;
volatile int Page2Params=0;
volatile int ScreenNumber=0;
void CapSense_ChangeMenu()
{   
    for(;;)
    {   
        volatile int SliderPosII=50;
        CapSense_ProcessAllWidgets();
        if(CapSense_IsWidgetActive(CapSense_BUTTON0_WDGT_ID))
        {
            AffichageGraph=false;
            if (Page1Params==0&&Page2Params==0){
                ScreenNumber--;
                if (ScreenNumber<0)
                {
                    ScreenNumber=2; 
                }
                DisplayPage(ScreenNumber, Page1Params, Page2Params);
            }
        }
        if(CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID))
        {
            AffichageGraph=false;
            if (Page1Params==0&&Page2Params==0){
                ScreenNumber++;
                if (ScreenNumber>2)
                {
                    ScreenNumber=0; 
                }
            DisplayPage(ScreenNumber, Page1Params,Page2Params);
            }
        }
        if (ScreenNumber==1)
        { 
            CapSense_ProcessAllWidgets();
            SliderPosII=CapSense_GetCentroidPos(CapSense_LINEARSLIDER0_WDGT_ID);
            if (SliderPosII>50 && SliderPosII<65535)
            {
                Page1Params++;
                if (Page1Params>4)
                {
                    Page1Params=0;
                }
                DisplayPage(ScreenNumber,Page1Params,Page2Params);
            }
            if (SliderPosII<50)
            {
                Page1Params--;
                if (Page1Params<0)
                {
                    Page1Params=4;
                }  
                DisplayPage(ScreenNumber,Page1Params,Page2Params);
            }
            if (Page1Params==2)
            {
                CapSense_ProcessAllWidgets();
                if(CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID))
                {
                    updateMotionAlarm();
                    while (StopDisableAlarm==false)
                    {
                        CapSense_EnableDisableMotionAlarm();
                        vTaskDelay(pdMS_TO_TICKS(200));
                    }
                    StopDisableAlarm=false;
                    DisplayPage(ScreenNumber,Page1Params,Page2Params);
                }
            }
        }
        if (ScreenNumber==2)
        { 
            CapSense_ProcessAllWidgets();
            SliderPosII=CapSense_GetCentroidPos(CapSense_LINEARSLIDER0_WDGT_ID);
            if (SliderPosII>50 && SliderPosII<65535)
            {
                Page2Params++;
                if (Page2Params>2)
                {
                    Page2Params=0;
                }
                DisplayPage(ScreenNumber,Page1Params,Page2Params);
            }
            if (SliderPosII<50)
            {
                Page2Params--;

                if (Page2Params<0)
                {
                    Page2Params=2;
                }
                DisplayPage(ScreenNumber,Page1Params,Page2Params);                
            }
            if (Page2Params==1)
            {
                CapSense_ProcessAllWidgets();
                if(CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID))
                {
                    updateLowHeartRateLimit(LowHeartRateI);
                    while (StopChangeLowHeartRate==false)
                    {
                        CapSense_ChangeLowHeartRateLimit();
                        vTaskDelay(pdMS_TO_TICKS(200));
                    }
                    StopChangeLowHeartRate=false;
                    DisplayPage(ScreenNumber,Page1Params,Page2Params);
                }
            }
            if (Page2Params==2)
            {
                CapSense_ProcessAllWidgets();
                if(CapSense_IsWidgetActive(CapSense_BUTTON1_WDGT_ID))
                {
                    updateHighHeartRateLimit(HighHeartRateI);
                    while (StopChangeHighHeartRate==false)
                    {
                        CapSense_ChangeHighHeartRateLimit();
                        vTaskDelay(pdMS_TO_TICKS(200));
                    }
                    StopChangeHighHeartRate=false;
                    DisplayPage(ScreenNumber,Page1Params,Page2Params);
                }
            }
            
        }
        CapSense_UpdateAllBaselines();
        CapSense_ScanAllWidgets();
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}