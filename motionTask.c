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
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "bmi160.h"         //proviens du driver sur github du capteur
//Part 2
#include "motionTask.h"
#include "math.h"
//END

static struct bmi160_dev bmi160Dev;  //Used as the interface point to my capteur

//uint8_t, car c un 8bit adress
//dev_addr = I2C address,   reg_addr = the register I want to write     *data = the data I want to write    len = the nb of bytes I want to write
//def des fct ds OneNote

static int8_t BMI160BurstWrite (uint8_t dev_addr, uint8_t reg_addr,uint8_t *data, uint16_t len)
{
    Cy_SCB_I2C_MasterSendStart(I2C_1_HW,dev_addr,CY_SCB_I2C_WRITE_XFER,0,&I2C_1_context);
    Cy_SCB_I2C_MasterWriteByte(I2C_1_HW,reg_addr,0,&I2C_1_context);
    for(int i = 0; i<len; i++)
    {
        Cy_SCB_I2C_MasterWriteByte(I2C_1_HW,data[i],0,&I2C_1_context);
    }
    
    Cy_SCB_I2C_MasterSendStop(I2C_1_HW,0,&I2C_1_context);
    
    return 0;
}

//This function supports the BMP180 library and read I2C Registers
static int8_t BMI160BurstRead (uint8_t dev_addr, uint8_t reg_addr,uint8_t *data, uint16_t len)
{
    Cy_SCB_I2C_MasterSendStart(I2C_1_HW,dev_addr,CY_SCB_I2C_WRITE_XFER,0,&I2C_1_context);
    Cy_SCB_I2C_MasterWriteByte(I2C_1_HW,reg_addr,0,&I2C_1_context);
    Cy_SCB_I2C_MasterSendReStart(I2C_1_HW,dev_addr,CY_SCB_I2C_READ_XFER,0,&I2C_1_context);
    for(int i = 0; i<len-1; i++)
    {
        Cy_SCB_I2C_MasterReadByte(I2C_1_HW,CY_SCB_I2C_ACK,&data[i],0,&I2C_1_context);
    }
    Cy_SCB_I2C_MasterReadByte(I2C_1_HW,CY_SCB_I2C_NAK,&data[len-1],0,&I2C_1_context);
    
    Cy_SCB_I2C_MasterSendStop(I2C_1_HW,0,&I2C_1_context);
    
    return 0;
}

static void bmi160Init(void)
{
    vTaskDelay(100); 
    
    /* BMI160 */
    bmi160Dev.read = (bmi160_read_fptr_t)BMI160BurstRead;           //c sa qui read selon un reg_addr, mais how code select sa
    bmi160Dev.write = (bmi160_write_fptr_t)BMI160BurstWrite;        //UPDATE: changer le read en write
    bmi160Dev.delay_ms = (bmi160_delay_fptr_t)vTaskDelay;
    bmi160Dev.id = BMI160_I2C_ADDR; // I2C device address
    
    bmi160_init(&bmi160Dev); // initialize the device
    
    bmi160Dev.gyro_cfg.odr = BMI160_GYRO_ODR_800HZ;
    bmi160Dev.gyro_cfg.range = BMI160_GYRO_RANGE_125_DPS;
    bmi160Dev.gyro_cfg.bw = BMI160_GYRO_BW_OSR4_MODE;
    
    /* Select the power mode of Gyroscope sensor */
    bmi160Dev.gyro_cfg.power = BMI160_GYRO_NORMAL_MODE;
    
    bmi160Dev.accel_cfg.odr = BMI160_ACCEL_ODR_1600HZ;
    bmi160Dev.accel_cfg.range = BMI160_ACCEL_RANGE_2G;
    bmi160Dev.accel_cfg.bw = BMI160_ACCEL_BW_OSR4_AVG1;
    bmi160Dev.accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;
    
    /* Set the sensor configuration */
    bmi160_set_sens_conf(&bmi160Dev);               // save configuration en-haut
    bmi160Dev.delay_ms(50);
    
}

//32768 = 2g
#define MAXACCEL (32768/2)      // 16 bits en int (+ et -), donc de -2^15 à (2^15)-1. On divise par 2 pr les valeurs acquisitionné de -2^14 à (2^14)-1

void motionTask(void *arg)
{
    (void)arg;
    I2C_1_Start();
    bmi160Init();
    struct bmi160_sensor_data acc;
    
    //Part 2 
    TickType_t lastMovement = 0;
    //End
    
    //Part 1 Old Code
    /*float gx,gy,gz;
    
    while(1)                //infinite loop
    {
        bmi160_get_sensor_data(BMI160_ACCEL_ONLY, &acc, NULL, &bmi160Dev);
        
        gx = (float)acc.x/MAXACCEL;
        gy = (float)acc.y/MAXACCEL;
        gz = (float)acc.z/MAXACCEL;
        
        printf("x=%1.2f y=%1.2f z=%1.2f\r\n", gx,gy,gz);
        
        vTaskDelay(200);
    }
    */
    
    //Part 2: normalement faudrait delete le while en haut, mais jveux keep display
    double gx,gy;
    float gz;
    double m1,m2;
    
    while(1)
    {
        bmi160_get_sensor_data(BMI160_ACCEL_ONLY, &acc, NULL, &bmi160Dev);          //c sa qui fait le reading du struct bmi160Dev
        
        //Convert counts to G
        gx = (double)acc.x/MAXACCEL;
        gy = (double)acc.y/MAXACCEL;
        gz = (float)acc.z/MAXACCEL;
        
        //Cap the readings at 1.0g
        if(gx>1.0) gx=1.0;
        if(gx<-1.0) gx=-1.0;
        if(gy>1.0) gy=1.0;
        if(gy<-1.0) gy=-1.0;
        
        if(gz>1.0) gz=1.0;
        if(gz<-1.0) gz=-1.0;
        
        //Calculate a number between 0 and 100 based on the angle
        m1 = acos(gx)*360/(2*M_PI*1.8);
        m2 = acos(gy)*360/(2*M_PI*1.8);
        
        //If the board moves more than X% take the time that it happened
        if (fabs(m1-50.0)>10.0 || fabs(m2-50.0)>10.0){
            lastMovement = xTaskGetTickCount();}
        
        //If it has been more than a second since last movement then turn off alarm (LED and MODE_MOTION)
        if ( (xTaskGetTickCount() - lastMovement) > 1000)
        {
            xEventGroupSetBits(systemInputMode,MODE_CAPSENSE);          //Mode qui signal que ya pas de mouvement
            xEventGroupClearBits(systemInputMode,MODE_MOTION);          //Mode qui signal que ya du mouvement
            Cy_GPIO_Write(LED8_PORT,LED8_NUM,1);                        //ici ya pas eu de mouvement depuis 1000ms, donc turn off alarm (LED)
        }
        else
        {
            xEventGroupClearBits(systemInputMode,MODE_CAPSENSE);
            xEventGroupSetBits(systemInputMode,MODE_MOTION);
            Cy_GPIO_Write(LED8_PORT,LED8_NUM,0);                        //ici ya pas eu de mouvement depuis 1000ms, donc turn off alarm (LED)
        }
        
        if(xEventGroupGetBits(systemInputMode) == MODE_MOTION)
        {
            printf("x=%1.2f y=%1.2f z=%1.2f\r\n", gx,gy,gz);
            vTaskDelay(200);
        }
        
    }
}


/* [] END OF FILE */
