// Bibliothèque Gear Generator écrite par Longrange, sous licence Creative Commons Attribution.
use <GearGeneratorV2.scad>;

r = 6/2+0.1;  // Rayon de l'axe du potentiomètre

difference() {
    union() {
        linear_extrude(height=5, center=true, convexity=10)
            gear(number_of_teeth=35,diametral_pitch=1);
        cylinder(h=5, r1=8/2+0.25+5, r2=8/2+0.25, $fn=100);
        cylinder(h=5+7, r=8/2+0.25, $fn=100);
    }
    cylinder(h=(5+10)*2, r=r, center=true, $fn=100);
}
