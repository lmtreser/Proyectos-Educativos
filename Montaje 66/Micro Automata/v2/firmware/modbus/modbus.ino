/*
  ┌─────────────────────────────────────────────────────────────┐
  │           AUTOMÁTA DIDÁCTICO (Versión Bare Metal)           │
  │           ------------------------------------------------  │
  │      Sistema educativo para prácticas de automatización     │
  │         con integración futura de comunicación Modbus RTU   │
  └─────────────────────────────────────────────────────────────┘

  Descripción:
    Aplicación educativa sin sistema operativo (bare metal)
    que simula un autómata industrial con entradas/salidas digitales,
    control de actuadores y monitoreo por puerto serie o Modbus RTU.

  Funciones principales:
    • Lectura de entradas digitales (sensores, pulsadores, FC)
    • Actualización de salidas (relés, LEDs, actuadores)
    • Control básico secuencial (toggle por flanco descendente)
    • Comunicación Modbus RTU esclavo (en desarrollo)
    • Reporte periódico del estado por puerto serie (modo DEBUG)

  --------------------------------------------------------------------
  Hardware sugerido:
    • Arduino UNO / NANO / MEGA (MCU AVR 8 bits)
    • Entradas aisladas con optoacopladores PC817
    • Salidas a relé o transistor (NPN)
    • Pulsadores con condensador de 100 nF en paralelo (antirrebote)

  --------------------------------------------------------------------
  Dependencias:
    • ModbusRTUSlave.h — Biblioteca de esclavo Modbus RTU
      https://github.com/CMB27/ModbusRTUSlave
    • defines.h — Archivo de definición de pines y parámetros

  --------------------------------------------------------------------
  Autor: Prof. Lucas Martín  
  Institución: Escuelas Técnicas de Mar del Plata  
  Proyecto: Automáta Didáctico (Bare Metal + Modbus RTU)  
  Versión: 0.2  
  Fecha: Noviembre 2025  
  Licencia: MIT — Uso libre con fines educativos  
*/

#include "defines.h"
#include <SoftwareSerial.h>
#include <ModbusRTUSlave.h>

const uint8_t INPUTS_PINS[]{ INPUT_IN1, INPUT_IN2, INPUT_IN3, INPUT_IN4 };
const uint8_t OUTPUTS_PINS[]{ RELAY_IN1, RELAY_IN2 };

const uint8_t NUM_IN = sizeof(INPUTS_PINS) / sizeof(INPUTS_PINS[0]);
const uint8_t NUM_OUT = sizeof(OUTPUTS_PINS) / sizeof(OUTPUTS_PINS[0]);

// Tiempos de muestreo
const uint32_t READ_INTERVAL = 10;
const uint32_t WRITE_INTERVAL = 10;
const uint32_t PRINT_INTERVAL = 1000;

// Variables de control de tiempo
uint32_t lastReadTime = 0;
uint32_t lastWriteTime = 0;
uint32_t lastPrintTime = 0;

// Registros Modbus
bool coils[NUM_OUT];
bool discreteInputs[NUM_IN];

struct pinData {
  uint8_t inStatus[NUM_IN];
  uint8_t outStatus[NUM_OUT];
};

pinData ioData{};
SoftwareSerial modbusSerial(COMM_RO, COMM_DI);  // RX, TX
ModbusRTUSlave modbusSlave(modbusSerial, COMM_DRE);

void readIO() {
  for (size_t i = 0; i < NUM_IN; ++i) {
    ioData.inStatus[i] = digitalRead(INPUTS_PINS[i]);
    discreteInputs[i] = ioData.inStatus[i];
  }
  for (size_t i = 0; i < NUM_OUT; ++i) {
    ioData.outStatus[i] = digitalRead(OUTPUTS_PINS[i]);
  }
}

void writeOutputs() {
  static uint8_t prevInputState[NUM_IN] = { HIGH, HIGH, HIGH, HIGH };

  for (size_t i = 0; i < NUM_OUT; ++i) {
    if (prevInputState[i] == HIGH && ioData.inStatus[i] == LOW) {
      ioData.outStatus[i] = !ioData.outStatus[i];
      coils[i] = !ioData.outStatus[i];  // sincroniza coils
      digitalWrite(OUTPUTS_PINS[i], ioData.outStatus[i]);
    }
    prevInputState[i] = ioData.inStatus[i];
  }
}

void syncOutputs() {
  for (int i = 0; i < NUM_OUT; i++) {
    bool out_nodered = !coils[i];
    bool out_now = ioData.outStatus[i];
    if (out_nodered != out_now) {
      ioData.outStatus[i] = out_nodered;
      digitalWrite(OUTPUTS_PINS[i], out_nodered);
    }
  }
}

void printStatus() {
  Serial.print("INPUTS: ");
  for (size_t i = 0; i < NUM_IN; ++i) {
    Serial.print(ioData.inStatus[i]);
  }
  Serial.print(" OUTPUTS: ");
  for (size_t i = 0; i < NUM_OUT; ++i) {
    Serial.print(ioData.outStatus[i]);
  }
  Serial.println();
}

void setup() {

#ifdef DEBUG
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println(F("Init..."));
#endif
  modbusSerial.begin(MB_BAUDRATE);
  modbusSlave.begin(SLAVE_ID, MB_BAUDRATE);
  modbusSlave.configureCoils(coils, NUM_OUT);
  modbusSlave.configureDiscreteInputs(discreteInputs, NUM_IN);

  for (auto pin : INPUTS_PINS) { pinMode(pin, INPUT_PULLUP); }
  for (auto pin : OUTPUTS_PINS) { pinMode(pin, OUTPUT); }
}

void loop() {
  modbusSlave.poll();
  unsigned long now = millis();

  // Leer I/O cada 10 ms
  if (now - lastReadTime >= READ_INTERVAL) {
    lastReadTime = now;
    readIO();
  }

  // Actualizar salidas cada 10 ms
  if (now - lastWriteTime >= WRITE_INTERVAL) {
    lastWriteTime = now;
    writeOutputs();
    syncOutputs();
  }

#ifdef DEBUG
  // Imprimir cada 1 segundo
  if (now - lastPrintTime >= PRINT_INTERVAL) {
    lastPrintTime = now;
    printStatus();
  }
#endif
}
