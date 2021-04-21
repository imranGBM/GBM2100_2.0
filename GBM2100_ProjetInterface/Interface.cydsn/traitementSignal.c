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
#include "traitementSignal.h"

volatile int ScreenNumber;
volatile bool DrawRedGraph;
volatile bool DrawInfraredGraph;

//Vecteurs dans lesquels sont stockés la partie du buffer circulaire à traiter :
volatile float32_t vectorRed[750];
volatile float32_t vectorInfra[750];

// Les vecteurs suivants sont obtenus sur Matlab, fonction de freq dechantillonnage/2, de la freq de coupure et de la longueur du vecteur input
// Les vecteurs générés sont faits pour un filtre passe-haut de fréquence de coupure 1 Hz à une fréquence d'échantillonnage de 100Hz, et un filtre passe-bas pour fs de 100 Hz aussi et une fréquence de coupure à 2 Hz

//float32_t CoeffPH[] = {-0.001018, -0.001106, -0.001296, -0.001594, -0.002008, -0.002538, -0.003185, -0.003945, -0.004811, -0.005775, -0.006823, -0.007942, -0.009114, -0.010320, -0.011539, -0.012750, -0.013931, -0.015061, -0.016116, -0.017078, -0.017926, -0.018645, -0.019220, -0.019640, -0.019894, 0.979022, -0.019894, -0.019640, -0.019220, -0.018645, -0.017926, -0.017078, -0.016116, -0.015061, -0.013931, -0.012750, -0.011539, -0.010320, -0.009114, -0.007942, -0.006823, -0.005775, -0.004811, -0.003945, -0.003185, -0.002538, -0.002008, -0.001594, -0.001296, -0.001106, -0.001018};
//float32_t CoeffPB[] = {-0.001060, -0.001116, -0.001227, -0.001361, -0.001464, -0.001458, -0.001251, -0.000738, 0.000187, 0.001623, 0.003653, 0.006332, 0.009685, 0.013695, 0.018301, 0.023402, 0.028853, 0.034477, 0.040071, 0.045414, 0.050288, 0.054481, 0.057806, 0.060112, 0.061293, 0.061293, 0.060112, 0.057806, 0.054481, 0.050288, 0.045414, 0.040071, 0.034477, 0.028853, 0.023402, 0.018301, 0.013695, 0.009685, 0.006332, 0.003653, 0.001623, 0.000187, -0.000738, -0.001251, -0.001458, -0.001464, -0.001361, -0.001227, -0.001116, -0.001060};
float32_t CoeffPH[]={-0.001018, -0.001106, -0.001296, -0.001594, -0.002008, -0.002538, -0.003185, -0.003945, -0.004811, -0.005775, -0.006823, -0.007942, -0.009114, -0.010320, -0.011539, -0.012750, -0.013931, -0.015061, -0.016116, -0.017078, -0.017926, -0.018645, -0.019220, -0.019640, -0.019894, 0.979022, -0.019894, -0.019640, -0.019220, -0.018645, -0.017926, -0.017078, -0.016116, -0.015061, -0.013931, -0.012750, -0.011539, -0.010320, -0.009114, -0.007942, -0.006823, -0.005775, -0.004811, -0.003945, -0.003185, -0.002538, -0.002008, -0.001594, -0.001296, -0.001106, -0.001018};
float32_t CoeffPB[]={0.000078, 0.000255, 0.000498, 0.000858, 0.001387, 0.002135, 0.003148, 0.004462, 0.006105, 0.008091, 0.010418, 0.013072, 0.016019, 0.019212, 0.022589, 0.026075, 0.029585, 0.033028, 0.036309, 0.039334, 0.042012, 0.044264, 0.046017, 0.047219, 0.047829, 0.047829, 0.047219, 0.046017, 0.044264, 0.042012, 0.039334, 0.036309, 0.033028, 0.029585, 0.026075, 0.022589, 0.019212, 0.016019, 0.013072, 0.010418, 0.008091, 0.006105, 0.004462, 0.003148, 0.002135, 0.001387, 0.000858, 0.000498, 0.000255, 0.000078};

// Déclaration des variables globales

float aCoeff=-45.06; // Coefficients de calibration du capteur, permettant d'obtenir la valeur de saturation en oxygène
float bCoeff=30.354;
float cCoeff=94.845;
float fs=100; // Fréquence d'échantillonnage
uint16_t fc=1; // Fréquence de coupure du filtre passe-haut en Hz
uint16_t fcpb=2; // Fréquence de coupure du filtre passe-bas en Hz
float32_t dureeTraitement=7.5; // Durée, en secondes, entre les affichages (donc durée du signal qui est traité à chaque fois qu'on répète le programme

uint32_t blockSize=BLOCK_SIZE; 

float32_t meanRythm;
float32_t meanSPO2;
float32_t mean;                 // Variable à laquelle la valeur moyenne du courant dans un certain intervalle sera attribuée dans la boucle qui suit
float32_t DCr;
float32_t DCI;

volatile int Saturation; 
volatile int RythmeCardiaque;

arm_fir_instance_f32 S;




int16_t numTaps=NUM_TAPS;
float32_t vector_Red_Clean[BLOCK_SIZE];          // Ce vecteur et le prochain contiendront les valeurs du signal complètement filtré
float32_t vector_IR_Clean[BLOCK_SIZE];
float32_t vector_Red_Half_Clean[BLOCK_SIZE];          //Ce vecteur et le prochain contiendront les valeurs du signal ayant passé dans un seul des deux filtres
float32_t vector_IR_Half_Clean[BLOCK_SIZE];
float32_t stateBuffer[NUM_TAPS+BLOCK_SIZE-1]; // taille telle que spécifiée dans la page internet de la librairie CMSIS DSP


void traitement_signal()
{     
    SystemInit();
    SystemCoreClockUpdate(); // Cette fonction et la précédente servent à initialiser les bibliothèques CMSIS-DSP
    
    // Filtres passe-haut et calcul de la composante DC en trouvant la moyenne du signal
    
    arm_mean_f32((float32_t*)&vectorRed,blockSize,&mean); // Fonction qui ressort dans la variable "mean" la valeur moyenne du vecteur vectorBattementRed, ce qui donne la composante DCr pour le battement actuel
    DCr=mean;
    arm_mean_f32((float32_t*)vectorInfra,blockSize,&mean); // Fonction qui ressort dans la variable "mean" la valeur moyenne du vecteur vectorBattementInfra, ce qui donne la composante DCI pour le battement actuel
    DCI=mean;
    
    for (uint32_t i = 0; i < blockSize; i++){
        vectorRed[i]=vectorRed[i]-DCr;
        vectorInfra[i]=vectorInfra[i]-DCI;
    }
    
    float32_t* ptrRedClean;            //    pointeurs qui pointent vers le output fu filtrage
    float32_t* ptrInfraClean;
  
    ptrRedClean=&vector_Red_Clean[0];
    ptrInfraClean=&vector_IR_Clean[0];
    
    arm_fir_init_f32(&S,numTaps,&CoeffPB[0],&stateBuffer[0],blockSize);
    arm_fir_f32(&S,&vectorRed[0],ptrRedClean,blockSize);
    arm_fir_f32(&S,&vectorInfra[0],ptrInfraClean,blockSize);
    
    char resultfiltre1[16];                               
                    sprintf(resultfiltre1,"%5.10f \n\r", vectorRed[100]);                 // conversion du resultat entier en chaine de caracteres
                    UART_1_PutString(resultfiltre1);
                    UART_1_PutString("\r\n");
    
    char resultfiltre2[16];                               
                    sprintf(resultfiltre2,"%5.10f \n\r", vector_Red_Clean[100]);                 // conversion du resultat entier en chaine de caracteres
                    UART_1_PutString(resultfiltre2);
                    UART_1_PutString("\r\n");
    
    
    float32_t maxTraitementRed=0; // Maximum absolu dans le vecteur que l'on est en train de traiter
    uint32_t positionMaxRed=0;
    arm_max_f32(ptrRedClean,blockSize,&maxTraitementRed,&positionMaxRed); //après cette ligne, la valeur de maxx sera la valeur du max du vecteur, et la valeur de position sera l'indice du max
    float32_t minTraitementRed=0;
    uint32_t positionMinRed=0;
    arm_min_f32(ptrRedClean,blockSize,&minTraitementRed,&positionMinRed);
    
    float32_t maxTraitementInfra=0; // Maximum absolu dans le vecteur que l'on est en train de traiter
    uint32_t positionMaxInfra=0;
    arm_max_f32(ptrInfraClean,blockSize,&maxTraitementInfra,&positionMaxInfra); //après cette ligne, la valeur de maxx sera la valeur du max du vecteur, et la valeur de position sera l'indice du max
    float32_t minTraitementInfra=0;
    uint32_t positionMinInfra=0;
    arm_min_f32(ptrInfraClean,blockSize,&minTraitementInfra,&positionMinInfra);
    
    // On cherche ensuite les valeurs et positions des extremums relatifs
    
    float32_t coeffSensibilite=1; // coefficient qui détermine à quel point une valeur doit être proche du maximum ou du minimum absolu pour être considérée comme un max ou min relatif. Ce coefficient multiplie la moitié de l'amplitude divisée par 2, et compare cette valeur à la valeur testée pour voir si cette dernière peut être considérée max ou min relatif
    float32_t amplitudeMaxRed=maxTraitementRed-minTraitementRed; // Borne supérieure sur l'amplitude crête-à-crête du signal pour le rouge
    float32_t amplitudeMaxInfra=maxTraitementInfra-minTraitementInfra; // Borne supérieure sur l'amplitude crête-à-crête du signal pour l'infrarouge
  
    float32_t seuilRed=maxTraitementRed-(amplitudeMaxRed/2)*coeffSensibilite; // Seuil au-dessus ou en-dessous duquel il faut qu'une valeur se trouve pour être considérée comme un maximum ou minimum relatif. Avec le coefficient de sensibilité qui vaut 1, le seuil se trouve environ à la valeur moyenne si l'on considère le signal comme une fonction sinusoïdale
    float32_t seuilInfra=maxTraitementInfra-(amplitudeMaxInfra/2)*coeffSensibilite;
    
    uint32_t borneNombreBattements=ceil(fcpb*dureeTraitement); // Ce nombre représente la limite théorique de battements qu'il pourrait y avoir dans la partie en cours de traitement. fcpb est la fréquence la plus haute que l'on pourrait retrouver dans le signal
    uint32_t j=0; // Compteur du nombre de battements
    
    int redPositionMaxRel[borneNombreBattements]; // Vecteur qui contiendra les indices des points où il y a des maximum relatifs dans le vecteur en traitement (pour le rouge).
    float redMaxRel[borneNombreBattements]; // Valeur de des maximums relatifs pour le rouge
    int redPositionMinRel[borneNombreBattements]; // Vecteur qui contiendra les indices des points où il y a des minimums relatifs dans le vecteur en traitement (pour le rouge).
    float redMinRel[borneNombreBattements]; //Valeur de des minimums relatifs pour le rouge
    
    float infraMaxRel[borneNombreBattements]; // Valeur de des maximums relatifs pour l'infrarouge
    float infraMinRel[borneNombreBattements]; // Valeur de des minimums relatifs pour l'infrarouge
    
    for (uint32_t i = 1; i < blockSize-1; i++){
        if (vector_Red_Clean[i]>vector_Red_Clean[i-1] && vector_Red_Clean[i]>vector_Red_Clean[i+1]){
            if (vector_Red_Clean[i]>vector_Red_Clean[i-2] && vector_Red_Clean[i]>vector_Red_Clean[i+2]){
                if (vector_Red_Clean[i]>vector_Red_Clean[i-3] && vector_Red_Clean[i]>vector_Red_Clean[i+3]){
                    if (vector_Red_Clean[i]>vector_Red_Clean[i-4] && vector_Red_Clean[i]>vector_Red_Clean[i+4]){
                        if (vector_Red_Clean[i]>vector_Red_Clean[i-10] && vector_Red_Clean[i]>vector_Red_Clean[i+10]){                      // Les conditions à respectées permettent de s'assurer que les points juste avant et juste après le point actuels sont moins élevés que le point actuel, et qu'on a bien un maximum. De plus, pour s'assurer que le maximum correspond bien à un pic beaucoup plus élevé que ce qui est autour, on vérifie que même à + ou - 10 échantillons, il n'y a pas de point plus élevé que l'échantillon actuel
                            if (vector_Red_Clean[i]>seuilRed){
                            redPositionMaxRel[j]=i;
                            redMaxRel[j]=vector_Red_Clean[i];
                            j++;                                                                                            // On ajoute 1 à la valeur de j à chaque fois qu'on trouve un maximum dans le rouge
                            }
                        }
                    }
                    
                }
                
            }
            
        }
        else if (vector_Red_Clean[i]<vector_Red_Clean[i-1] && vector_Red_Clean[i]<vector_Red_Clean[i+1]){
            if (vector_Red_Clean[i]<vector_Red_Clean[i-2] && vector_Red_Clean[i]<vector_Red_Clean[i+2]){
                if (vector_Red_Clean[i]<vector_Red_Clean[i-3] && vector_Red_Clean[i]<vector_Red_Clean[i+3]){
                    if (vector_Red_Clean[i]<vector_Red_Clean[i-4] && vector_Red_Clean[i]<vector_Red_Clean[i+4]){
                        if (vector_Red_Clean[i]<vector_Red_Clean[i-10] && vector_Red_Clean[i]<vector_Red_Clean[i+10]){
                            if (vector_Red_Clean[i]<seuilRed){
                            redPositionMinRel[j]=i;
                            redMinRel[j]=vector_Red_Clean[i];
                            }
                        }
                    }
                    
                }
                
            }
            
        } 
        if (vector_IR_Clean[i]>vector_IR_Clean[i-1] && vector_IR_Clean[i]>vector_IR_Clean[i+1]){
            if (vector_IR_Clean[i]>vector_IR_Clean[i-2] && vector_IR_Clean[i]>vector_IR_Clean[i+2]){
                if (vector_IR_Clean[i]>vector_IR_Clean[i-3] && vector_IR_Clean[i]>vector_IR_Clean[i+3]){
                    if (vector_IR_Clean[i]>vector_IR_Clean[i-4] && vector_IR_Clean[i]>vector_IR_Clean[i+4]){
                        if (vector_IR_Clean[i]>vector_IR_Clean[i-10] && vector_IR_Clean[i]>vector_IR_Clean[i+10]){
                            if (vector_IR_Clean[i]>seuilInfra){
                                infraMaxRel[j]=vector_IR_Clean[i];
                            }
                        }                    
                    }
                }
            }   
        }
        else if (vector_IR_Clean[i]<vector_IR_Clean[i-1] && vector_IR_Clean[i]<vector_IR_Clean[i+1]){
            if (vector_IR_Clean[i]<vector_IR_Clean[i-2] && vector_IR_Clean[i]<vector_IR_Clean[i+2]){
                if (vector_IR_Clean[i]<vector_IR_Clean[i-3] && vector_IR_Clean[i]<vector_IR_Clean[i+3]){
                    if (vector_IR_Clean[i]<vector_IR_Clean[i-4] && vector_IR_Clean[i]<vector_IR_Clean[i+4]){
                        if (vector_IR_Clean[i]<vector_IR_Clean[i-10] && vector_IR_Clean[i]<vector_IR_Clean[i+10]){
                            if (vector_IR_Clean[i]<seuilInfra){
                                infraMinRel[j]=vector_IR_Clean[i];
                            }
                        }
                    }
                }
            }
            
        }   
   }
    
    char result[16];                               
                    sprintf(result,"%d\n\r", j);                 // conversion du resultat entier en chaine de caracteres
                    UART_1_PutString(result);
                    UART_1_PutString("\r\n");
                    
    
    uint32_t nbBattements = j; // Nombre de maximums (battements détectés) dans le signal filtré
    // On utilisera ensuite les indices des maximums relatifs pour calculer la période de chaque battement, il faut établir une correspondance entre leur indice et l'instant (en secondes depuis le début du traitement) où ces maximums sont atteints     
    float32_t rythmeCard[nbBattements-1]; // Vecteur contenant les l'inverse intervalles de temps entre chaque maximum consécutif (Fréquence cardiaque), multipliés par 60 secondes (rythme cardiaque). Sa taille est le nombre de battements -1, car il y a toujours n-1 intervalles entre n points       
    float32_t instantMaxRel[nbBattements]; // Vecteur qui contiendra les instants (en secondes) où les maximums sont atteints

    
    
    for (uint32_t i = 0; i < nbBattements ; i++){ 
        instantMaxRel[i]=redPositionMaxRel[i]/fs; // Transformation des valeurs de position (indices) des maximums en valeur de temps, c-a-d les instants où il y a des maximums (en secondes)
    }

    for (uint32_t i = 0; i < nbBattements-1; i++){
       rythmeCard[i]=60/(instantMaxRel[i+1]-instantMaxRel[i]); //Caclul du rythme cardiaque (60* la fréquence des battements, donc 60 *(inverse de la période)
    }
    
    
    float32_t ACr[nbBattements-1]; // Vecteur contenant les valeurs de la composante AC pour le rouge à chaque intervalle entre les maximums
    float32_t ACI[nbBattements-1]; // Idem pour infrarouge

    for (uint32_t i = 0; i < nbBattements-1; i++){
       ACr[i]=redMaxRel[i]-redMinRel[i]; 
       ACr[i]=infraMaxRel[i]-infraMinRel[i];
    }
   
    // Calcul de SPO2
    float32_t R[nbBattements-1];
    float32_t ratioOxygen[nbBattements-1];
    float32_t SPO2[nbBattements-1]; 

    for (uint32_t i = 0; i < nbBattements-1; i++){
    R[i]=(DCr/ACr[i])/(DCI/ACI[i]);
    ratioOxygen[i]=aCoeff*(R[i])*(R[i])+bCoeff*R[i]+cCoeff;
    SPO2[i]=100/((1/ratioOxygen[i])+1); // Permet d'obtenir le pourcentage de saturation en oxygène de l'hémoglobine, pour chaque battement
    }
    
    arm_mean_f32(&rythmeCard[0],nbBattements-1,&meanRythm); // permet d'obtenir, dans la variable meanRythm, la moyenne du rythme cardiaque sur la période analysée
    arm_mean_f32(&SPO2[0],nbBattements-1,&meanSPO2);    // Permet d'obtenir, dans la variable meanSPO2, la moyenne de la saturation en oxygène sur la période analysée
    
    Saturation=round(meanSPO2);
    RythmeCardiaque=round(meanRythm);
    
    char resultRythme[16];                               
                    sprintf(resultRythme,"%d\n\r", RythmeCardiaque);                 // conversion du resultat entier en chaine de caracteres
                    UART_1_PutString(resultRythme);
                    UART_1_PutString("\r\n");
                    
    char resultSaturation[16];                               
                    sprintf(resultSaturation,"%d\n\r", Saturation);                 // conversion du resultat entier en chaine de caracteres
                    UART_1_PutString(resultSaturation);
                    UART_1_PutString("\r\n");  
    
    if(ScreenNumber==0){
        if(DrawRedGraph==true){
            drawGraph(&vector_Red_Clean[0]);
            updateParameters( Saturation, RythmeCardiaque, 3); 
        }
        else {
            drawGraph(&vector_IR_Clean[0]);
            updateParameters( Saturation, RythmeCardiaque, 4);
        }
    }                 

}


/* [] END OF FILE */
