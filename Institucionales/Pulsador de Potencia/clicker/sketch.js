/*  Proyecto Educativo: "Mando Interactivo Inclusivo"
    
    Institución: E.E.S.T. N° 3 "Domingo Faustino Sarmiento"
    Especialidad: Técnico en Electrónica
    Alumno: Brusco, Ramiro
    Docente Asesor: Treser, Lucas Martín

    Junio de 2023

*/

// Variables globales
let oscilador;
let clicker = false;
let tiempo;
let colorStroke = "#F88D4B";
let colorTexto = "#000000";
let colorBackground = "#A6FAB8";
let cajaTexto = "mando interactivo inclusivo";

function setup() {
  createCanvas(windowWidth, windowHeight);

  // Oscilador senoidal
  oscilador = new p5.Oscillator('sine');
  oscilador.amp(5);
  tiempo = millis();
  
  // Estilo de texto
  textSize(48);
  strokeWeight(15);
}

function draw() {
  background(colorBackground);

  // Texto a mostrar
  stroke(colorStroke);
  fill(colorTexto);
  text(cajaTexto, width / 4, height / 2);

  // Detener el oscilador después de un tiempo
  if (millis() > tiempo + 750){
    tiempo = millis();
    oscilador.stop();
  }

}

// Evento ante un clic del mouse
function mouseClicked() {

  clicker = !clicker;

  if (clicker == true) {
    colorStroke = "#4BF8F8";
    colorBackground = "#F88D4B";
  } else {
    colorStroke = "#F88D4B";
    colorBackground = "#A6FAB8";
  }
}

// Evento ante un doble clic del mouse
function doubleClicked() {
  
  // Generar una señal senoidal de frecuencia aleatoria
  let frecuencia = random(100, 650);
  oscilador.freq(frecuencia);
  oscilador.start();

}

// Adapta el tamaño del canvas a la pantalla
function windowResized() {
  resizeCanvas(windowWidth, windowHeight);
}
