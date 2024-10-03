/*
  ModbusRTU Slave

  Ejemplo de configuración de comunicación Modbus sobre RS-485 para placa Arduino UNO
  o compatible. Utiliza la biblioteca "ModbusRTUSlave" de C. M. Bulliner y SoftwareSerial
  para crear un puerto serie virtual.

  https://github.com/CMB27/ModbusRTUSlave
  https://docs.arduino.cc/learn/built-in-libraries/software-serial/

  Funciones disponibles: 

  Coils (Bobinas). Salidas digitales, permiten lectura y escritura.
    Read Coils (Función 01)
    Write Single Coil (Función 05)
    Write Multiple Coils (Función 15)
  
  Discrete Inputs. Entradas digitales de solo lectura.
    Read Discrete Inputs (Función 02)

  Holding Registers. Registros de 16 bits, almacenan valores enteros.
    Read Holding Registers (Función 03)
    Write Single Register (Función 06)
    Write Multiple Registers (Función 16)

  Input Registers. Registros de 16 bits de solo lectura.
    Read Input Registers (Función 04)

  Circuito:
  
    - Pines A0, A1: potenciómetros
    - Pines D2, D3: Pulsador a GND
    - Pines D5, D6, D7, D8: LED a GND
    - Pin 10: SoftwareSerial RX
    - Pin 11: SoftwareSerial TX
    - Pin 13: Driver enable (HIGH para transmitir)

  
  Created: 2023-07-22
  By: C. M. Bulliner
  Modified: 2024-01-27
  By: C. M. Bulliner
  Last Modified: 2024-10-03
  By: L. M. Treser

*/

//=====[Bibliotecas]=============================================================
#include <SoftwareSerial.h>
#include <ModbusRTUSlave.h>

//=====[Constantes]==============================================================
const byte potPins[2] = { A0, A1 };
const byte buttonPins[2] = { 2, 3 };
const byte ledPins[4] = { 5, 6, 7, 8 };
const byte dePin = 13;
const byte rxPin = 10;
const byte txPin = 11;

//=====[Variables]===============================================================
bool coils[2];
bool discreteInputs[2];
uint16_t holdingRegisters[2];
uint16_t inputRegisters[2];

//=====[Objetos]=================================================================
SoftwareSerial mySerial(rxPin, txPin);
ModbusRTUSlave modbus(mySerial, dePin);  // Serial port, driver enable pin

//=====[Main]====================================================================

void setup() {

  // Configuracion de pines E/S
  pinMode(buttonPins[0], INPUT_PULLUP);
  pinMode(buttonPins[1], INPUT_PULLUP);
  pinMode(ledPins[0], OUTPUT);
  pinMode(ledPins[1], OUTPUT);
  pinMode(ledPins[2], OUTPUT);
  pinMode(ledPins[3], OUTPUT);

  // bool array of coil values, number of coils
  modbus.configureCoils(coils, 2);
  // bool array of discrete input values, number of discrete inputs
  modbus.configureDiscreteInputs(discreteInputs, 2);
  // unsigned 16 bit integer array of holding register values, number of holding registers
  modbus.configureHoldingRegisters(holdingRegisters, 2);
  // unsigned 16 bit integer array of input register values, number of input registers
  modbus.configureInputRegisters(inputRegisters, 2);

  modbus.begin(1, 38400);
}

void loop() {

  inputRegisters[0] = map(analogRead(potPins[0]), 0, 1023, 0, 255);
  inputRegisters[1] = map(analogRead(potPins[1]), 0, 1023, 0, 255);
  discreteInputs[0] = !digitalRead(buttonPins[0]);
  discreteInputs[1] = !digitalRead(buttonPins[1]);

  modbus.poll();

  analogWrite(ledPins[0], holdingRegisters[0]);
  analogWrite(ledPins[1], holdingRegisters[1]);
  digitalWrite(ledPins[2], coils[0]);
  digitalWrite(ledPins[3], coils[1]);
}