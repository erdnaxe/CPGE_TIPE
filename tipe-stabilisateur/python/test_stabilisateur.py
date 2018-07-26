"""
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
"""

from websocket import create_connection
import matplotlib.pyplot as plt
import numpy as np
from struct import pack


def faire_acquisition(kp, ki, kd, n_mesures):
    """ Règle le correcteur et réalise une acquisition en Wifi """

    ws = create_connection("ws://192.168.4.1:81")

    # Réglage du correcteur
    param_bin = pack('ddd', kp, ki, kd)
    ws.send_binary(param_bin)

    # Réception
    donnees = np.zeros((n_mesures, 4), dtype=np.float)
    for i in range(n_mesures):
        recu = ws.recv()
        print(recu)
        donnees[i, :] = recu.split(",")

    ws.close()

    # Conversions
    temps = (donnees[:, 0] - donnees[0, 0]) / 1000  # Temps en seconde
    servo = donnees[:, 1] + 1375  # Commande servomoteur en us
    pot = donnees[:, 2]  # Valeur CAN du potentiomètre
    imu = donnees[:, 3] * 180 / np.pi  # Angle de la centrale en degrées

    return temps, servo, pot, imu


def tracer_entree_sortie_correcteur(temps, servo, pot, imu):
    plt.figure(figsize=(12, 4))

    plt.subplot("121")
    plt.plot(temps, imu, label="Centrale inertielle")
    plt.title("Signal d'erreur")
    plt.xlabel("Temps (secondes)")
    plt.ylabel("Angle (degrés)")
    plt.legend(); plt.grid(); plt.ylim(-90, 90)

    plt.subplot("122")
    plt.plot(temps, servo, label="Commande servomoteur")
    plt.title("Sortie du correcteur")
    plt.xlabel("Temps (secondes)")
    plt.ylabel("Commande (us)")
    plt.legend(); plt.grid(); plt.ylim(550, 2200)

    plt.tight_layout()
    plt.show()


# Réglages
kp, ki, kd = 100., 3300., 0.
n_mesures = 100

temps, servo, pot, imu = faire_acquisition(kp, ki, kd, n_mesures)
tracer_entree_sortie_correcteur(temps, servo, pot, imu)
