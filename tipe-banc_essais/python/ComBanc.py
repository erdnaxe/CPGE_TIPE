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

import serial
import time
import numpy as np


class ComBanc(serial.Serial):
    """
    Représente la communication avec le banc d'essais, hérite de Serial
    """

    def __init__(self, baudrate=9600, port='COM1'):
        super().__init__()

        self.baudrate = baudrate
        self.port = port

    def open(self):
        # Ouverture de la communication
        super().open()

        # Attendre que le banc démarre
        time.sleep(2)

    def changer_pulsation(self, omega):
        """ Permet de changer la pulsation de la sollicitation harmonique """

        print("Nouvelle pulsation :", omega)

        # Envoie de la commande
        cmd = str(omega)
        self.write(cmd.encode('utf-8'))

        # Attendre que le banc prenne la valeur en compte
        time.sleep(2)

    def acquisition(self, n):
        """ Permet d'acquerir n valeurs séparées par une virgule """

        # Vider le buffer d'entree
        self.flushInput()

        # Information
        print("Acquisition de " + str(n) + " couples")

        # Acquisition
        mesures = []
        for i in range(n):
            try:
                rx = str(self.readline()).split(",")
                mesures.append([
                    float(rx[0][2:]),
                    float(rx[1]),
                    float(rx[2]),
                    float(rx[3]),
                    float(rx[4]),
                    float(rx[5][:-5])
                ])
                print(mesures[-1])
            except ValueError:
                print("Erreur de reception")

        return np.array(mesures)
