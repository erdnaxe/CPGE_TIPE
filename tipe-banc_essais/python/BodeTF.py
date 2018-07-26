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
from Bode import Bode


class BodeTF(Bode):
    """
    Objet représentant un diagramme de Bode
    créé à partie d'une fonction de transfert
    """

    def calc_gain(v):
        """ Calcule le gain en décibel du complexe v """

        return 20 * np.log10(abs(v))

    def calc_phase(v):
        """ Calcule la phase du complexe v """

        r = v.real
        i = v.imag

        if r == 0:
            return 0
        elif r * i > 0:
            return np.arctan(i / r) - np.pi
        else:
            return np.arctan(i / r)

    def plot(self, fonction_de_transfert):
        """ Trace le diagramme des gains et des phases d'une fonction de transfert """

        # Calcul des points
        self.omegas = np.logspace(self.ten_pow_min, self.ten_pow_max, 100)
        vals = [fonction_de_transfert(x) for x in self.omegas]
        self.gains = [self.calc_gain(v) for v in vals]
        self.phases = [self.calc_phase(v) for v in vals]

        # Affichage
        super().plot()
