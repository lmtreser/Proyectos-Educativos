// Parámetros
base_largo = 60;   // Largo de la base en mm
base_ancho = 30;   // Ancho de la base en mm
base_altura = 3;   // Grosor de la base en mm
pared_altura = 40; // Altura de la pared vertical
pared_grosor = 3;  // Grosor de la pared vertical
refuerzo_grosor = 3; // Grosor del triángulo de refuerzo
orificio_diam = 3;  // Diámetro de los orificios de montaje

module base() {
    difference() {
        cube([base_largo, base_ancho, base_altura]); 
        // Orificios en la base
        translate([10, 10, -1]) cylinder(h = base_altura + 2, d = orificio_diam);
        translate([50, 10, -1]) cylinder(h = base_altura + 2, d = orificio_diam);
    }
}

module pared() {
    translate([0, 0, base_altura])
    difference() {
        cube([pared_grosor, base_ancho, pared_altura]); 
        // Orificios en la pared
        translate([-1, 10, 20]) rotate([90, 0, 0]) cylinder(h = pared_grosor + 2, d = orificio_diam);
        translate([-1, 20, 30]) rotate([90, 0, 0]) cylinder(h = pared_grosor + 2, d = orificio_diam);
    }
}

module refuerzo() {
    translate([0, 0, base_altura])
    linear_extrude(height = refuerzo_grosor)
    polygon(points=[[0,0], [0, pared_altura], [pared_altura, 0]]);
}

// Ensamblaje
base();
pared();
refuerzo();
