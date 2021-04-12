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
#include "Max30102_task.h"

//buffer circulaire variable init pour le ISR function
    //int size_traitement = 300; //for later
    uint32_t bufferRED[100];
    uint32_t bufferIR[100];
    uint8_t indexWrite = 0;

// uint8_t car sa return 1 byte de data
uint8_t read_Register(uint8_t address)      //address du register qui va read
{
    I2C_MAX_MasterSendStart( I2C_ADDRESS,CY_SCB_I2C_WRITE_XFER,100);
    I2C_MAX_MasterWriteByte(address,100);  
    I2C_MAX_MasterSendReStart(I2C_ADDRESS,CY_SCB_I2C_READ_XFER,100);
    uint8_t regValue=0;
    I2C_MAX_MasterReadByte(CY_SCB_I2C_MASTER_DATA_NAK,(uint8_t*)&regValue,100);
    I2C_MAX_MasterSendStop(100);  
    return regValue;
}

void write_Register(uint8_t address, uint8_t data)      //adress du register et data qu'on veut write ds le register
{ 
    I2C_MAX_MasterSendStart(I2C_ADDRESS,CY_SCB_I2C_WRITE_XFER,100);
    I2C_MAX_MasterWriteByte(address,100);        
    I2C_MAX_MasterWriteByte(data,100);         
    I2C_MAX_MasterSendStop(100);   
    
}

void readMultipleBytes(uint8_t baseAddress, uint8_t *buffer, uint8_t length)    //baseAddress du register  length = 6 pr RED + IR pr buffer store le data
{
        I2C_MAX_MasterSendStart( I2C_ADDRESS,CY_SCB_I2C_WRITE_XFER,100); 
        I2C_MAX_MasterWriteByte(baseAddress,100);       
        I2C_MAX_MasterSendReStart( I2C_ADDRESS,CY_SCB_I2C_READ_XFER,100);
        uint8_t idx = 0;
        for(idx=0 ; idx < length-1 ; idx++){ 
            I2C_MAX_MasterReadByte(CY_SCB_I2C_ACK,(uint8_t*)&buffer[idx],100);  
        }
        I2C_MAX_MasterReadByte(CY_SCB_I2C_NAK,(uint8_t*)&buffer[idx],100);
        I2C_MAX_MasterSendStop(100); 
}

void max30102_init()
{
    //I2C init
    I2C_MAX_Start();   
    write_Register(REG_INTR_ENABLE_1, 0b01000000);  //reset le config au default
    CyDelay(1000);
    
    Cy_GPIO_Write(LED8_PORT,LED8_NUM,0);    //turns on red LED pr voir si code run jusqu'ici 
    
    //Config init
    write_Register(REG_INTR_ENABLE_1, 0b11000000);
    write_Register(REG_INTR_ENABLE_2, 0b00000000);
    write_Register(REG_FIFO_WR_PTR, 0b00000000);    //on utilise pas les pointers
    write_Register(REG_OVF_COUNTER, 0b00000000);
    write_Register(REG_FIFO_RD_PTR, 0b00000000);
    //write_Register(REG_FIFO_DATA, 0xc0);      //acquisition des données ds FIFO_DATA, so rien a write dedans
    write_Register(REG_FIFO_CONFIG, 0b00001111); //sample average = 000 (no averaging) / FIFO_ROLLOVER_EN = 0 (false) / fifo almost full a 17 data unread
    write_Register(REG_MODE_CONFIG, 0b00000011); //MODE[2:0] = 011 pr SpO2, donc RED et IR     
    write_Register(REG_SPO2_CONFIG, 0b00100111); //ADC range = 4096nA (01), sample rate = 100Hz(001), LED PulseWidth = 411us (11), car 18bit ADC resolution
    write_Register(REG_LED1_PA, 0b00100100);  //8bits varie de 0 à 255, 0=0ma et 255=51ma
    write_Register(REG_LED2_PA, 0b00100100);  //donc ici on a 7.2mA (54 en binaire)
    write_Register(REG_PILOT_PA, 0x7f);     //
    //autre registers que je use pas
    /*write_Register(REG_MULTI_LED_CTRL1, 0xc0);
    write_Register(REG_MULTI_LED_CTRL2, 0xc0);
    write_Register(REG_TEMP_INTR, 0xc0);
    write_Register(REG_TEMP_FRAC, 0xc0);
    write_Register(REG_TEMP_CONFIG, 0xc0);
    write_Register(REG_PROX_INT_THRESH, 0xc0);
    write_Register(REG_REV_ID, 0xc0);
    write_Register(REG_PART_ID, 0xc0);
    */
    
    //ISR init
    Cy_SysInt_Init(&Max_int_cfg, read_data_ISR_fct); // first parametre = le nom du module ISR dans top design
    NVIC_ClearPendingIRQ(Max_int_cfg.intrSrc);
    NVIC_EnableIRQ(Max_int_cfg.intrSrc);
    
    //
    read_Register(0x00);       //lecture d'initialisation pour enlever le interrupt
   
}

void readFIFOData(uint32_t *bufferRED,uint32_t* bufferIR){          //acquisition de 24bit de data pr RED et 24bit pr IR

    uint8_t buffer[6];
    
    readMultipleBytes(REG_FIFO_DATA,buffer,6);
    
    *bufferRED = ((buffer[0]&0b00000011)<<16) + (buffer[1]<<8) + buffer[2];
    *bufferIR = ((buffer[3]&0b00000011)<<16) + (buffer[4]<<8) + buffer[5];
    
}

#include <inttypes.h>       //StackOverflow pr "zu" qui me permet de printf un uint32_t

void read_data_ISR_fct()
{
    
    readFIFOData(&bufferRED[indexWrite],&bufferIR[indexWrite]);
    //UART pr vérifier mes variables
    //printf("RED=%24lu IR=%24lu \r\n", bufferRED[indexWrite], bufferIR[indexWrite]);     //problem, car c un uint32_t et mon shit work pr int
    
    char Output[50];
    sprintf(Output,"%lu : %lu \r\n", bufferRED[indexWrite], bufferIR[indexWrite]);
    //sprintf(Output,"%lu : %lu \r\n", *bufferRED, *bufferIR);      //Geb 
    UART_1_PutString("RED and IR Values: \r\n");
    UART_1_PutString(Output);
    
    //END UART
    
    //gestion de l'index pour le vuffer circulaire
            //index++ jusqua index = 99, a 99 met index a 0
    if(indexWrite%100)

    
    indexWrite++;
    if (indexWrite == 99){
        indexWrite = 0;
    }
    Cy_GPIO_ClearInterrupt(Pin_INT_0_PORT,Pin_INT_0_NUM);
    NVIC_ClearPendingIRQ(Max_int_cfg.intrSrc);
    
}

void max30102_task(void *arg)
{
    (void)arg;
    max30102_init();
    
    for(;;){
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}


