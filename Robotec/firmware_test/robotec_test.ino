/*
    ROBOTEC TEST
    Sketch para probar dispositivos roboticos

    Para leer la distancia utiliza la biblioteca "Ultrasonic" de Erick Simoes
    https://github.com/ErickSimoes/Ultrasonic

    24 de noviembre de 2023
    Lucas Martin Treser
*/

#include <Ultrasonic.h>
#include "pines.h"

// Sensores de distancia
Ultrasonic ultrasonicFrl(ULTRASON_TRIG_1, ULTRASON_ECHO_1);
Ultrasonic ultrasonicIzq(ULTRASON_TRIG_2, ULTRASON_ECHO_2);
Ultrasonic ultrasonicDer(ULTRASON_TRIG_3, ULTRASON_ECHO_3);

// Variables globales
int velocidadMA = 255;
int velocidadMB = 255;

void setup() {

  pinMode(MA_IN1, OUTPUT);
  pinMode(MA_IN2, OUTPUT);
  pinMode(MB_IN1, OUTPUT);
  pinMode(MB_IN2, OUTPUT);
  pinMode(LED_1, OUTPUT);

  pinMode(BOT_START, INPUT);
  pinMode(TRACKING_1, INPUT);
  pinMode(TRACKING_2, INPUT);
  pinMode(TRACKING_3, INPUT);

  Serial.begin(9600);
  Serial.println("Init ok!");

  // Retardo de inicio de marcha
  digitalWrite(LED_1, HIGH);
  delay(1000);
  digitalWrite(LED_1, LOW);
}

void loop() {

  // Leer estado del pulsador
  bool estadoBoton = digitalRead(BOT_START);
  Serial.print("Pulsador: ");
  Serial.println(estadoBoton);

  // Leer estado de sensores de distancia
  int distanciaFrontal = ultrasonicFrl.read();
  int distanciaIzquierda = ultrasonicIzq.read();
  int distanciaDerecha = ultrasonicDer.read();

  Serial.print("Sensor Frontal: ");
  Serial.print(distanciaFrontal);
  Serial.print("\tSensor Izquierda: ");
  Serial.print(distanciaIzquierda);
  Serial.print("\tSensor Derecha: ");
  Serial.println(distanciaDerecha);

  // Leer estado de sensores IR
  int tracking = digitalRead(TRACKING_1);
  Serial.print("Sensor IR: ");
  Serial.println(tracking);

  // Activar Motor A
  analogWrite(MA_ENABLE, velocidadMA);
  digitalWrite(MA_IN1, LOW);
  digitalWrite(MA_IN2, HIGH);

    // Activar Motor B
  analogWrite(MB_ENABLE, velocidadMB);
  digitalWrite(MB_IN1, LOW);
  digitalWrite(MB_IN2, HIGH);

  delay(2000);

}
