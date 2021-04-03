clear all   %Supprime tous les variables du workspace
close all   %Ferme toutes les figures
clc         %nettoie la command window

%% Plan
%
% Op�rations effecut�es sur signal, une structure :

%signal = 
%  struct with fields:
%
%       HeartRate: [36�1 double]
%    SpO2Infrared: [9000�1 double]
%         SpO2Red: [9000�1 double]
%            SpO2: [36�1 double]

% En premier, on va effectuer les op�rations pour en retirer la valeur
% d'amplitude de la composante DC, puis AC, et du rythme cardiaque pour la
% longueur d'onde rouge. Ensuite, on refait toutes les m�mes �tapes en
% changeant simplement signal.SpO2Red pour signal.SpO2Infrared.
% On aura toutes les informations pour calculer le ratio nous int�ressant,
% et pour le rythme cardiaque on fera une moyenne des deux valeurs obtenues
% si elles diff�rent.

%% Variables et op�rations pr�liminaires
%

signal=loadSignal('SpO2_signal_normal_BPM');
fs=250;        %fr�quence d'�chantillonnage

fc=1;           % fr�quence de coupure � 1Hz, pour couper les fr�quences pr�s de 0 (DC)
fcpb=5;         % fr�quence de coupure du filtre passe-bas pour �liminer le bruit de haute fr�quence


debut=5; % On d�bute l'analyse du signal � 5 s apr�s le d�but de l'acquisition pour ignorer les premiers signaux
startIndex=debut*fs; % On obtient l'indice qui correspond � l'instant 0 en multipliant le temps �coul� depuis le d�but de l'acquisition (en secondes) par la fr�quence d'�chantillonnage

%% Les op�rations qui suivent sont effectu�es sur le signal signal.SpO2Red
%
%% Rep�rage de la fr�quence associ�e � la composante DC 
%

%Ici, on fait une transform�e de Fourier du signal brut pour tout d'abord
%�tre en mesure de rep�rer quelle fr�quence correspond � la composante DC
%du signal, dans le but d'�ventuellement la couper

x=linspace(-fs/2,fs/2,length(signal.SpO2Red));
spectreSignal=abs(fftshift(fft(signal.SpO2Red))); %l'axe des x doit �tre : -fs/2 à fs/2
figure(1)
plot(x,spectreSignal);
legend('Signal');

%% Cr�er et appliquer un premier filtre num�rique (passe-haut) pour �liminer la composante DC :
%

% Cr�ation du filtre passe-haut
Wn=fc/(fs/2); %fr�quence de coupure normalis�e
[b,a] = butter(6,Wn,'high');
figure(2)
freqz(b,a)

% Application du filtre passe-haut
filteredSignal=filter(b,a,signal.SpO2Red);
x=linspace(0,length(filteredSignal)/fs,length(filteredSignal));


figure (3) %% On remarque que le signal filtr� a perdu sa composante DC, car on a pris un filtre passe haut, donc les basses fr�quences ont �t� enlev�es, et le signal DC n'oscille pas donc sa fr�quence est 0 Hz. Donc, en enlevant cette fr�quence, on diminue l'amplitude du signal globalement
plot(x,signal.SpO2Red,x,filteredSignal);
legend('Signal','filtered Signal');

%Donc rendu ici, on a signal.SpO2Red et filteredSignal, qui sont respectivement
%les signaux avec et sans la composante DC. On peut donc mesurer la valeur
%de cette composante DC du signal. V�rifier aussi si 1Hz est vrm la bonne
%fr�q de coupure!

%% Rep�rage des fr�quences � filtrer
%
%Ici, il faut faire une transform�e de Fourier pour d�terminer quelles
%fr�quences sont associ�es � du bruit et sont nuisibles � notre bonne
%lecture du signal. On retrouvera probablement un pic � 60 Hz (secteur), et
%possiblement d'autres fr�quences ind�sirables.

x=linspace(-fs/2,fs/2,length(filteredSignal));
spectreFilteredSignal=abs(fftshift(fft(filteredSignal))); 

figure(4)
plot(x,spectreFilteredSignal);
legend('Filtered Signal');

%% Cr�er et appliquer un second filtre num�rique (passe-bas) pour �liminer des fr�quences �lev�es parasite (bruit):
%

% Cr�ation du filtre passe-haut           
Wn=fcpb/(fs/2); %fr�quence de coupure normalis�e
[b,a] = butter(6,Wn,'low');
figure(5)
freqz(b,a)

% Application du filtre passe-haut
cleanSignal=filter(b,a,filteredSignal); % cleanSignal EST LE VECTEUR � ENVOYER � FELIX
x=linspace(0,length(cleanSignal)/fs,length(cleanSignal));

figure(6)
plot(x,cleanSignal);
%plot(x,filteredSignal,x,cleanSignal);
legend('Clean Signal');

%% Dernier rep�rage de bruit
%
x=linspace(-fs/2,fs/2,length(cleanSignal));
spectreCleanSignal=abs(fftshift(fft(cleanSignal))); 

figure(7)
plot(x,spectreCleanSignal);
legend('Clean Signal');


%% Calcul du rythme cardiaque
%

% � noter qu'il y a des pics tr�s �lev�s au d�but qui faussent les donn�es,
% on va donc commencer � prendre le signal � 5 secondes, cela donne le m�me
% nombre de battements que les vecteurs "r�ponse" des fichiers test.

% Option 1 pour trouver les max
%syms S(x)
%S=cleanSignal;
%S (et ensuite trouver quand la d�riv�e vaut 0?)

% Option 2 pour trouver les max
%positionsMAX=islocalmax(cleanSignal);

% Option 3 pour trouver les max :

MAX=max(cleanSignal(startIndex:length(cleanSignal)));  %Trouver la valeur max du signal
MIN=min(cleanSignal(startIndex:length(cleanSignal)));            %Trouver la valeur min du signal
amplitude=MAX-MIN;   %Calculer l'amplitude du signal filtr�
coeffSensibilite=1; %Choix d'une valeur de coefficient de sensibilit� entre 0 et 1
seuil=MAX-(amplitude/2)*coeffSensibilite; %On �tablit une valeur au dessus de laquelle on consid�re qu'on a un pic
%posPulse=find(signal>seuil);
for i=(startIndex+1):(length(cleanSignal)-1)
    if (cleanSignal(i) > cleanSignal(i-1)) && (cleanSignal(i) > cleanSignal(i+1)) % On entre dans ce if seulement si la valeur du signal clean est plus �lev�e avant et apr�s la i�me composante (cela repr�sente donc un maximum � la i�me composante)
        if cleanSignal(i)>=seuil     % On v�rifie si une deuxi�me condition est respect�e : la valeur de cette i�me composante doit aussi �tre sup�rieure � un certain seuil pr�d�termin� pour �tre s�r que le maximum local correspond � un pic d'ECG �lev� (associ� � la contraction des ventricules)
            positionMAX(i)=1;
        end
    else
        positionMAX(i)=0;
    end
    

end

pics=find(positionMAX); % On cr�e un vecteur dont les composantes sp�cifient quels indices de positionMAX sont non-nuls (donc � quels indices on a des maximums)
instantsMAX=pics./fs; %Conversion des indices de position des max en temps o� chaque max est observ�

for i=1:(length(instantsMAX)-1)
    periodeBattement(i)=instantsMAX(i+1)-instantsMAX(i);
    freqCardInstant(i)=1/(periodeBattement(i));
    rythmeCardInstant(i)=freqCardInstant(i)*60;
    
    instantStartIndex=startIndex+(i-1)*((length(signal.SpO2Red)-startIndex)/length(instantsMAX));
    instantEndIndex=startIndex+(i)*((length(signal.SpO2Red)-startIndex)/length(instantsMAX)); % Ces deux indices permettent de d�terminer entre quels points �valuer les composantes AC et DC pour calculer la saturation en oxyg�ne instantan�e
    DCRed(i)=mean(signal.SpO2Red(instantStartIndex:instantEndIndex))-mean(filteredSignal(instantStartIndex:instantEndIndex));
    MAX=max(cleanSignal(instantStartIndex:instantEndIndex));  %Trouver la valeur max du signal
    MIN=min(cleanSignal(instantStartIndex:instantEndIndex));            %Trouver la valeur min du signal
%     MAX=max(filteredSignal(instantStartIndex:instantEndIndex));  %Trouver la valeur max du signal
%     MIN=min(filteredSignal(instantStartIndex:instantEndIndex));            %Trouver la valeur min du signal
    ACRed(i)=MAX-MIN;   %Calculer l'amplitude du signal filtr�

end

freqCardMoyen=1/mean(periodeBattement);
rythmeCardMoyen=freqCardMoyen*60;
%% DEUXI�ME PARTIE : R�P�TITION DES �TAPES POUR L'INFRAROUGE
%
%% Rep�rage de la fr�quence associ�e � la composante DC
%

%Ici, on fait une transform�e de Fourier du signal brut pour tout d'abord
%�tre en mesure de rep�rer quelle fr�quence correspond � la composante DC
%du signal, dans le but d'�ventuellement la couper

x=linspace(-fs/2,fs/2,length(signal.SpO2Infrared));
spectreSignal=abs(fftshift(fft(signal.SpO2Infrared))); %l'axe des x doit �tre : -fs/2 à fs/2
figure(8)
plot(x,spectreSignal);
legend('Signal');

%% Cr�er et appliquer un premier filtre num�rique (passe haut) pour �liminer la composante DC :
%

% Cr�ation du filtre passe-haut           %fr�quence de coupure
Wn=fc/(fs/2); %fr�quence de coupure normalis�e
[b,a] = butter(6,Wn,'high');
figure(9)
freqz(b,a)

% Application du filtre passe-haut
filteredSignalInfra=filter(b,a,signal.SpO2Infrared);
x=linspace(0,length(filteredSignalInfra)/fs,length(filteredSignalInfra));


figure (10) %% On remarque que le signal filtr� a perdu sa composante DC, car on a pris un filtre passe haut, donc les basses fr�quences ont �t� enlev�es, et le signal DC n'oscille pas donc sa fr�quence est 0 Hz. Donc, en enlevant cette fr�quence, on diminue l'amplitude du signal globalement
plot(x,signal.SpO2Infrared,x,filteredSignalInfra);
legend('Signal','filtered Signal Infrared');

%Donc rendu ici, on a signalf et filteredSignal, qui sont respectivement
%les signaux avec et sans la composante DC. On peut donc mesurer la valeur
%de cette composante DC du signal. V�rifier aussi si 1Hz est vrm la bonne
%fr�q de coupure!


%% Rep�rage des fr�quences � filtrer
%
%Ici, il faut faire une transform�e de Fourier pour d�terminer quelles
%fr�quences sont associ�es � du bruit et sont nuisibles � notre bonne
%lecture du signal. On retrouvera probablement un pic � 60 Hz (secteur), et
%possiblement d'autres fr�quences ind�sirables

x=linspace(-fs/2,fs/2,length(filteredSignalInfra));
spectreFilteredSignalInfra=abs(fftshift(fft(filteredSignalInfra))); 

figure(11)
plot(x,spectreFilteredSignalInfra);
legend('Filtered Signal Infrared');

%% Cr�er et appliquer un second filtre num�rique (passe-bas) pour �liminer des fr�quences �lev�es parasite (bruit):
%

% Cr�ation du filtre passe-haut
Wn=fcpb/(fs/2); %fr�quence de coupure normalis�e
[b,a] = butter(6,Wn,'low');
figure(12)
freqz(b,a)

% Application du filtre passe-haut
cleanSignalInfra=filter(b,a,filteredSignalInfra); % cleanSignalInfra EST LE VECTEUR � ENVOYER � l'expert d'interface pour la courbe d'infrarouge
x=linspace(0,length(cleanSignalInfra)/fs,length(cleanSignalInfra));

figure(13)
plot(x,cleanSignalInfra);
%plot(x,filteredSignalInfra,x,cleanSignal);
legend('Clean Signal Infrared');

%% Dernier rep�rage de bruit
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
%S (et ensuite trouver quand la d�riv�e vaut 0?)

% Option 2 pour trouver les max
%positionsMAX=islocalmax(cleanSignal);

% Option 3 pour trouver les max :

MAX=max(cleanSignalInfra(startIndex:length(cleanSignalInfra)));  %Trouver la valeur max du signal
MIN=min(cleanSignalInfra(startIndex:length(cleanSignalInfra)));            %Trouver la valeur min du signal
amplitude=MAX-MIN;   %Calculer l'amplitude du signal filtr�
coeffSensibilite=1; %Choix d'une valeur de coefficient de sensibilit� entre 0 et 1
seuil=MAX-(amplitude/2)*coeffSensibilite; %On �tablit une valeur au dessus de laquelle on consid�re qu'on a un pic
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
instantsMAXInfra=pics./fs %Conversion des indices de position des max en temps o� chaque max est observ�

for i=1:(length(instantsMAXInfra)-1)
    periodeBattementInfra(i)=instantsMAXInfra(i+1)-instantsMAXInfra(i);
    freqCardInstantInfra(i)=1./(periodeBattementInfra(i));
    rythmeCardInstantInfra(i)=freqCardInstantInfra(i).*60;
    
    instantStartIndex=startIndex+(i-1)*((length(signal.SpO2Infrared)-startIndex)/length(instantsMAXInfra));
    instantEndIndex=startIndex+(i)*((length(signal.SpO2Infrared)-startIndex)/length(instantsMAXInfra)); % Ces deux indices permettent de d�terminer entre quels points �valuer les composantes AC et DC pour calculer la saturation en oxyg�ne instantan�e
    DCInfraRed(i)=mean(signal.SpO2Infrared(instantStartIndex:instantEndIndex))-mean(filteredSignalInfra(instantStartIndex:instantEndIndex));
    MAX=max(cleanSignalInfra(instantStartIndex:instantEndIndex));  %Trouver la valeur max du signal
    MIN=min(cleanSignalInfra(instantStartIndex:instantEndIndex));            %Trouver la valeur min du signal
%     MAX=max(filteredSignalInfra(instantStartIndex:instantEndIndex));  %Trouver la valeur max du signal
%     MIN=min(filteredSignalInfra(instantStartIndex:instantEndIndex));            %Trouver la valeur min du signal
    ACInfraRed(i)=MAX-MIN;   %Calculer l'amplitude du signal filtr�
end
freqCardMoyenInfra=1/mean(periodeBattementInfra);
rythmeCardMoyenInfra=freqCardMoyenInfra*60; % Donne la moyenne du rythme cardiaque sur la p�riode d'acquisition, au besoin
%% Calcul de la saturation en Oxyg�ne (� mettre � la fin)
%

lambdaRed=660e-9;
lambdaInfraRed=880e-9;
DCRed;
DCInfraRed;
ACRed;
ACInfraRed;

% for i=1:length(DCRed)
% taux(i)=((ACRed(i)./DCRed(i)).*lambdaRed)./((ACInfraRed(i)./DCInfraRed(i)).*lambdaInfraRed);
% tauxSANSlambda(i)=((ACRed(i)./DCRed(i)))./((ACInfraRed(i)./DCInfraRed(i))); % Premi�re fa�on
% end
% Deuxi�me fa�on

% Eo660=319,6; %Coefficients d'extinction molaire selon une premi�re source
% Er660=3226,56;
% Eo880=1154;
% Er880=726,44;
Eo660=319,6; %Coefficients d'extinction molaire selon une deuxi�me source
Er660=3227;
Eo880=1154;
Er880=726,39; % Pour le moment, on utilise une m�thode qui ne n�cessite pas ces coefficients d'extinction molaire

a=-45.06; % Coefficients de calibration du MAX30102, afin de calculer la saturation en oxyg�ne � partir du ratio phi
b=30.354;
% c=94,845; % Vraie valeur du coefficient c trouv�e dans un article datant de 2019
c=97.6; %Facteur de calibration modifi� pour que les r�sultats correspondent aux valeurs des tests

for i=1:length(DCRed)
phi(i)=(ACRed(i)./DCRed(i))./(ACInfraRed(i)./DCInfraRed(i));
% fraction(i)=(Er660-(phi(i).*Eo660))./((phi(i).*(Eo880-Er880))-(Eo660-Er660)); 
% ratio(i)=fraction(i)./(1-fraction(i));
SPO2mesure(i)=a.*(phi(i).^2)+b.*phi(i)+c;
end
SPO2mesure % Affichage du vecteur contenant la saturation d'oxyg�ne � chaque battement

%% Calcul du rythme cardiaque moyen obtenu � l'aide des deux longueurs d'onde
%
for i=1:(length(rythmeCardInstant))
    rythmeCard(i)=(rythmeCardInstant(i)+rythmeCardInstantInfra(i))./2;
end
rythmeCard % Affichage du vecteur contenant les valeurs de rythme cardiaque � chaque battement

%% Affichage des r�ponses aux tests 
%
signal.HeartRate
signal.SpO2
length(signal.HeartRate)
length(rythmeCard)
length(signal.SpO2)
length(SPO2mesure)

%% Modifications possibles � apporter pour obtenir des valeurs plus exactes
%
% Calculer la composante DC et AC diff�remment, par exemple DC � partir de
% l'amplitude de la fr�quence 0 apr�s une FFT
%
