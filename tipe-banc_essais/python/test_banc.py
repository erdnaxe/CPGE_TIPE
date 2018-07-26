"""
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
"""

from ComBanc import ComBanc
import matplotlib.pyplot as plt
import numpy as np
import time

amplitude = 10


def acquisition(banc, omega):
    """
    Réalise une sollicitation harmonique et renvoie les mesures du potentiomètre
    et de la centrale inertielle en fonction du temps ainsi que la commande
    """

    banc.open()  # Ouvre la connexion et démarre le banc d'essais
    time.sleep(5)  # Laisse le temps à la centrale de converger
    banc.changer_pulsation(omega)

    # Acquisition de 1000 valeurs (1000*0.01 = 1s)
    mesures = banc.acquisition(1000)

    # Alignement du potentiomètre et conversions
    temps = mesures[:, 0] / 1000  # en secondes
    max_pot = max(mesures[:, 2])
    moy_pot = (max_pot + min(mesures[:, 2])) / 2
    commande = mesures[:, 1]
    potentiometre = (mesures[:, 2] - moy_pot) / (
        max_pot - moy_pot) * amplitude  # en degrées
    imu = -mesures[:, 5] * 180 / np.pi  # en degrées

    banc.close()  # Fermeture de la connexion

    return [temps, commande, potentiometre, imu]


def tracer_potentiometre_imu(temps, commande, potentiometre, imu):
    plt.figure(figsize=(8, 4))

    plt.subplot(211)
    plt.title("Commande et retour potentiomètre")
    plt.plot(temps, commande, "+:", label="Commande")
    plt.plot(temps, potentiometre, "+:", label="Potentiomètre")
    plt.xlabel("Temps (en secondes)")
    plt.grid()
    plt.legend()

    plt.subplot(212)
    plt.title("Commande et retour MPU6050")
    plt.plot(temps, commande, "+:", label="Commande")
    plt.plot(temps, imu, "+:", label="MPU6050 (Roulis)")
    plt.xlabel("Temps (en secondes)")
    plt.grid()
    plt.legend()

    plt.tight_layout()


# Instance du banc d'essais
banc = ComBanc(baudrate=115200, port='/dev/ttyUSB0')

temps, commande, potentiometre, imu = acquisition(banc, 3.14156)
tracer_potentiometre_imu(temps, commande, potentiometre, imu)

plt.show()
