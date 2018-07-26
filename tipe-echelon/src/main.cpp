/*
    This file is part of the TIPE step source code.

    TIPE step is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TIPE step is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this source code.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Arduino.h"
#include "ESP8266WiFi.h"
#include <Hash.h>
#include <Servo.h>
#include <SimpleTimer.h>
#include <WebSocketsServer.h>

// Timers
SimpleTimer timer_calcul_commande;
SimpleTimer timer_transmission_websocket;

// Objets
Servo servomoteur;
WebSocketsServer webSocket = WebSocketsServer(81);

// Variables pour la commande
unsigned long temps_commande = 0; // ms, temps à partir du début de la commande
unsigned long temps_commande_orig = 0; // ms, décalage temporel de l'origine
unsigned long temps_duree_rampe = 0;   // ms, durée de la rampe
int angle_servo = 950;                 // us, angle à envoyer au servomoteur
const int angle_servo_min = 950;       // us, angle minimal du servomoteur
const int angle_servo_max = 1800;      // us, angle maximal du servomoteur

// Initialisation du point d'accès Wifi
void initialiser_wifi() {
  WiFi.mode(WIFI_AP);                 // Point d'accès WiFi
  IPAddress ip_stab(192, 168, 4, 1);  // IP du stabilisateur & passerelle
  IPAddress masque(255, 255, 255, 0); // Masque de sous-réseau
  WiFi.softAPConfig(ip_stab, ip_stab, masque); // Serveur DHCP
  WiFi.softAP("Stabilisateur", "t1p3stabi");   // Nom et mot de passe du Wifi
}

// Commande en rampe
void commande_servomoteur() {
  if (temps_duree_rampe == 0) {
    // Aucune donnée n'a été reçue donc retour à l'origine
    angle_servo = angle_servo_min;
  } else {
    temps_commande = millis() - temps_commande_orig;

    if (temps_commande > 5000 and temps_commande < temps_duree_rampe + 5000) {
      // Effectue la rampe
      angle_servo = (temps_commande - 5000) *
                        (angle_servo_max - angle_servo_min) /
                        temps_duree_rampe +
                    angle_servo_min;
    } else if (temps_commande > temps_duree_rampe + 10000) {
      // La rampe est terminéee
      temps_duree_rampe = 0;
    }
  }

  servomoteur.writeMicroseconds(angle_servo);
}

// Transmet une mesure vers le socket réseau
void transmission_websocket() {
  int pot = analogRead(A0);
  String texte =
      String(temps_commande) + "," + String(angle_servo) + "," + String(pot);
  webSocket.broadcastTXT(texte);
}

// Gère la réception d'une commande (inspiré d'un exemple de WebSocket)
void rx_websocket(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT and payload[0] == '#') {
    // Parsing de la commande (de la forme #FFF)
    uint32_t recu = (uint32_t)strtol((const char *)&payload[1], NULL, 16);
    int num_vitesse = ((recu >> 4) & 0xFF);      // 2 premiers chiffres
    int exp_vitesse = ((recu >> 0) & 0xF) - 0x8; // Puissance de 10

    // Calcul de la vitesse et de la durée de la rampe
    float vitesse = num_vitesse * pow(10, exp_vitesse);                // us/ms
    temps_duree_rampe = (angle_servo_max - angle_servo_min) / vitesse; // ms

    // Déplacement de l'origine temporel
    temps_commande_orig = millis();

    // Renvoie de la durée pour vérification
    webSocket.broadcastTXT("#" + String(temps_duree_rampe));
  }
}

// Initialisation
void setup() {
  servomoteur.attach(D6);          // Commande du servomoteur
  pinMode(A0, INPUT);              // Entrée du potentiomètre du servomoteur
  initialiser_wifi();              // Point d'accès Wifi
  webSocket.begin();               // Initialisation du Websocket
  webSocket.onEvent(rx_websocket); // Réception d'une commande

  timer_calcul_commande.setInterval(25, commande_servomoteur);
  timer_transmission_websocket.setInterval(75, transmission_websocket);
}

// Boucle principale qui lance les sous-boucles
void loop() {
  timer_calcul_commande.run();
  timer_transmission_websocket.run();
  webSocket.loop();
}
