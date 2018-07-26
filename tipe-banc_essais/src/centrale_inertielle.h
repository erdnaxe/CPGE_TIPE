/*
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
*/

#ifndef IMU_DMP_h
#define IMU_DMP_h

#define PIN_SDA A4
#define PIN_SCL A5
#define PIN_INTERRUPTION 2

// Paramètres de calibration
#define X_GYRO_OFFSET 99
#define Y_GYRO_OFFSET -1
#define Z_GYRO_OFFSET -4
#define X_ACCEL_OFFSET -2073
#define Y_ACCEL_OFFSET -1429
#define Z_ACCEL_OFFSET 1147

extern bool centrale_inertielle_initialisee; // vraie si succès d'initialisation
extern float centrale_inertielle_angles[3];  // lacet, roulis et tangage (rad)

extern void initialiser_centrale_inertielle();
extern void boucle_centrale_inertielle();

#endif
