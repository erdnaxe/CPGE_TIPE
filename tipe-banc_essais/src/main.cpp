/*
    This file is part of the TIPE test bench source code.

    TIPE test bench is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TIPE test bench is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this source code.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Arduino.h"
#include "BrushlessServo.h"
#include "SinArray.h"
#include "centrale_inertielle.h"

// Objets
BrushlessServo moteur;
SinArray sinArray;

// Variables pour la commande
int amplitude = 10; // en degrés
float pulsation = 0.;
float angle_moteur = 0.;       // en degrés
unsigned long temps_debut = 0; // origine temporelle

unsigned long temps_precedent_commande_moteur_boucle = 0;
unsigned long temps_precedent_transmission_serie_boucle = 0;
unsigned long temps_actuel = 0; // en ms

// Commande du moteur
void commande_moteur() {
  // Toutes les 10ms
  if (temps_actuel - temps_precedent_commande_moteur_boucle >= 10) {
    temps_precedent_commande_moteur_boucle = temps_actuel;

    // Commande du moteur
    if (pulsation > 0.1) {
      angle_moteur =
          amplitude * sinArray.getSin(pulsation * temps_actuel / 1000.);
      moteur.write(-angle_moteur - 20);
    }
  }
}

// Gère la réception d'une commande
void reception_serie() {
  if (Serial.available() > 0) {      // Si on a reçu quelque chose
    pulsation = Serial.parseFloat(); // Reception d'une nouvelle pulsation

    // Nouvelle origine des temps
    temps_debut = millis();
    temps_precedent_commande_moteur_boucle = 0;
    temps_precedent_transmission_serie_boucle = 0;
  }
}

// Transmet une mesure sur le port série
void transmission_serie() {
  // Toutes les 10 ms
  if (temps_actuel - temps_precedent_transmission_serie_boucle >= 10) {
    temps_precedent_transmission_serie_boucle = temps_actuel;

    Serial.print(millis() - temps_debut);
    Serial.print(",");
    Serial.print(String(angle_moteur, 4));
    Serial.print(",");
    Serial.print(analogRead(A0));
    Serial.print(",");
    Serial.print(String(centrale_inertielle_angles[0], 4));
    Serial.print(",");
    Serial.print(String(centrale_inertielle_angles[1], 4));
    Serial.print(",");
    Serial.println(String(centrale_inertielle_angles[2], 4));
  }
}

// Initialisation
void setup() {
  // Fast PWM Mode, diviseur de 1 : 16MHz/256
  // Extrait de https://www.arduino.cc/en/Tutorial/SecretsOfArduinoPWM
  TCCR1B = (TCCR1B & 0b11111000) | 0x01;
  TCCR2B = (TCCR2B & 0b11111000) | 0x01;

  moteur.attach(9, 10, 11); // Définition du moteur Brushless
  // moteur.setOutputPower(160);  // Réduction de la tension

  Serial.begin(115200);              // Communication série
  pinMode(A0, INPUT);                // Entrée du potentiomètre
  sinArray.generate();               // Précalcul du table de valeurs de sinus
  initialiser_centrale_inertielle(); // Centrale inertielle
}

// Boucle principale du programme
void loop() {
  // On arrête le programme si la centrale ne fonctionne pas
  if (!centrale_inertielle_initialisee)
    return;

  temps_actuel = millis() - temps_debut;

  boucle_centrale_inertielle(); // Centrale inertielle
  commande_moteur();            // Commande du moteur
  transmission_serie();         // Transmission des mesures
  reception_serie();            // Réception d'une commande
}
