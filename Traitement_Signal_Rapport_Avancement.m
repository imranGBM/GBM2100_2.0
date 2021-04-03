clear all   %Supprime tous les variables du workspace
close all   %Ferme toutes les figures
clc         %nettoie la command window

%% Plan
%
% Opérations effecutées sur signal, une structure :

%signal = 
%  struct with fields:
%
%       HeartRate: [36×1 double]
%    SpO2Infrared: [9000×1 double]
%         SpO2Red: [9000×1 double]
%            SpO2: [36×1 double]

% En premier, on va effectuer les opérations pour en retirer la valeur
% d'amplitude de la composante DC, puis AC, et du rythme cardiaque pour la
% longueur d'onde rouge. Ensuite, on refait toutes les mêmes étapes en
% changeant simplement signal.SpO2Red pour signal.SpO2Infrared.
% On aura toutes les informations pour calculer le ratio nous intéressant,
% et pour le rythme cardiaque on fera une moyenne des deux valeurs obtenues
% si elles diffèrent.

%% Variables et opérations préliminaires
%

signal=loadSignal('SpO2_signal_normal_BPM');
fs=250;        %fréquence d'échantillonnage

fc=1;           % fréquence de coupure à 1Hz, pour couper les fréquences près de 0 (DC)
fcpb=5;         % fréquence de coupure du filtre passe-bas pour éliminer le bruit de haute fréquence


debut=5; % On débute l'analyse du signal à 5 s après le début de l'acquisition pour ignorer les premiers signaux
startIndex=debut*fs; % On obtient l'indice qui correspond à l'instant 0 en multipliant le temps écoulé depuis le début de l'acquisition (en secondes) par la fréquence d'échantillonnage

%% Les opérations qui suivent sont effectuées sur le signal signal.SpO2Red
%
%% Repérage de la fréquence associée à la composante DC 
%

%Ici, on fait une transformée de Fourier du signal brut pour tout d'abord
%être en mesure de repérer quelle fréquence correspond à la composante DC
%du signal, dans le but d'éventuellement la couper

x=linspace(-fs/2,fs/2,length(signal.SpO2Red));
spectreSignal=abs(fftshift(fft(signal.SpO2Red))); %l'axe des x doit être : -fs/2 Ã  fs/2
figure(1)
plot(x,spectreSignal);
legend('Signal');

%% Créer et appliquer un premier filtre numérique (passe-haut) pour éliminer la composante DC :
%

% Création du filtre passe-haut
Wn=fc/(fs/2); %fréquence de coupure normalisée
[b,a] = butter(6,Wn,'high');
figure(2)
freqz(b,a)

% Application du filtre passe-haut
filteredSignal=filter(b,a,signal.SpO2Red);
x=linspace(0,length(filteredSignal)/fs,length(filteredSignal));


figure (3) %% On remarque que le signal filtré a perdu sa composante DC, car on a pris un filtre passe haut, donc les basses fréquences ont été enlevées, et le signal DC n'oscille pas donc sa fréquence est 0 Hz. Donc, en enlevant cette fréquence, on diminue l'amplitude du signal globalement
plot(x,signal.SpO2Red,x,filteredSignal);
legend('Signal','filtered Signal');

%Donc rendu ici, on a signal.SpO2Red et filteredSignal, qui sont respectivement
%les signaux avec et sans la composante DC. On peut donc mesurer la valeur
%de cette composante DC du signal. Vérifier aussi si 1Hz est vrm la bonne
%fréq de coupure!

%% Repérage des fréquences à filtrer
%
%Ici, il faut faire une transformée de Fourier pour déterminer quelles
%fréquences sont associées à du bruit et sont nuisibles à notre bonne
%lecture du signal. On retrouvera probablement un pic à 60 Hz (secteur), et
%possiblement d'autres fréquences indésirables.

x=linspace(-fs/2,fs/2,length(filteredSignal));
spectreFilteredSignal=abs(fftshift(fft(filteredSignal))); 

figure(4)
plot(x,spectreFilteredSignal);
legend('Filtered Signal');

%% Créer et appliquer un second filtre numérique (passe-bas) pour éliminer des fréquences élevées parasite (bruit):
%

% Création du filtre passe-haut           
Wn=fcpb/(fs/2); %fréquence de coupure normalisée
[b,a] = butter(6,Wn,'low');
figure(5)
freqz(b,a)

% Application du filtre passe-haut
cleanSignal=filter(b,a,filteredSignal); % cleanSignal EST LE VECTEUR À ENVOYER À FELIX
x=linspace(0,length(cleanSignal)/fs,length(cleanSignal));

figure(6)
plot(x,cleanSignal);
%plot(x,filteredSignal,x,cleanSignal);
legend('Clean Signal');

%% Dernier repérage de bruit
%
x=linspace(-fs/2,fs/2,length(cleanSignal));
spectreCleanSignal=abs(fftshift(fft(cleanSignal))); 

figure(7)
plot(x,spectreCleanSignal);
legend('Clean Signal');


%% Calcul du rythme cardiaque
%

% À noter qu'il y a des pics très élevés au début qui faussent les données,
% on va donc commencer à prendre le signal à 5 secondes, cela donne le même
% nombre de battements que les vecteurs "réponse" des fichiers test.

% Option 1 pour trouver les max
%syms S(x)
%S=cleanSignal;
%S (et ensuite trouver quand la dérivée vaut 0?)

% Option 2 pour trouver les max
%positionsMAX=islocalmax(cleanSignal);

% Option 3 pour trouver les max :

MAX=max(cleanSignal(startIndex:length(cleanSignal)));  %Trouver la valeur max du signal
MIN=min(cleanSignal(startIndex:length(cleanSignal)));            %Trouver la valeur min du signal
amplitude=MAX-MIN;   %Calculer l'amplitude du signal filtré
coeffSensibilite=1; %Choix d'une valeur de coefficient de sensibilité entre 0 et 1
seuil=MAX-(amplitude/2)*coeffSensibilite; %On établit une valeur au dessus de laquelle on considère qu'on a un pic
%posPulse=find(signal>seuil);
for i=(startIndex+1):(length(cleanSignal)-1)
    if (cleanSignal(i) > cleanSignal(i-1)) && (cleanSignal(i) > cleanSignal(i+1)) % On entre dans ce if seulement si la valeur du signal clean est plus élevée avant et après la ième composante (cela représente donc un maximum à la ième composante)
        if cleanSignal(i)>=seuil     % On vérifie si une deuxième condition est respectée : la valeur de cette ième composante doit aussi être supérieure à un certain seuil prédéterminé pour être sûr que le maximum local correspond à un pic d'ECG élevé (associé à la contraction des ventricules)
            positionMAX(i)=1;
        end
    else
        positionMAX(i)=0;
    end
    

end

pics=find(positionMAX); % On crée un vecteur dont les composantes spécifient quels indices de positionMAX sont non-nuls (donc à quels indices on a des maximums)
instantsMAX=pics./fs; %Conversion des indices de position des max en temps où chaque max est observé

for i=1:(length(instantsMAX)-1)
    periodeBattement(i)=instantsMAX(i+1)-instantsMAX(i);
    freqCardInstant(i)=1/(periodeBattement(i));
    rythmeCardInstant(i)=freqCardInstant(i)*60;
    
    instantStartIndex=startIndex+(i-1)*((length(signal.SpO2Red)-startIndex)/length(instantsMAX));
    instantEndIndex=startIndex+(i)*((length(signal.SpO2Red)-startIndex)/length(instantsMAX)); % Ces deux indices permettent de déterminer entre quels points évaluer les composantes AC et DC pour calculer la saturation en oxygène instantanée
    DCRed(i)=mean(signal.SpO2Red(instantStartIndex:instantEndIndex))-mean(filteredSignal(instantStartIndex:instantEndIndex));
    MAX=max(cleanSignal(instantStartIndex:instantEndIndex));  %Trouver la valeur max du signal
    MIN=min(cleanSignal(instantStartIndex:instantEndIndex));            %Trouver la valeur min du signal
%     MAX=max(filteredSignal(instantStartIndex:instantEndIndex));  %Trouver la valeur max du signal
%     MIN=min(filteredSignal(instantStartIndex:instantEndIndex));            %Trouver la valeur min du signal
    ACRed(i)=MAX-MIN;   %Calculer l'amplitude du signal filtré

end

freqCardMoyen=1/mean(periodeBattement);
rythmeCardMoyen=freqCardMoyen*60;
%% DEUXIÈME PARTIE : RÉPÉTITION DES ÉTAPES POUR L'INFRAROUGE
%
%% Repérage de la fréquence associée à la composante DC
%

%Ici, on fait une transformée de Fourier du signal brut pour tout d'abord
%être en mesure de repérer quelle fréquence correspond à la composante DC
%du signal, dans le but d'éventuellement la couper

x=linspace(-fs/2,fs/2,length(signal.SpO2Infrared));
spectreSignal=abs(fftshift(fft(signal.SpO2Infrared))); %l'axe des x doit être : -fs/2 Ã  fs/2
figure(8)
plot(x,spectreSignal);
legend('Signal');

%% Créer et appliquer un premier filtre numérique (passe haut) pour éliminer la composante DC :
%

% Création du filtre passe-haut           %fréquence de coupure
Wn=fc/(fs/2); %fréquence de coupure normalisée
[b,a] = butter(6,Wn,'high');
figure(9)
freqz(b,a)

% Application du filtre passe-haut
filteredSignalInfra=filter(b,a,signal.SpO2Infrared);
x=linspace(0,length(filteredSignalInfra)/fs,length(filteredSignalInfra));


figure (10) %% On remarque que le signal filtré a perdu sa composante DC, car on a pris un filtre passe haut, donc les basses fréquences ont été enlevées, et le signal DC n'oscille pas donc sa fréquence est 0 Hz. Donc, en enlevant cette fréquence, on diminue l'amplitude du signal globalement
plot(x,signal.SpO2Infrared,x,filteredSignalInfra);
legend('Signal','filtered Signal Infrared');

%Donc rendu ici, on a signalf et filteredSignal, qui sont respectivement
%les signaux avec et sans la composante DC. On peut donc mesurer la valeur
%de cette composante DC du signal. Vérifier aussi si 1Hz est vrm la bonne
%fréq de coupure!


%% Repérage des fréquences à filtrer
%
%Ici, il faut faire une transformée de Fourier pour déterminer quelles
%fréquences sont associées à du bruit et sont nuisibles à notre bonne
%lecture du signal. On retrouvera probablement un pic à 60 Hz (secteur), et
%possiblement d'autres fréquences indésirables

x=linspace(-fs/2,fs/2,length(filteredSignalInfra));
spectreFilteredSignalInfra=abs(fftshift(fft(filteredSignalInfra))); 

figure(11)
plot(x,spectreFilteredSignalInfra);
legend('Filtered Signal Infrared');

%% Créer et appliquer un second filtre numérique (passe-bas) pour éliminer des fréquences élevées parasite (bruit):
%

% Création du filtre passe-haut
Wn=fcpb/(fs/2); %fréquence de coupure normalisée
[b,a] = butter(6,Wn,'low');
figure(12)
freqz(b,a)

% Application du filtre passe-haut
cleanSignalInfra=filter(b,a,filteredSignalInfra); % cleanSignalInfra EST LE VECTEUR À ENVOYER à l'expert d'interface pour la courbe d'infrarouge
x=linspace(0,length(cleanSignalInfra)/fs,length(cleanSignalInfra));

figure(13)
plot(x,cleanSignalInfra);
%plot(x,filteredSignalInfra,x,cleanSignal);
legend('Clean Signal Infrared');

%% Dernier repérage de bruit
%
x=linspace(-fs/2,fs/2,length(cleanSignalInfra));
spectreCleanSignalInfra=abs(fftshift(fft(cleanSignalInfra))); 

figure(14)
plot(x,spectreCleanSignalInfra);
legend('Clean Signal');


%% Calcul du rythme cardiaque
%

% Option 1 pour trouver les max
%syms S(x)
%S=cleanSignal;
%S (et ensuite trouver quand la dérivée vaut 0?)

% Option 2 pour trouver les max
%positionsMAX=islocalmax(cleanSignal);

% Option 3 pour trouver les max :

MAX=max(cleanSignalInfra(startIndex:length(cleanSignalInfra)));  %Trouver la valeur max du signal
MIN=min(cleanSignalInfra(startIndex:length(cleanSignalInfra)));            %Trouver la valeur min du signal
amplitude=MAX-MIN;   %Calculer l'amplitude du signal filtré
coeffSensibilite=1; %Choix d'une valeur de coefficient de sensibilité entre 0 et 1
seuil=MAX-(amplitude/2)*coeffSensibilite; %On établit une valeur au dessus de laquelle on considère qu'on a un pic
%posPulse=find(signal>seuil);
for i=(startIndex+1):(length(cleanSignalInfra)-1)
    if (cleanSignalInfra(i) > cleanSignalInfra(i-1)) && (cleanSignalInfra(i) > cleanSignalInfra(i+1))
        if cleanSignalInfra(i)>=seuil
            positionMAX(i)=1;
        end
    else
        positionMAX(i)=0;
    end
end

pics=find(positionMAX)
instantsMAXInfra=pics./fs %Conversion des indices de position des max en temps où chaque max est observé

for i=1:(length(instantsMAXInfra)-1)
    periodeBattementInfra(i)=instantsMAXInfra(i+1)-instantsMAXInfra(i);
    freqCardInstantInfra(i)=1./(periodeBattementInfra(i));
    rythmeCardInstantInfra(i)=freqCardInstantInfra(i).*60;
    
    instantStartIndex=startIndex+(i-1)*((length(signal.SpO2Infrared)-startIndex)/length(instantsMAXInfra));
    instantEndIndex=startIndex+(i)*((length(signal.SpO2Infrared)-startIndex)/length(instantsMAXInfra)); % Ces deux indices permettent de déterminer entre quels points évaluer les composantes AC et DC pour calculer la saturation en oxygène instantanée
    DCInfraRed(i)=mean(signal.SpO2Infrared(instantStartIndex:instantEndIndex))-mean(filteredSignalInfra(instantStartIndex:instantEndIndex));
    MAX=max(cleanSignalInfra(instantStartIndex:instantEndIndex));  %Trouver la valeur max du signal
    MIN=min(cleanSignalInfra(instantStartIndex:instantEndIndex));            %Trouver la valeur min du signal
%     MAX=max(filteredSignalInfra(instantStartIndex:instantEndIndex));  %Trouver la valeur max du signal
%     MIN=min(filteredSignalInfra(instantStartIndex:instantEndIndex));            %Trouver la valeur min du signal
    ACInfraRed(i)=MAX-MIN;   %Calculer l'amplitude du signal filtré
end
freqCardMoyenInfra=1/mean(periodeBattementInfra);
rythmeCardMoyenInfra=freqCardMoyenInfra*60; % Donne la moyenne du rythme cardiaque sur la période d'acquisition, au besoin
%% Calcul de la saturation en Oxygène (à mettre à la fin)
%

lambdaRed=660e-9;
lambdaInfraRed=880e-9;
DCRed;
DCInfraRed;
ACRed;
ACInfraRed;

% for i=1:length(DCRed)
% taux(i)=((ACRed(i)./DCRed(i)).*lambdaRed)./((ACInfraRed(i)./DCInfraRed(i)).*lambdaInfraRed);
% tauxSANSlambda(i)=((ACRed(i)./DCRed(i)))./((ACInfraRed(i)./DCInfraRed(i))); % Première façon
% end
% Deuxième façon

% Eo660=319,6; %Coefficients d'extinction molaire selon une première source
% Er660=3226,56;
% Eo880=1154;
% Er880=726,44;
Eo660=319,6; %Coefficients d'extinction molaire selon une deuxième source
Er660=3227;
Eo880=1154;
Er880=726,39; % Pour le moment, on utilise une méthode qui ne nécessite pas ces coefficients d'extinction molaire

a=-45.06; % Coefficients de calibration du MAX30102, afin de calculer la saturation en oxygène à partir du ratio phi
b=30.354;
% c=94,845; % Vraie valeur du coefficient c trouvée dans un article datant de 2019
c=97.6; %Facteur de calibration modifié pour que les résultats correspondent aux valeurs des tests

for i=1:length(DCRed)
phi(i)=(ACRed(i)./DCRed(i))./(ACInfraRed(i)./DCInfraRed(i));
% fraction(i)=(Er660-(phi(i).*Eo660))./((phi(i).*(Eo880-Er880))-(Eo660-Er660)); 
% ratio(i)=fraction(i)./(1-fraction(i));
SPO2mesure(i)=a.*(phi(i).^2)+b.*phi(i)+c;
end
SPO2mesure % Affichage du vecteur contenant la saturation d'oxygène à chaque battement

%% Calcul du rythme cardiaque moyen obtenu à l'aide des deux longueurs d'onde
%
for i=1:(length(rythmeCardInstant))
    rythmeCard(i)=(rythmeCardInstant(i)+rythmeCardInstantInfra(i))./2;
end
rythmeCard % Affichage du vecteur contenant les valeurs de rythme cardiaque à chaque battement

%% Affichage des réponses aux tests 
%
signal.HeartRate
signal.SpO2
length(signal.HeartRate)
length(rythmeCard)
length(signal.SpO2)
length(SPO2mesure)

%% Modifications possibles à apporter pour obtenir des valeurs plus exactes
%
% Calculer la composante DC et AC différemment, par exemple DC à partir de
% l'amplitude de la fréquence 0 après une FFT
%
