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

/* [] END OF FILE */
//arm_fir_instance_f32* S;
//arm_fir_instance_f32* SIR;
//int16_t numTaps = 50;
//
//float32_t* stateBuffer[numTaps+blockSize-1]; // taille telle que spécifiée dans la page internet de la librairie CMSIS DSP
//    float32_t* stateBufferIR[numTaps+blockSize-1];
//    
//    float32_t* vectorRedFloat[blockSize];
//    float32_t* vectorInfraFloat[blockSize];
//    float32_t* vectorRedClean[blockSize];
//    float32_t* vectorInfraClean[blockSize];
//    
//    
//    for (uint32_t i = 0; i < blockSize; i++){
//        vectorRedFloat[i]=(float32_t*)&vectorRed[i]; 
//        vectorInfraFloat[i]=(float32_t*)&vectorInfra[i]; 
//    }
//    
//    
//    arm_fir_init_f32(&S,numTaps,&CoeffPB[0],&stateBuffer[0],blockSize);
//    arm_fir_f32(&S,vectorRedFloat,vectorRedClean,blockSize);
//    
//    //arm_fir_init_f32(&S,numTaps,&CoeffPB[0],&stateBufferIR[0],blockSize);
//    arm_fir_f32(&S,vectorInfraFloat,vectorInfraClean,blockSize);
//    
////    for (uint32_t i = 0; i < blockSize; i++){
////        vectorRed[i]=(int32_t)(vectorRedClean[i]); 
////        vectorInfra[i]=(int32_t)(vectorInfraClean[i]);
////    }