/*
  ROBOT_FUTBOL - Control por Bluetooth con ESP32
  ------------------------------------------------
  Placa: ESP32 NodeMCU WROOM 38 pines
  Driver de motores: L298N
  App recomendada: "Bluetooth RC Controller"

  Comandos por Bluetooth:
    F/B/L/R/S = adelante/atrás/izq/der/stop
    G/I/H/J   = diagonales (adel-izq, adel-der, atrás-izq, atrás-der)
    0-9, q    = velocidad (PWM 120-255)

  Cambios en esta versión:
    - Rango de velocidad ampliado (120-255 en vez de 80-180)
    - Pulso de arranque a máxima potencia (60ms) al iniciar movimiento
      desde parado, para vencer la inercia/fricción estática del motor
*/

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth no está habilitado. Verifica la configuración de la placa.
#endif

BluetoothSerial SerialBT;

// --- Pines del L298N (lado derecho físico del ESP32) ---
int VelocidadMotor1 = 17; // GPIO17 -> ENA (PWM motor A)   | pata física N°28
int VelocidadMotor2 = 16; // GPIO16 -> ENB (PWM motor B)   | pata física N°27
int Motor1A = 4;          // GPIO4  -> IN1 (dirección A)   | pata física N°26
int Motor1B = 2;          // GPIO2  -> IN2 (dirección A)   | pata física N°24
int Motor2C = 15;         // GPIO15 -> IN3 (dirección B)   | pata física N°23
int Motor2D = 5;          // GPIO5  -> IN4 (dirección B)   | pata física N°29

// --- Configuración PWM (API nueva del core ESP32 3.x) ---
const int frecuenciaPWM = 5000;
const int resolucionPWM = 8;

int speedMotor = 200; // velocidad por defecto (rango 120-255)
char comando;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ROBOT_FUTBOL");
  Serial.println("ROBOT_FUTBOL listo. Esperando conexión Bluetooth...");

  pinMode(Motor1A, OUTPUT);
  pinMode(Motor1B, OUTPUT);
  pinMode(Motor2C, OUTPUT);
  pinMode(Motor2D, OUTPUT);

  ledcAttach(VelocidadMotor1, frecuenciaPWM, resolucionPWM);
  ledcAttach(VelocidadMotor2, frecuenciaPWM, resolucionPWM);

  detenerMotores();
}

void loop() {
  if (SerialBT.available()) {
    comando = SerialBT.read();
    Serial.print("Comando recibido: ");
    Serial.println(comando);
    ejecutarComando(comando);
  }
}

void ejecutarComando(char cmd) {
  switch (cmd) {
    case 'F': moveForward(); break;
    case 'B': moveBackward(); break;
    case 'L': turnLeft(); break;
    case 'R': turnRight(); break;
    case 'S': detenerMotores(); break;
    case 'G': moveForwardLeft(); break;
    case 'I': moveForwardRight(); break;
    case 'H': moveBackwardsLeft(); break;
    case 'J': moveBackwardsRight(); break;
    case 'q':
      speedMotor = 255;
      actualizarVelocidad();
      break;
    default:
      if (cmd >= '0' && cmd <= '9') {
        int nivel = cmd - '0';
        speedMotor = map(nivel, 0, 9, 120, 255);
        actualizarVelocidad();
      }
      break;
  }
}

void actualizarVelocidad() {
  ledcWrite(VelocidadMotor1, speedMotor);
  ledcWrite(VelocidadMotor2, speedMotor);
}

// --- Pulso de arranque para vencer la inercia del motor ---
void arrancarConImpulso(int pinA, int pinB) {
  ledcWrite(pinA, 255);
  ledcWrite(pinB, 255);
  delay(60); // pulso breve a máxima potencia
}

// --- Movimientos básicos ---

void moveForward() {
  digitalWrite(Motor1A, HIGH);
  digitalWrite(Motor1B, LOW);
  digitalWrite(Motor2C, HIGH);
  digitalWrite(Motor2D, LOW);
  arrancarConImpulso(VelocidadMotor1, VelocidadMotor2);
  actualizarVelocidad();
}

void moveBackward() {
  digitalWrite(Motor1A, LOW);
  digitalWrite(Motor1B, HIGH);
  digitalWrite(Motor2C, LOW);
  digitalWrite(Motor2D, HIGH);
  arrancarConImpulso(VelocidadMotor1, VelocidadMotor2);
  actualizarVelocidad();
}

void turnLeft() {
  digitalWrite(Motor1A, LOW);
  digitalWrite(Motor1B, HIGH);
  digitalWrite(Motor2C, HIGH);
  digitalWrite(Motor2D, LOW);
  arrancarConImpulso(VelocidadMotor1, VelocidadMotor2);
  actualizarVelocidad();
}

void turnRight() {
  digitalWrite(Motor1A, HIGH);
  digitalWrite(Motor1B, LOW);
  digitalWrite(Motor2C, LOW);
  digitalWrite(Motor2D, HIGH);
  arrancarConImpulso(VelocidadMotor1, VelocidadMotor2);
  actualizarVelocidad();
}

void detenerMotores() {
  digitalWrite(Motor1A, LOW);
  digitalWrite(Motor1B, LOW);
  digitalWrite(Motor2C, LOW);
  digitalWrite(Motor2D, LOW);
  ledcWrite(VelocidadMotor1, 0);
  ledcWrite(VelocidadMotor2, 0);
}

// --- Diagonales (motor "de adentro" a velocidad reducida, no detenido) ---

void moveForwardLeft() {
  digitalWrite(Motor1A, HIGH);
  digitalWrite(Motor1B, LOW);
  digitalWrite(Motor2C, HIGH);
  digitalWrite(Motor2D, LOW);
  arrancarConImpulso(VelocidadMotor1, VelocidadMotor2);
  int velExtra = min(speedMotor + 30, 255);
  int velLenta = speedMotor - 40 < 0 ? 0 : speedMotor - 40;
  ledcWrite(VelocidadMotor1, velLenta); // motor izquierdo más lento
  ledcWrite(VelocidadMotor2, velExtra);
}

void moveForwardRight() {
  digitalWrite(Motor1A, HIGH);
  digitalWrite(Motor1B, LOW);
  digitalWrite(Motor2C, HIGH);
  digitalWrite(Motor2D, LOW);
  arrancarConImpulso(VelocidadMotor1, VelocidadMotor2);
  int velExtra = min(speedMotor + 30, 255);
  int velLenta = speedMotor - 40 < 0 ? 0 : speedMotor - 40;
  ledcWrite(VelocidadMotor1, velExtra);
  ledcWrite(VelocidadMotor2, velLenta); // motor derecho más lento
}

void moveBackwardsLeft() {
  digitalWrite(Motor1A, LOW);
  digitalWrite(Motor1B, HIGH);
  digitalWrite(Motor2C, LOW);
  digitalWrite(Motor2D, HIGH);
  arrancarConImpulso(VelocidadMotor1, VelocidadMotor2);
  int velExtra = min(speedMotor + 30, 255);
  int velLenta = speedMotor - 40 < 0 ? 0 : speedMotor - 40;
  ledcWrite(VelocidadMotor1, velLenta);
  ledcWrite(VelocidadMotor2, velExtra);
}

void moveBackwardsRight() {
  digitalWrite(Motor1A, LOW);
  digitalWrite(Motor1B, HIGH);
  digitalWrite(Motor2C, LOW);
  digitalWrite(Motor2D, HIGH);
  arrancarConImpulso(VelocidadMotor1, VelocidadMotor2);
  int velExtra = min(speedMotor + 30, 255);
  int velLenta = speedMotor - 40 < 0 ? 0 : speedMotor - 40;
  ledcWrite(VelocidadMotor1, velExtra);
  ledcWrite(VelocidadMotor2, velLenta);
}
