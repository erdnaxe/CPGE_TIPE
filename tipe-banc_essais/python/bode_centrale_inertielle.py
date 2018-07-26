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

from Bode import Bode
from ComBanc import ComBanc
import matplotlib.pyplot as plt
import numpy as np
import time


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
        max_pot - moy_pot) * 10  # en degrées
    imu = -mesures[:, 5] * 180 / np.pi  # en degrées

    banc.close()  # Fermeture de la connexion

    return [temps, commande, potentiometre, imu]


def gain(entree, sortie):
    amplitude_entree = max(entree) - min(entree)
    amplitude_sortie = max(sortie) - min(sortie)

    return 20 * np.log10(abs(amplitude_sortie / amplitude_entree))


# Instance du banc d'essais
banc = ComBanc(baudrate=115200, port='/dev/ttyUSB0')
""" POUR FAIRE L'ACQUISITION D'UN POINT
temps, commande, potentiometre, imu = acquisition(banc, 1.5)
print(gain(potentiometre[5:], imu[5:]))

plt.figure(figsize=(8, 4))
plt.plot(temps[5:], potentiometre[5:], "+:", label="Potentiomètre")
plt.plot(temps[5:], imu[5:], "+:", label="IMU")
plt.xlabel("Temps (en secondes)")
plt.grid(); plt.legend(); plt.show()
"""

# Trace le diagramme de Bode
bode_imu = Bode(title="Gain de Bode expérimental de la centrale inertielle")

bode_imu.omegas, bode_imu.gains = np.loadtxt("bode_imu.csv")

# On affiche le résultat
plt.figure(figsize=(8, 4))
bode_imu.plot_gain()
plt.tight_layout()
plt.show()
