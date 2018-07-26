---
lang: fr-FR
papersize: a4
geometry: margin=1.8cm
numbersections: yes
title: Stabilisateur électronique -- Listing
author: Alexandre IOOSS
toc: yes
---

\newpage

# Code utilisant le Framework Arduino (pour ATMEGA328p et ESP8266)

\lstset{
  language=C++,
  morekeywords={SimpleTimer, Servo, WebSocketsServer, IPAddress, WiFi, uint32_t, uint8_t, WStype_t, size_t, String}
}

## Bibliothèque pour asservir en position un moteur Brushless

Bibliothèque `BrushlessServo` maintenant publiée dans les contributions de l'éditeur Arduino.

### BrushlessServo.h

\lstinputlisting{brushlessservo/BrushlessServo.h}

### BrushlessServo.cpp

\lstinputlisting{brushlessservo/BrushlessServo.cpp}

### SinArray.h

\lstinputlisting{brushlessservo/SinArray.h}

### SinArray.cpp

\lstinputlisting{brushlessservo/SinArray.cpp}

\newpage

## Bibliothèque pour utiliser l'algorithme constructeur de la centrale inertielle

Programme basé sur l'example du projet *The I2C Device Library*.

### centrale_inertielle.h

\lstinputlisting{../tipe-banc_essais/src/centrale_inertielle.h}

### centrale_inertielle.cpp

\lstinputlisting{../tipe-banc_essais/src/centrale_inertielle.cpp}

\newpage

## Programme pour envoyer des commandes en rampe au servomoteur (ESP8266)

### main.cpp

\lstinputlisting{../tipe-echelon/src/main.cpp}

\newpage

## Programme embarqué sur le stabilisateur finalisé (ESP8266)

### main.cpp

\lstinputlisting{../tipe-stabilisateur/src/main.cpp}

\newpage

## Programme embarqué sur le banc d'essais (ATMega328p)

### main.cpp

\lstinputlisting{../tipe-banc_essais/src/main.cpp}

\newpage

## Bibliothèques externes

### WebSockets (Arduino Framework)

Utilisée sur le stabilisateur pour héberger un serveur au standard WebSocket.

Sous licence GNU LGPL 2.1+.

### SimpleTimer (Arduino Framework)

Utilisée sur le stabilisateur pour obtenir un semblant de multitâche.

Sous licence GNU LGPL 2.1+.

### PID (Brett Beauregard, Arduino Framework)

Utilisée sur le stabilisateur pour l'asservissement.

Développée par *Brett Beauregard* sous licence MIT.

### I2Cdevlib-MPU6050 (The I2C Device Library)

Utilisée pour envoyer l'algorithme constructeur (de MotionApps) sur le Digital
Motion Unit de la centrale inertielle.

Dérivée par le projet *The I2C Device Library* de la librarie de *MotionApps*
sous licence MIT.

\cleardoublepage

# Code Python

\lstset{
  language=Python,
  morekeywords={}
}

## Script pour tester le stabilisateur et configurer les coefficients du correcteur à la volée

### test_stabilisateur.py

\lstinputlisting{../tipe-stabilisateur/python/test_stabilisateur.py}

## Script pour commander le banc d'essais

### ComBanc.py : définit un objet représentant le banc d'essais

\lstinputlisting{../tipe-banc_essais/python/ComBanc.py}

\newpage

### Bode.py : définit un objet représentant un diagramme de Bode

\lstinputlisting{../tipe-banc_essais/python/Bode.py}

\newpage

### BodeTF.py : extension de l'objet Bode pour tracer une fonction de transfert

\lstinputlisting{../tipe-banc_essais/python/BodeTF.py}

\newpage

### test_banc.py : exemple de code pour une commande du banc

\lstinputlisting{../tipe-banc_essais/python/test_banc.py}

\newpage

### bode_centrale_inertielle.py : Création du Bode expérimental de la centrale inertielle

\lstinputlisting{../tipe-banc_essais/python/bode_centrale_inertielle.py}

\newpage

## Script pour le traitement du signal du pendule pesant

### pendule.py

\lstinputlisting{../experience_pendule/pendule.py}

\cleardoublepage

# Code MATLAB pour acquérir les rampes envoyées au servomoteur

\lstset{
  language=MatLab,
  morekeywords={classdef, methods, properties}
}

## Fonctions

### reel_faireAcquisition.m : acquisition d'une rampe sur le servomoteur

\lstinputlisting{../matlab/reel_faireAcquisition.m}

### reel_faireRegression.m : régression affine entre l'entrée et la sortie

\lstinputlisting{../matlab/reel_faireRegression.m}

### reel_rognerRampe.m : rogne le signal de 5s au début et à la fin

\lstinputlisting{../matlab/reel_rognerRampe.m}

### reel_traceCourbe.m : trace l'entrée et la sortie du servomoteur

\lstinputlisting{../matlab/reel_traceCourbe.m}

### reel_traceRegression.m : trace la sortie en fonction de l'entrée du servomoteur (avec superposition de la régression)

\lstinputlisting{../matlab/reel_traceRegression.m}

### reel_SystemeReel.m : définit un objet représentant la communication avec le système réel

\lstinputlisting{../matlab/reel_SystemeReel.m}

\newpage

## Scripts d'exemple

### Effectue un grand nombre d'essais pour affiner la régression

\lstinputlisting{../matlab/reel_SCRIPT5_regressions.m}

### Test de suivi d'une rampe

\lstinputlisting{../matlab/reel_SCRIPT6_suivi.m}

\newpage

# Simulations MATLAB Simulink

## Simulation du servomoteur

\begin{center}
\includegraphics[height=23cm]{assets/modele_rampe.pdf}
\end{center}

## Simulation complète du stabilisateur

\begin{center}
\includegraphics[height=23cm]{assets/modele_stabilisateur.pdf}
\end{center}


