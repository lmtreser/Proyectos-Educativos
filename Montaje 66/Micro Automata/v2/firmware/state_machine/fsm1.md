🧠 ¿Qué es una FSM?

Una Máquina de Estados Finita es un modelo computacional basado en:

Estados (condiciones en las que puede estar el sistema)

Eventos que generan transiciones

Acciones que se ejecutan en esas transiciones o dentro de los estados


Es ideal para sistemas embebidos porque:

Es sencilla de programar (estructura clara: switch-case, if-else)

Visualmente es intuitiva para representar comportamientos reactivas

Es determinística (más fácil de verificar y simular que otros modelos)



---

📘 Caso de estudio: Sensor CO + Ventilador (FSM)

🎯 Requerimientos

Leer el sensor de CO cada segundo.

Si la lectura supera un umbral (ej. 100 ppm), encender ventilador.

Apagarlo cuando baje de otro umbral más bajo (histeresis, ej. 60 ppm).

Evitar cambios rápidos entre ON/OFF.



---

🛑 Estados posibles del sistema

Estado	Descripción

WAITING	Esperando la primera medición
MEASURING	Midiendo el nivel de CO
VENT_OFF	Ventilador apagado
VENT_ON	Ventilador encendido



---

🔄 Transiciones

Evento	Estado origen	Estado destino	Acción

Lectura válida	WAITING	MEASURING	Registrar lectura
CO < UmbralLow	MEASURING	VENT_OFF	Apagar ventilador
CO > UmbralHigh	MEASURING	VENT_ON	Encender ventilador
Timer (repetición de lectura)	Cualquiera	MEASURING	Leer nuevamente CO



---

🎨 Diagrama de Estados

┌───────────────┐                   ppm < 60
              │  WAITING      │───────────────┐───────────→ VENT_OFF
              └───────────────┘               │
                       │                      │ ppm > 100
                       ▼                      │
               ┌───────────────┐              └───────→ VENT_ON
               │  MEASURING    │
               └───────────────┘                          ↑
                       │ Timer 1s                         │ Timer 1s
                       └───────────────→ back to measuring


---

🧰 ¿Cómo se programa esto?

Una FSM se implementa normalmente con un switch-case sobre un estado global. Ejemplo simplificado (estilo Arduino/ESP32):

typedef enum {
    WAITING,
    MEASURING,
    VENT_OFF,
    VENT_ON
} SystemState;

SystemState state = WAITING;
float ppm = 0;

void loop() {
    switch (state) {
        case WAITING:
            state = MEASURING;
            break;

        case MEASURING:
            ppm = readCO();
            if (ppm > 100) state = VENT_ON;
            else if (ppm < 60) state = VENT_OFF;
            break;

        case VENT_ON:
            digitalWrite(FAN_PIN, HIGH);
            Serial.println("Ventilador ON");
            state = MEASURING;
            break;

        case VENT_OFF:
            digitalWrite(FAN_PIN, LOW);
            Serial.println("Ventilador OFF");
            state = MEASURING;
            break;
    }
    delay(1000);
}


---

📚 Fundamento formal: ¿qué es una FSM?

Una Máquina de Estado se define como una tupla:

> FSM = (S, Σ, δ, s₀)
Donde:

S: conjunto de estados

Σ: conjunto de eventos

δ: función de transición (δ(estado_actual, evento) = nuevo_estado)

s₀: estado inicial




Puede ser además:

Moore FSM: la salida depende sólo del estado actual.

Mealy FSM: la salida depende del estado y del evento recibido.


🧠 Tip didáctico: este ejemplo es estilo Moore, porque las acciones ("encender ventilador") se hacen al entrar en el estado.


---

🧪 Herramientas para enseñar FSMs

🔧 Simuladores y editores visuales

Qt FSM Editor: https://github.com/katzer/fsm-editor

Draw.io o Figma: para diseñar diagramas a mano.

Tina (Petri Nets + FSM): https://projects.laas.fr/tina/

Yakindu Statechart Tools (muy potente, usado en industria): https://www.itemis.com/en/yakindu/state-machine/


🧑‍🏫 Material para los alumnos

Explicá siempre:

1. Qué estados existen


2. Qué eventos definen las transiciones


3. Qué acciones se ejecutan al entrar / salir de un estado



Hacelos simular estados con carteles o símbolos en aula (“vos sos el ventilador”, “vos sos el sensor”).



---

⚖️ Comparación Red de Petri vs FSM

Aspecto	FSM	Red de Petri

Fácil para principiantes	✔	○ más abstracto
Modela concurrencia	○ difícil	✔ nativamente
Análisis formal	aceptable	excelente
Adecuado para	Alarmas, control secuencial	Procesos complejos, multitarea


👉 Ideal: enseñá FSM como puente pedagógico antes de Petri Nets.


---

🏁 Tips finales para la práctica

Usá FSM en Arduino, MicroPython o ESP para mostrarles que lo que vean gráficamente, puede ser codificado fácil.

Es excelente para ejercicios cortos: “dibujá la FSM de un control de luces”, etc.

Si después ven concurrencia (FreeRTOS, multitarea, MQTT), Petri les va a parecer una evolución natural.
