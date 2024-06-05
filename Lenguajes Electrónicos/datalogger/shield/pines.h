#ifndef PINES_H
#define PINES_H

// I2C address
const int RTC_ADDR = 0x68;
const int LCD_ADDR = 0x27;

// Configuracion de pines
const byte DHT_PIN = 2;
const byte LED = 3;
const byte PULSADOR = 4; // Activo alto
const byte RS485_RO = 5;
const byte RS485_DI = 6;
const byte RS485_RE_DE = 7; // LOW = RX, HIGH = TX
const byte SD_SS = 10;

#endif