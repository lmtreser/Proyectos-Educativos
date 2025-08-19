// Joystick ESP32
// Nombre de red: Joystick_ESP32
// Contrseña de red: 12345678
// IP a conectarse: 192.168.100.100
// Autor: Prof. Ing. Juan Mercadin
// Dependencias:
// https://github.com/me-no-dev/AsyncTCP
// https://github.com/me-no-dev/ESPAsyncWebServer

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <WebSocketsServer.h>

// WiFi AP credentials
const char* ssid = "Joystick_ESP32";
const char* password = "12345678";

// Motor control pins (assuming L298N bridge)
// Left motor
#define LEFT_FORWARD 26   // IN1
#define LEFT_BACKWARD 27  // IN2
// Right motor
#define RIGHT_FORWARD 32  // IN3
#define RIGHT_BACKWARD 33 // IN4

// Additional digital outputs for buttons
#define BUTTON_UP_PIN 12    // Top button
#define BUTTON_RIGHT_PIN 13 // Right button
#define BUTTON_DOWN_PIN 14  // Bottom button
#define BUTTON_LEFT_PIN 15  // Left button

// Threshold for joystick (to detect direction, values from -100 to 100)
const int THRESHOLD = 30;

// Async WebServer on port 80
AsyncWebServer server(80);

// WebSocket server on port 81
WebSocketsServer webSocket(81);

// HTML page with CSS and JS
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, orientation=landscape">
    <title>ESP32 Remote Control</title>
    <style>
        body {
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: space-between;
            align-items: center;
            height: 100vh;
            background-color: #f0f0f0;
            font-family: Arial, sans-serif;
            overflow: hidden;
            flex-direction: row;
        }
        #joystick-container {
            width: 50%;
            height: 100%;
            display: flex;
            justify-content: center;
            align-items: center;
            touch-action: none;
            transform: translateY(-10vh); /* Mueve hacia arriba para un punto intermedio */
        }
        #joystick {
            width: 200px;
            height: 200px;
            background-color: #ddd;
            border-radius: 50%;
            position: relative;
        }
        #joystick-base-hole {
            width: 20px;
            height: 20px;
            background-color: #000;
            border-radius: 50%;
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
        }
        #joystick-stick {
            position: absolute;
            bottom: 50%;
            left: 50%;
            transform-origin: bottom center;
            background-color: #8B4513; /* Marrón para la palanca */
            width: 10px; /* Grosor de la palanca */
            border-radius: 5px;
        }
        #joystick-handle {
            width: 80px; /* Tamaño original de la bola */
            height: 80px;
            background-color: #ff0000; /* Color rojo para la bola */
            border-radius: 50%;
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
        }
        #buttons-container {
            width: 50%;
            height: 100%;
            display: flex;
            justify-content: center;
            align-items: center;
            transform: translateY(-10vh); /* Mueve hacia arriba para un punto intermedio */
        }
        #buttons {
            display: grid;
            grid-template-areas:
                " . up . "
                "left . right"
                " . down . ";
            grid-gap: 20px;
            width: 200px;
            height: 200px;
        }
        .button {
            width: 60px;
            height: 60px;
            background-color: #007bff;
            color: white;
            font-size: 24px;
            border: none;
            border-radius: 50%;
            display: flex;
            justify-content: center;
            align-items: center;
            touch-action: manipulation;
        }
        #up { grid-area: up; }
        #right { grid-area: right; }
        #down { grid-area: down; }
        #left { grid-area: left; }
        @media (orientation: portrait) {
            body {
                flex-direction: column;
            }
            #joystick-container, #buttons-container {
                width: 100%;
                height: 50%;
                transform: none; /* Sin transformación en portrait */
            }
        }
    </style>
</head>
<body>
    <div id="joystick-container">
        <div id="joystick">
            <div id="joystick-base-hole"></div>
            <div id="joystick-stick"></div>
            <div id="joystick-handle"></div>
        </div>
    </div>
    <div id="buttons-container">
        <div id="buttons">
            <button id="up" class="button">A</button>
            <button id="right" class="button">B</button>
            <button id="down" class="button">C</button>
            <button id="left" class="button">D</button>
        </div>
    </div>
    <script>
        const ws = new WebSocket('ws://' + location.hostname + ':81');

        // Joystick
        const joystick = document.getElementById('joystick');
        const stick = document.getElementById('joystick-stick');
        const handle = document.getElementById('joystick-handle');
        const radius = joystick.offsetWidth / 2;
        const handleRadius = handle.offsetWidth / 2;
        let centerX = radius;
        let centerY = radius;
        let isDragging = false;

        function updateJoystick(event) {
            event.preventDefault();
            const touch = event.touches ? event.touches[0] : event;
            const rect = joystick.getBoundingClientRect();
            let x = touch.clientX - rect.left - radius;
            let y = touch.clientY - rect.top - radius;
            const dist = Math.sqrt(x * x + y * y);
            if (dist > radius - handleRadius) {
                x = (x / dist) * (radius - handleRadius);
                y = (y / dist) * (radius - handleRadius);
            }
            handle.style.left = (radius + x) + 'px';
            handle.style.top = (radius + y) + 'px';

            // Actualizar la palanca
            const angle = Math.atan2(y, x) * (180 / Math.PI) + 90; /* Corregir desfase de 90° */
            const length = Math.sqrt(x * x + y * y);
            stick.style.height = length + 'px';
            stick.style.transform = `translate(-50%, 0) rotate(${angle}deg)`;

            const normX = Math.round((x / (radius - handleRadius)) * 100);
            const normY = Math.round((y / (radius - handleRadius)) * -100); // Invert Y for forward/back
            ws.send(`joystick:${normX},${normY}`);
        }

        function resetJoystick() {
            handle.style.left = '50%';
            handle.style.top = '50%';
            stick.style.height = '0px';
            stick.style.transform = 'translate(-50%, 0) rotate(0deg)';
            ws.send('joystick:0,0');
            isDragging = false;
        }

        joystick.addEventListener('touchstart', (e) => {
            isDragging = true;
            updateJoystick(e);
        });
        joystick.addEventListener('touchmove', updateJoystick);
        joystick.addEventListener('touchend', resetJoystick);

        // For mouse support (optional)
        joystick.addEventListener('mousedown', (e) => {
            isDragging = true;
            updateJoystick(e);
        });
        joystick.addEventListener('mousemove', (e) => {
            if (isDragging) updateJoystick(e);
        });
        joystick.addEventListener('mouseup', resetJoystick);
        joystick.addEventListener('mouseleave', resetJoystick);

        // Buttons
        const buttons = {
            up: document.getElementById('up'),
            right: document.getElementById('right'),
            down: document.getElementById('down'),
            left: document.getElementById('left')
        };

        Object.keys(buttons).forEach(key => {
            buttons[key].addEventListener('touchstart', () => ws.send(`button:${key}:1`));
            buttons[key].addEventListener('touchend', () => ws.send(`button:${key}:0`));
            // Mouse fallback
            buttons[key].addEventListener('mousedown', () => ws.send(`button:${key}:1`));
            buttons[key].addEventListener('mouseup', () => ws.send(`button:${key}:0`));
        });
    </script>
</body>
</html>
)rawliteral";

// WebSocket event handler
void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      break;
    case WStype_CONNECTED:
      break;
    case WStype_TEXT:
      if (payload) {
        String message = (char*)payload;
        if (message.startsWith("joystick:")) {
          String data = message.substring(9);
          int commaIndex = data.indexOf(',');
          int x = data.substring(0, commaIndex).toInt();
          int y = data.substring(commaIndex + 1).toInt();
          handleJoystick(x, y);
        } else if (message.startsWith("button:")) {
          String data = message.substring(7);
          int colonIndex = data.indexOf(':');
          String button = data.substring(0, colonIndex);
          int state = data.substring(colonIndex + 1).toInt();
          handleButton(button, state);
        }
      }
      break;
  }
}

// Handle joystick input
void handleJoystick(int x, int y) {
  // Reset all motors
  digitalWrite(LEFT_FORWARD, LOW);
  digitalWrite(LEFT_BACKWARD, LOW);
  digitalWrite(RIGHT_FORWARD, LOW);
  digitalWrite(RIGHT_BACKWARD, LOW);

  if (abs(x) < THRESHOLD && abs(y) < THRESHOLD) {
    // In rest position
    return;
  }

  // Forward/Backward
  if (y > THRESHOLD) { // Forward
    digitalWrite(LEFT_FORWARD, HIGH);
    digitalWrite(RIGHT_FORWARD, HIGH);
  } else if (y < -THRESHOLD) { // Backward
    digitalWrite(LEFT_BACKWARD, HIGH);
    digitalWrite(RIGHT_BACKWARD, HIGH);
  }

  // Left/Right (override for turning)
  if (x < -THRESHOLD) { // Left
    if (y > THRESHOLD) { // Forward left
      digitalWrite(LEFT_FORWARD, LOW);
      digitalWrite(RIGHT_FORWARD, HIGH);
    } else if (y < -THRESHOLD) { // Backward left
      digitalWrite(LEFT_BACKWARD, LOW);
      digitalWrite(RIGHT_BACKWARD, HIGH);
    } else { // Pure left turn
      digitalWrite(LEFT_BACKWARD, HIGH);
      digitalWrite(RIGHT_FORWARD, HIGH);
    }
  } else if (x > THRESHOLD) { // Right
    if (y > THRESHOLD) { // Forward right
      digitalWrite(LEFT_FORWARD, HIGH);
      digitalWrite(RIGHT_FORWARD, LOW);
    } else if (y < -THRESHOLD) { // Backward right
      digitalWrite(LEFT_BACKWARD, HIGH);
      digitalWrite(RIGHT_BACKWARD, LOW);
    } else { // Pure right turn
      digitalWrite(LEFT_FORWARD, HIGH);
      digitalWrite(RIGHT_BACKWARD, HIGH);
    }
  }
}

// Handle button input
void handleButton(String button, int state) {
  int pin = -1;
  if (button == "up") pin = BUTTON_UP_PIN;
  else if (button == "right") pin = BUTTON_RIGHT_PIN;
  else if (button == "down") pin = BUTTON_DOWN_PIN;
  else if (button == "left") pin = BUTTON_LEFT_PIN;

  if (pin != -1) {
    digitalWrite(pin, state);
  }
}

void setup() {
  // Initialize serial
  Serial.begin(115200);

  // Set pin modes
  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(LEFT_BACKWARD, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);
  pinMode(RIGHT_BACKWARD, OUTPUT);
  pinMode(BUTTON_UP_PIN, OUTPUT);
  pinMode(BUTTON_RIGHT_PIN, OUTPUT);
  pinMode(BUTTON_DOWN_PIN, OUTPUT);
  pinMode(BUTTON_LEFT_PIN, OUTPUT);

  // Reset all outputs
  digitalWrite(LEFT_FORWARD, LOW);
  digitalWrite(LEFT_BACKWARD, LOW);
  digitalWrite(RIGHT_FORWARD, LOW);
  digitalWrite(RIGHT_BACKWARD, LOW);
  digitalWrite(BUTTON_UP_PIN, LOW);
  digitalWrite(BUTTON_RIGHT_PIN, LOW);
  digitalWrite(BUTTON_DOWN_PIN, LOW);
  digitalWrite(BUTTON_LEFT_PIN, LOW);

  // Configure static IP for AP
  IPAddress local_IP(192, 168, 100, 100);
  IPAddress gateway(192, 168, 100, 100);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.softAPConfig(local_IP, gateway, subnet);

  // Start WiFi AP
  WiFi.softAP(ssid, password);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // Serve the HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Start server
  server.begin();

  // Start WebSocket server
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
}

void loop() {
  webSocket.loop();
}
