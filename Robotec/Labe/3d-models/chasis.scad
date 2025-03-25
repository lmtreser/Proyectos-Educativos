// Parámetros generales
chasis_largo = 100;    // Largo de la placa en mm
chasis_ancho = 100;    // Ancho de la placa en mm
chasis_grosor = 3;     // Grosor de la placa
radio_esquinas = 10;   // Radio de las esquinas redondeadas
espacio_orificios = 10; // Separación de los orificios en mm
diametro_orificio = 3; // Diámetro de los orificios

module chasis() {
    difference() {
        // Placa con esquinas redondeadas
        hull() {
            translate([radio_esquinas, radio_esquinas, 0]) cylinder(h=chasis_grosor, r=radio_esquinas);
            translate([chasis_largo - radio_esquinas, radio_esquinas, 0]) cylinder(h=chasis_grosor, r=radio_esquinas);
            translate([radio_esquinas, chasis_ancho - radio_esquinas, 0]) cylinder(h=chasis_grosor, r=radio_esquinas);
            translate([chasis_largo - radio_esquinas, chasis_ancho - radio_esquinas, 0]) cylinder(h=chasis_grosor, r=radio_esquinas);
        }

        // Orificios distribuidos en una cuadrícula
        for (x = [espacio_orificios/2 : espacio_orificios : chasis_largo - espacio_orificios/2]) {
            for (y = [espacio_orificios/2 : espacio_orificios : chasis_ancho - espacio_orificios/2]) {
                translate([x, y, -1]) cylinder(h = chasis_grosor + 2, d = diametro_orificio);
            }
        }
    }
}

// Renderizar el chasis
chasis();
