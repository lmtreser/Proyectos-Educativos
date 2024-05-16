/*
    Prototipo educativo de un "Datalogger"
    implementado con un sensor de temperatura
    DHT22 y un RTC DS1307. Utiliza las bibliotecas:
    
    RTClib de Adafruit
    https://github.com/adafruit/RTClib

    DHT Sensor de Adafruit
    https://github.com/adafruit/DHT-sensor-library

    Para el tiempo, usa el formato "UnixTime"
    https://es.wikipedia.org/wiki/Tiempo_Unix
    https://www.unixtimestamp.com/

    Simulación disponible en línea en Wokwi
    https://wokwi.com/projects/366275938732959745

*/

#include <DHT.h>
#include <Wire.h>
#include <RTClib.h>
#include <EEPROM.h>

// Constantes
const int DHTPIN = 2;
const int DHTTYPE = DHT22;

// Objetos
DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 rtc;

// Arreglos de datos
float temperaturas[10];
float humedades[10];
unsigned long timestamps[10];

void setup() {

  Serial.begin(9600);
  dht.begin();

  if (!rtc.begin()) {
    Serial.println("No se pudo encontrar el RTC");
    while (1)
      ;
  }
  if (!rtc.isrunning()) {
    Serial.println("El RTC no está funcionando. Configurando...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {

  // Tomar 10 muestras desde el sensor y el reloj
  // Los resultados se almacenan en los arreglos de datos
  for (int i = 0; i < 10; i++) {

    DateTime now = rtc.now();
    float temperatura = dht.readTemperature();
    float humedad = dht.readHumidity();

    temperaturas[i] = temperatura;
    humedades[i] = humedad;
    timestamps[i] = now.unixtime();

    delay(1000);
  }

  guardarEnEEPROM();
  leerDesdeEEPROM();
  delay(20000);
}

// Funcion para guardar los datos en la EEPROM
void guardarEnEEPROM() {
  int direccion = 0;
  for (int i = 0; i < 10; i++) {
    EEPROM.put(direccion, temperaturas[i]);
    direccion += sizeof(float);
    EEPROM.put(direccion, humedades[i]);
    direccion += sizeof(float);
    EEPROM.put(direccion, timestamps[i]);
    direccion += sizeof(unsigned long);
  }
}

// Funcion para recuperar los datos desde la EEPROM
void leerDesdeEEPROM() {
  Serial.println("Datos almacenados en la EEPROM:");
  int direccion = 0;
  for (int i = 0; i < 10; i++) {
    float temperatura;
    EEPROM.get(direccion, temperatura);
    direccion += sizeof(float);
    float humedad;
    EEPROM.get(direccion, humedad);
    direccion += sizeof(float);
    unsigned long timestamp;
    EEPROM.get(direccion, timestamp);
    direccion += sizeof(unsigned long);
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.print(" °C, Humedad: ");
    Serial.print(humedad);
    Serial.print("%, Timestamp: ");
    Serial.println(timestamp);
  }
}
