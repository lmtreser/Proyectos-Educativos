#include <WiFi.h>
#include <WebServer.h>
#include "ui_joystick.h"
#include "const.h"

// Crear servidor en el puerto 80
WebServer server(80);

// IP fija deseada
IPAddress local_IP(192, 168, 100, 100);
IPAddress gateway(192, 168, 10, 1);
IPAddress subnet(255, 255, 255, 0);

void setup() {

  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Iniciar WiFi en modo AP
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ssid, password);
  Serial.println("AP Init!");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  // Página principal
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", getHTML());
  });

  // Comandos de movimiento
  server.on("/cmd", HTTP_GET, []() {
    String dir = server.arg("dir");
    Serial.println("Comando: " + dir);
    mover(dir);
    server.send(200, "text/plain", "OK");
  });

  server.begin();
  Serial.println("HTTP Server Init!");
}

void loop() {
  server.handleClient();
}

// Control de motores
void mover(String comando) {

  if (comando == "adelante") {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else if (comando == "atras") {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else if (comando == "izquierda") {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else if (comando == "derecha") {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else if (comando == "stop") {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  } else {
    // Futuras funciones para botones extra
    // Podés agregar comportamiento aquí
  }
}