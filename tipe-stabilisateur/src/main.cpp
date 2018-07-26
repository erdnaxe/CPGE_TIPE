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

#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "centrale_inertielle.h"
#include <Hash.h>
#include <PID_v1.h>
#include <Servo.h>
#include <SimpleTimer.h>
#include <WebSocketsServer.h>

// Variables pour la commande
double angle_mesure = 0.; // rad, angle mesuré par la centrale inertielle
double angle_servo = 0.;  // us, angle à envoyer au servomoteur
double angle_voulu = 0.;  // Angle correspondant à l'horizontale

// Objets
SimpleTimer timer_transmission_websocket;
Servo servomoteur;
WebSocketsServer webSocket = WebSocketsServer(81);
PID correcteur(&angle_mesure, &angle_servo, &angle_voulu, 100, 3300, 0,
               REVERSE); // Kp=100, Ki=Kd=0

// Initialisation du point d'accès Wifi
void initialiser_wifi() {
  WiFi.mode(WIFI_AP);                 // Point d'accès WiFi
  IPAddress ip_stab(192, 168, 4, 1);  // IP du stabilisateur & passerelle
  IPAddress masque(255, 255, 255, 0); // Masque de sous-réseau
  WiFi.softAPConfig(ip_stab, ip_stab, masque); // Serveur DHCP
  WiFi.softAP("Stabilisateur", "t1p3stabi");   // Nom et mot de passe du Wifi
}

// Transmet une mesure vers le socket réseau
void transmission_websocket() {
  int pot = analogRead(A0);
  int temps_actuel = millis();
  String texte = String(temps_actuel) + "," + String(angle_servo) + "," +
                 String(pot) + "," + String(angle_mesure);
  webSocket.broadcastTXT(texte);
}

// Gère la réception d'une commande
void rx_websocket(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_BIN) {
    double *donnees = (double *)payload; // On reçoit 3 double

    if (length / sizeof(double) != 3) {
      // Si on ne reçoit pas 3 éléments, alors problème
      return;
    }

    // On change les paramètres du correcteur
    correcteur.SetTunings(donnees[0], donnees[1], donnees[2]);
  }
}

// Initialisation
void setup() {
  servomoteur.attach(D6);            // Commande du servomoteur
  pinMode(A0, INPUT);                // Entrée du potentiomètre du servomoteur
  initialiser_centrale_inertielle(); // Centrale inertielle
  initialiser_wifi();                // Point d'accès Wifi
  webSocket.begin();                 // Initialisation du Websocket
  webSocket.onEvent(rx_websocket);   // Réception d'une commande

  // Transmet une valeur toutes les 60ms
  timer_transmission_websocket.setInterval(60, transmission_websocket);

  correcteur.SetOutputLimits(-825, 825); // Domaine délimité en us
  correcteur.SetSampleTime(20);          // Calcul tous les 25ms
  correcteur.SetMode(AUTOMATIC);         // Active le correcteur
}

// Boucle principale qui lance les sous-boucles
void loop() {
  // Appel des différents Timer
  boucle_centrale_inertielle();       // Centrale inertielle
  correcteur.Compute();               // Correcteur
  timer_transmission_websocket.run(); // Transmission des données
  webSocket.loop();                   // WebSocket

  // Transfert des variables
  angle_mesure = centrale_inertielle_angles[2];
  servomoteur.writeMicroseconds(angle_servo+1375);
}
