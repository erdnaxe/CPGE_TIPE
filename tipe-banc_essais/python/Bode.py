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

import numpy as np
import matplotlib.pyplot as plt


class Bode:
    """
    Objet représentant un diagramme de Bode
    """

    def __init__(self, title="", puissance_dix_min=-1, puissance_dix_max=2):
        self.title = title
        self.puissance_dix_min = puissance_dix_min
        self.puissance_dix_max = puissance_dix_max

        self.omegas = []
        self.gains = []
        self.phases = []

    def plot_gain(self):
        """ Trace le diagramme des gains """

        plt.plot(self.omegas, self.gains, color="green")

        # Réglages
        plt.xscale("log")
        plt.xlim(10**self.puissance_dix_min, 10**self.puissance_dix_max)
        plt.title(self.title)
        plt.grid()
        plt.xlabel("x")
        plt.ylabel("GdB")
        plt.axhline(color="black")

    def plot_phase(self):
        """ Trace le diagramme des phases """

        plt.plot(self.omegas, self.phases, color="green")

        # Réglages
        plt.xscale("log")
        plt.xlim(10**self.puissance_dix_min, 10**self.puissance_dix_max)
        plt.grid()
        plt.xlabel("omega")
        plt.ylabel("Phase")
        plt.axhline(color="black")

    def plot(self):
        """ Trace le diagramme de Bode complet """

        plt.subplot(211)
        self.plot_gain()

        plt.subplot(212)
        self.plot_phase()
