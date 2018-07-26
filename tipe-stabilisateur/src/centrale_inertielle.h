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

#ifndef IMU_DMP_h
#define IMU_DMP_h

#define PIN_SDA D2
#define PIN_SCL D1
#define PIN_INTERRUPTION D7

// Paramètres de calibration
#define X_GYRO_OFFSET 53
#define Y_GYRO_OFFSET -61
#define Z_GYRO_OFFSET 9
#define X_ACCEL_OFFSET -815
#define Y_ACCEL_OFFSET 1353
#define Z_ACCEL_OFFSET 1667

extern bool centrale_inertielle_initialisee; // vraie si succès d'initialisation
extern float centrale_inertielle_angles[3];  // lacet, roulis et tangage (rad)

extern void initialiser_centrale_inertielle();
extern void boucle_centrale_inertielle();

#endif
