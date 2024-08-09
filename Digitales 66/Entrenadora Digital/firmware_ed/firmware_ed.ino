/*
    ENTRENADORA DIGITAL
    
    Placa basada en Arduino Nano para realizar prácticas de programación.

    Creado 05 12 2023
    Lucas Martín Treser

*/

#include "pinout.h"

// Instancia de objetos
// .....

// Variables globales
bool valorA = false;
bool valorB = true;

// Constantes
const int TIEMPO = 1000;

void setup() {

  pinMode(OUT_PWR_1, OUTPUT);
  pinMode(OUT_PWR_2, OUTPUT);
  pinMode(OUT_PWR_3, OUTPUT);
  pinMode(OUT_PWR_4, OUTPUT);
  pinMode(OUT_PWR_5, OUTPUT);
  pinMode(OUT_PWR_6, OUTPUT);
  pinMode(OUT_PWR_7, OUTPUT);
  pinMode(OUT_PWR_8, OUTPUT);

  pinMode(BOTON_1, INPUT);
  pinMode(BOTON_2, INPUT);
  pinMode(BOTON_3, INPUT);
  pinMode(BOTON_4, INPUT);

  Serial.begin(9600);
  delay(TIEMPO);
}

void loop() {

  // Lectura entradas digitales (pulsadores)
  int estadoBoton1 = digitalRead(BOTON_1);
  int estadoBoton2 = digitalRead(BOTON_2);
  int estadoBoton3 = digitalRead(BOTON_3);
  int estadoBoton4 = digitalRead(BOTON_4);

  Serial.print("Boton 1: ");
  Serial.print(estadoBoton1);
  Serial.print("\tBoton 2: ");
  Serial.print(estadoBoton2);
  Serial.print("\tBoton 3: ");
  Serial.print(estadoBoton3);
  Serial.print("\tBoton 4: ");
  Serial.println(estadoBoton4);

  // Lectura entradas analogicas (potenciometros)
  int estadoPot1 = analogRead(POT_1);
  int estadoPot2 = analogRead(POT_2);

  Serial.print("\nPotenciometro 1: ");
  Serial.print(estadoPot1);
  Serial.print("\tPotenciometro 2: ");
  Serial.println(estadoPot2);

  // Escritura salidas digitales
  valorA = !valorA;
  valorB = !valorB;

  digitalWrite(OUT_PWR_1, valorA);
  digitalWrite(OUT_PWR_2, valorA);
  digitalWrite(OUT_PWR_3, valorA);
  digitalWrite(OUT_PWR_4, valorA);
  digitalWrite(OUT_PWR_5, valorB);
  digitalWrite(OUT_PWR_6, valorB);
  digitalWrite(OUT_PWR_7, valorB);
  digitalWrite(OUT_PWR_8, valorB);

  delay(TIEMPO);
}
