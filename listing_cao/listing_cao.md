---
lang: fr-FR
papersize: a4
geometry: margin=1.8cm
numbersections: yes
title: Stabilisateur électronique -- CAO
author: Alexandre IOOSS
toc: yes
---

\newpage

\lstset{
  language=C++,
  morekeywords={difference, translate, cube, rotate, cylinder, use}
}

# Conception du stabilisateur

## Impression 3D du boitier accueillant l'électronique

\begin{center}
\vspace{20pt}
\includegraphics[width=0.7\textwidth]{cao/boitier.png}
\vspace{20pt}
\end{center}

## Impression 3D de l'attache de la tige au servomoteur

\begin{center}
\vspace{20pt}
\includegraphics[width=0.3\textwidth]{cao/attache-tige_servo.png}
\vspace{20pt}
\end{center}

Code **OpenSCAD** :

\lstinputlisting{cao/attache-tige_servo.scad}

\newpage

# Conception du banc d'essais

\begin{center}
\vspace{20pt}
\includegraphics[width=\textwidth]{cao/banc_vide.pdf}
\end{center}

\newpage

## Découpe laser des planches pour faire la structure

Échelle 30%. Planche de 6mm *Medium* découpée au laser dans le laboratoire de
fabrication *SqyLab*. Le tout est monté sur une planche de largeur 202mm et de
hauteur 22mm.

\begin{center}
\vspace{20pt}
\includegraphics[scale=0.3]{cao/banc_back.eps}

\includegraphics[scale=0.3]{cao/banc_left.eps}

\includegraphics[scale=0.3]{cao/banc_right.eps}
\end{center}

\newpage

## Impression 3D de l'attache du potentiomètre au banc d'essais

\begin{center}
\vspace{20pt}
\includegraphics[width=0.5\textwidth]{cao/attache_potentiometre.png}
\vspace{20pt}
\end{center}

Code **OpenSCAD** :

\lstinputlisting{cao/attache_potentiometre.scad}

\newpage

## Impression 3D de l'engrenage entre le moteur et le potentiomètre

\begin{center}
\vspace{20pt}
\includegraphics[width=0.3\textwidth]{cao/pignon_moteur.png}
\hspace{10pt}
\includegraphics[width=0.3\textwidth]{cao/pignon_potentiometre.png}
\vspace{20pt}
\end{center}

Code **OpenSCAD** du pignon du moteur :

\lstinputlisting{cao/pignon_moteur.scad}

Code **OpenSCAD** du pignon du potentiomètre :

\lstinputlisting{cao/pignon_potentiometre.scad}

\newpage

# Modèle pour trouver une approximation de l'inertie de l'appareil photo

Masse : 228 grammes

\begin{center}
\vspace{20pt}
\includegraphics[width=0.8\textwidth]{cao/modele_app_photo.pdf}
\vspace{20pt}
\end{center}



