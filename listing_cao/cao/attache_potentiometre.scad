$fn = 100;  // Nombre de facettes dans une révolution
d   = 21.8; // Diamètre du roulement
l   = 65;   // Distance centre axe - bord droit
h   = 20;   // Profondeur de la piece

hauteur = d+4*2;  // Hauteur de la pièce
vide = 22.4-6.5;  // Vide derrière le roulement

difference() {
    translate([-l/2-d/2-4,-h/2,-hauteur/2])  cube([l+d/2+4, h, hauteur]);

    // Trous pour le potentiomètre et son roulement
    translate([-l/2,-h/2+8,0]) rotate([90,0,0]) cylinder(r=d/2,h=8+1);
    translate([-l/2,-h/2+30,0]) rotate([90,0,0]) cylinder(r=5,h=60);

    // Vide derrière
    translate([-l/2-d/2-4-40,0,-hauteur/2-1]) cube([l+d/2+4, h/2+1, hauteur+2]);
    translate([l/2+5-40,-h/2-1,-(hauteur-8)/2]) cube([40-10,h-vide,hauteur-8]);

    // Trous pour les 3 vis
    translate([0,-h/2+30,0]) rotate([90,0,0]) cylinder(r=3/2,h=60);
    translate([24,-h/2+30,+8]) rotate([90,0,0]) cylinder(r=3/2,h=60);
    translate([24,-h/2+30,-8]) rotate([90,0,0]) cylinder(r=3/2,h=60);
}
