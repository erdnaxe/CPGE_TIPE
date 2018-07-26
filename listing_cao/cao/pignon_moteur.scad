// Bibliothèque Gear Generator écrite par Longrange, sous licence Creative Commons Attribution.
use <GearGeneratorV2.scad>;

r = 27.7/2;  // Rayon du moteur

difference() {
    linear_extrude(height=5, center=true, convexity=10)
            gear(number_of_teeth=35,diametral_pitch=1);
    cylinder(h=5*4, r=r, center=true, $fn=100);
}
