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
fs=100;        %fréquence d'échantillonnage

fc=1;           % fréquence de coupure à 1Hz, pour couper les fréquences forprès de 0 (DC)
fcpb=2;         % fréquence de coupure du filtre passe-bas pour éliminer le bruit de haute fréquence


debut=5; % On débute l'analyse du signal à 5 s après le début de l'acquisition pour ignorer les premiers signaux
startIndex=debut*fs; % On obtient l'indice qui correspond à l'instant 0 en multipliant le temps écoulé depuis le début de l'acquisition (en secondes) par la fréquence d'échantillonnage



% Paramètres à mettre en argument de la fonction fir1 :
aPH=49; % Longueur du vecteur à filtrer -1
bPH=fc/(fs/2); %

aPB=49; % Longueur du vecteur à filtrer -1
bPB=fcpb/(fs/2);



vecteurCoefficientsPH=fliplr(fir1(aPH,bPH,'high'))

vecteurCoefficientsPB=fliplr(fir1(aPB,bPB));

% 
% for i=1:length(vecteurCoefficientsPH)
% fprintf('%d', '%s', vecteurCoefficientsPH(i),', ')
% end

fprintf('vecteurCoefficientsPH is: [');
fprintf('%.6f, ', vecteurCoefficientsPH(1:end-1));
fprintf('%.6f]\n', vecteurCoefficientsPH(end));

% On répète pour afficher les valeurs du passe-bas

fprintf('vecteurCoefficientsPB is: [');
fprintf('%.6f, ', vecteurCoefficientsPB(1:end-1));
fprintf('%.6f]\n', vecteurCoefficientsPB(end));

length(vecteurCoefficientsPB)




%a=0.00000000001234;
%fprintf('%.7f\n',a)
%fprintf('%.7f\n',vecteurCoefficientsPH(1))
