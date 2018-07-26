import numpy as np
import matplotlib.pyplot as plt
from scipy import stats


def lireDonneesLatis(nom_fichier):
    """ Charge le fichier Latis en une matrice de données """

    donnees = np.loadtxt(nom_fichier, delimiter=';', skiprows=1)
    donnees[:, 1] *= 9  # conversion tension en degree, facteur constructeur
    return donnees


def trouverCoupures(liste_grandeur, liste_temps):
    """ Renvoie les temps où la grandeur passe du signe - à + """

    liste_coupures = []
    for i, t in enumerate(liste_temps[:-1]):
        if liste_grandeur[i] < 0 and liste_grandeur[i + 1] > 0:
            liste_coupures.append([i, t])
    return np.array(liste_coupures)


def tronquerDonnees(donnees, n_periodes):
    """ Tronque le début des données et laisse n_periodes périodes """

    liste_coupures = trouverCoupures(donnees[:, 1], donnees[:, 0])
    debut = int(liste_coupures[0, 0])
    if n_periodes:
        fin = int(liste_coupures[n_periodes, 0])
    else:
        fin = -1
    return donnees[debut:fin, :], liste_coupures[:n_periodes, :]


def plotAngle(titre, fichier, from_i, n_periodes, calcul_periodes=True):
    """ Trace l'angle du pendule à partir du point from_i sur n_periodes périodes """

    donnees_brut = lireDonneesLatis(fichier)

    # Centre les points
    donnees_brut[:, 1] += -np.average(donnees_brut[-10000:, 1])

    # Tronque les données
    donnees, liste_coupures = tronquerDonnees(donnees_brut[from_i:, :],
                                              n_periodes)
    plt.xlim(donnees[0, 0], donnees[-1, 0])
    m = max(donnees[:, 1]) + 1
    plt.ylim(-m, m)

    # Graduations et calcul de la période
    if calcul_periodes:
        plt.gca().set_xticks(np.round(liste_coupures[:, 1], 1))
        O = []
        for i in range(len(liste_coupures[:, 1]) - 1):
            O.append(liste_coupures[i + 1, 1] - liste_coupures[i, 1])
        moyenne = np.average(O)
        ecart_type = np.std(O)
        periode = "Période = " + str(np.round(moyenne, 2)) + " +/- " + str(
            np.round(ecart_type, 2)) + " s"
        style_text_pyplot = dict(
            boxstyle='square', facecolor='white', alpha=0.8)
        plt.text(
            donnees[-1, 0],
            m + 1,
            titre + " : " + periode,
            bbox=style_text_pyplot,
            fontsize=12,
            verticalalignment='top',
            horizontalalignment='right')

    # Graphique
    plt.xlabel("Temps (en seconde)")
    plt.ylabel("Angle du\npendule (degrés)")
    plt.plot(donnees[:, 0], donnees[:, 1], "k.", markersize=1.)
    plt.grid()


for a in ["20", "30"]:
    plt.figure(figsize=(10, 3))
    plt.subplot(211)
    plotAngle("Sans la caméra", "data/s_" + a + ".csv", -6000, 20)
    plt.subplot(212)
    plotAngle("Avec la caméra", "data/a_" + a + ".csv", -6000, 20)
    plt.tight_layout()

plt.show()
