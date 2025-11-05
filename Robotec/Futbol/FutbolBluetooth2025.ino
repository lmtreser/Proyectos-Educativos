#include <SoftwareSerial.h>

// CONEXIONES PARA MODO AT:
// HC-05 TX → Arduino Pin 2
// HC-05 RX → Arduino Pin 3  
// HC-05 EN/KEY → Arduino Pin 3.3V (ANTES de encender)
// HC-05 VCC → Arduino 5V
// HC-05 GND → Arduino GND

SoftwareSerial bluetooth(2, 3); // RX, TX para modo AT

void setup() {
  Serial.begin(9600);
  bluetooth.begin(38400); // HC-05 en modo AT usa 38400 baudios
  
  Serial.println("=== CONFIGURADOR HC-05 ===");
  Serial.println("Comandos disponibles:");
  Serial.println("AT         - Probar conexión");
  Serial.println("AT+ORGL    - Reset a fábrica");
  Serial.println("AT+NAME?   - Ver nombre");
  Serial.println("AT+PSWD?   - Ver PIN");
  Serial.println("AT+UART?   - Ver velocidad");
  Serial.println("AT+VERSION? - Ver versión");
  Serial.println("=============================");
  Serial.println("");
  Serial.println("Ingrese comando AT:");
}

void loop() {
  // Enviar datos del HC-05 al monitor serie
  if (bluetooth.available()) {
    String response = bluetooth.readString();
    Serial.print("HC-05: ");
    Serial.println(response);
  }
  
  // Enviar comandos del monitor serie al HC-05
  if (Serial.available()) {
    String command = Serial.readString();
    command.trim(); // Eliminar espacios en blanco
    bluetooth.println(command);
    Serial.print("Enviado: ");
    Serial.println(command);
  }
}