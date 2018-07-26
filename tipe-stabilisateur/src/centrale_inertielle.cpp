/*
    This file is part of the TIPE stabilisator source code.

    TIPE stabilisator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TIPE stabilisator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this source code.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "centrale_inertielle.h"
#include "Arduino.h"
#include "MPU6050_6Axis_MotionApps20.h"

// Variables définies dans l'en-tête
bool centrale_inertielle_initialisee = false;
float centrale_inertielle_angles[3] = {0., 0., 0.};

// Variables globales
uint16_t taille_mesure;             // Taille d'une mesure
uint16_t taille_fifo;               // Taille de la file d'attente FIFO
volatile bool interruption = false; // Interruption envoyée par la centrale
MPU6050 centrale_inertielle;

// Fonction d'interruption
void interruptionVraie() { interruption = true; }

// Initialise la centrale inertielle avec l'algorithme de MotionApps
void initialiser_centrale_inertielle() {
#if defined(__AVR_ATmega328P__)
  Wire.begin(); // I2C ATMEGA
#else
  Wire.begin(PIN_SDA, PIN_SCL); // I2C ESP8266
#endif
  Wire.setClock(400000);

  pinMode(PIN_INTERRUPTION, INPUT); // Entrée de l'interruption

  centrale_inertielle.initialize(); // Initialisation de la centrale
  uint8_t etat = centrale_inertielle.dmpInitialize(); // Initialisation DMP

  // Calibration
  centrale_inertielle.setXGyroOffset(X_GYRO_OFFSET);
  centrale_inertielle.setYGyroOffset(Y_GYRO_OFFSET);
  centrale_inertielle.setZGyroOffset(Z_GYRO_OFFSET);
  centrale_inertielle.setXAccelOffset(X_ACCEL_OFFSET);
  centrale_inertielle.setYAccelOffset(Y_ACCEL_OFFSET);
  centrale_inertielle.setZAccelOffset(Z_ACCEL_OFFSET);

  if (etat == 0) {
    // La centrale marche
    centrale_inertielle.setDMPEnabled(true); // Active le module de calcul
    attachInterrupt(digitalPinToInterrupt(PIN_INTERRUPTION), interruptionVraie,
                    RISING); // Interruption envoyée par la centrale

    // Récupération de la taille d'une mesure
    taille_mesure = centrale_inertielle.dmpGetFIFOPacketSize();

    centrale_inertielle_initialisee = true;
  }
}

// Récupère des nouvelles mesures si elles sont disponibles
void boucle_centrale_inertielle() {
  if ((!interruption && taille_fifo < taille_mesure) ||
      !centrale_inertielle_initialisee) {
    return; // Pas encore prête
  }

  interruption = false;

  // Etat actuel de la centrale
  uint8_t etat_interruption = centrale_inertielle.getIntStatus();

  // Récupère la taille de la liste d'attente
  taille_fifo = centrale_inertielle.getFIFOCount();

  if ((etat_interruption & 0x10) || taille_fifo == 1024) {
    // Dépassement de capacité, on réinitialise
    centrale_inertielle.resetFIFO();
  } else if (etat_interruption & 0x02) {
    // Attente d'avoir toutes les données (très rapide)
    while (taille_fifo < taille_mesure) {
      taille_fifo = centrale_inertielle.getFIFOCount();
    }

    // Lecture de la liste FIFO
    uint8_t fifoBuffer[64];
    centrale_inertielle.getFIFOBytes(fifoBuffer, taille_mesure);

    // Nouvelle taille de la liste d'attente
    taille_fifo -= taille_mesure;

    // Calcul de l'angle en passant par les quaternions
    Quaternion q;  // Représente l'orientation
    VectorFloat p; // Vecteur pesanteur
    centrale_inertielle.dmpGetQuaternion(&q, fifoBuffer);
    centrale_inertielle.dmpGetGravity(&p, &q);
    centrale_inertielle.dmpGetYawPitchRoll(centrale_inertielle_angles, &q, &p);
  }
}
