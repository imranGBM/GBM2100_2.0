#include "project.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "bmi160.h"         //proviens du driver sur github du capteur
#include "motionTask.h"
#include "math.h"


volatile bool MotionAlarmEnable;

static struct bmi160_dev bmi160Dev;  //Used as the interface point to my capteur

//uint8_t, car c un 8bit adress
//dev_addr = I2C address,   reg_addr = the register I want to write     *data = the data I want to write    len = the nb of bytes I want to write
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
    bmi160Dev.read = (bmi160_read_fptr_t)BMI160BurstRead;           
    bmi160Dev.write = (bmi160_write_fptr_t)BMI160BurstWrite;        
    bmi160Dev.delay_ms = (bmi160_delay_fptr_t)vTaskDelay;
    bmi160Dev.id = BMI160_I2C_ADDR; 
    
    bmi160_init(&bmi160Dev); 
    
    bmi160Dev.gyro_cfg.odr = BMI160_GYRO_ODR_800HZ;
    bmi160Dev.gyro_cfg.range = BMI160_GYRO_RANGE_125_DPS;
    bmi160Dev.gyro_cfg.bw = BMI160_GYRO_BW_OSR4_MODE;
    
    bmi160Dev.gyro_cfg.power = BMI160_GYRO_NORMAL_MODE;
    
    bmi160Dev.accel_cfg.odr = BMI160_ACCEL_ODR_1600HZ;              
    bmi160Dev.accel_cfg.range = BMI160_ACCEL_RANGE_2G;
    bmi160Dev.accel_cfg.bw = BMI160_ACCEL_BW_OSR4_AVG1;
    bmi160Dev.accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;
    
    bmi160_set_sens_conf(&bmi160Dev);               
    bmi160Dev.delay_ms(50);
    
}

//32768 = 2g
#define MAXACCEL (32768)      

void motionTask(void *arg)
{
    (void)arg;
    I2C_1_Start();
    bmi160Init();
    struct bmi160_sensor_data acc;
   
    TickType_t lastMovement = 0;
    
    bmi160_get_sensor_data(BMI160_ACCEL_ONLY, &acc, NULL, &bmi160Dev);
    
    double gx[2],gy[2],gz[2];
    double m1[2],m2[2],m3[2];
    gx[0]= (double)acc.x/MAXACCEL;
    gy[0] = (double)acc.y/MAXACCEL;
    gz[0] = (double)acc.z/MAXACCEL;
    m1[0] = acos(gx[0])*360/(2*M_PI*1.8);
    m2[0] = acos(gy[0])*360/(2*M_PI*1.8);
    m3[0] = acos(gz[0])*360/(2*M_PI*1.8);
    
    
    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
        
        bmi160_get_sensor_data(BMI160_ACCEL_ONLY, &acc, NULL, &bmi160Dev);          //c sa qui fait le reading du struct bmi160Dev
        
        //valeur max de 1 = à 2 fois la force g
        gx[1] = (double)acc.x/MAXACCEL;
        gy[1] = (double)acc.y/MAXACCEL;
        gz[1] = (double)acc.z/MAXACCEL;
        
        //Calculé un nombre entre 0 et 100 (%)
        m1[1] = acos(gx[1])*360/(2*M_PI*1.8);
        m2[1] = acos(gy[1])*360/(2*M_PI*1.8);
        
        //Si la valeur consécutive varie de 2.5% on active l'alarme
        //Une faible valeur puisque le produit est pour des personnes agées
        if (fabs(m1[0]-m1[1])>2.5 || fabs(m2[0]-m2[1])>2.5 || fabs(m3[0]-m3[1])>2.5){
            lastMovement = xTaskGetTickCount();}
        
        m1[0] = m1[1];
        m2[0] = m2[1];
        m3[0] = m3[1];
        
        
        //If it has been more than a 0.5sec since last movement then turn off alarm
        if ( (xTaskGetTickCount() - lastMovement) > 500)
        {
            Cy_GPIO_Write(LED8_PORT,LED8_NUM,1);                        //turn off alarm (LED)
        }
        else
        {
            if (MotionAlarmEnable == true){
            Cy_GPIO_Write(LED8_PORT,LED8_NUM,0);}                       ////turn on alarm (LED) with toggle
        }
        
    }
}


/* [] END OF FILE */
