$fn=100;     // Nombre de facettes dans une révolution
gap = 0.05;  // Espace pour encastrer les pièces

/* Tige */
module tige() {
    longueur_tige = 330.8;  // Mesurée au pied à coulisse
    diametre_tige = 12 + gap;  // ISO + espace pour emboîter
    decalage_trou = 8.6;  // Mesuré au pied à coulisse

    color([0.5, 0.5, 0.5, 1]) rotate([0,90,0]) difference() {
        cylinder(h=longueur_tige, r=diametre_tige/2, center=true);  // Tige
        translate([0,6.8,0]) cube([20,2,30], center=true);  // Partie limée
        translate([0,0,-decalage_trou]) rotate([90,0,0])
            cylinder(h=20, r=1.5+gap, center=true);  // Perçage
    }
}

/* Attache servo-tige */
module attache_servo_tige() {
    distance_attache = 16;  // Distance entre les 2 trous sur l'attache servo
    epaisseur_attache = 3;  // Épaisseur de l'attache donnée par le constructeur
    diametre_tige = 12 + gap;  // ISO + espace pour emboîter
    decalage_trou = 8.6;  // Mesuré au pied à coulisse

    union() {
        // Partie épousant la forme de la tige
        difference() {
            rotate([0, 90, 0]) cylinder(h=30, r=8, center=true);
            tige();
            translate([-decalage_trou,0,0])
                rotate([90,0,0]) cylinder(h=20, r=1.5+gap*2, center=true);
            
            // Ajout suite au prototype : permet d'autoriser une déformation
            translate([0,-2,7]) cube([32,1,5], center=true);
        }
        
        // Partie d'accroche au moteur
        difference() {
            translate([0,8.2,0]) rotate([90,0,0])
                cylinder(r=13, h=epaisseur_attache, center=true);  // Forme de base
            translate([-decalage_trou,8,0]) rotate([90,0,0]) 
                cylinder(h=5, r=5.3/2+gap, center=true);  // Trou de la vis
            translate([0,8,distance_attache/2]) rotate([90,0,0]) 
                cylinder(h=5, r=1+gap, center=true);  // Trou en haut
            translate([0,8,-distance_attache/2]) rotate([90,0,0]) 
                cylinder(h=5, r=1+gap, center=true);  // Trou en bas
        }
        
        // Renforcements
        difference() {
            translate([-10,0,diametre_tige/2+gap]) cube([20,8,1.8]);
            translate([0,5,distance_attache/2]) rotate([90,0,0]) cylinder(h=10, r=1.5, center=true);
        }
        difference() {
            translate([-10,0,-1.9-diametre_tige/2-gap]) cube([20,8,1.8]);
            translate([0,5,-distance_attache/2]) rotate([90,0,0]) cylinder(h=10, r=1.5, center=true);
        }
    }
}

attache_servo_tige();
