/*
    Testeo Shield Datalogger
    Creado 5 de junio de 2024
*/

#include "pines.h"
#include <DHT.h>
#include <SoftwareSerial.h>

DHT dht(DHT_PIN, DHT11);
SoftwareSerial portSerial(RS485_DI, RS485_RO);  // RX, TX

void setup() {

  pinMode(PULSADOR, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(RS485_RE_DE, OUTPUT);

  portSerial.begin(9600);
  portSerial.println("Pruebas RS-485");

  dht.begin();

  // Habilitar depuración por Monitor Serial
  Serial.begin(9600);
  Serial.println("Depuracion habilitada");
}

void loop() {

  // Obtener variables
  float humedad = dht.readHumidity();
  float temperatura = dht.readTemperature();

  // Enviar datos por monitor Serial
  Serial.print(humedad);
  Serial.print(",");
  Serial.println(temperatura);

  // Enviar datos por RS-485
  digitalWrite(RS485_RE_DE, HIGH);
  portSerial.print(humedad);
  portSerial.print(",");
  portSerial.println(temperatura);

  delay(2000);
}
