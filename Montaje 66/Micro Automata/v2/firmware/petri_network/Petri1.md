1) Idea general (resumen)

El sensor mide CO periódicamente.

Si el nivel supera un umbral seguro, la lógica automática puede activar el ventilador.

El dashboard muestra la lectura y tiene un interruptor virtual que puede forzar el ventilador ON/OFF (modo manual).

Se evita el conflicto entre control automático y control manual mediante una plaza de arbitraje (modo): si el usuario pone MANUAL, la acción automática queda bloqueada hasta que vuelva a AUTO.

Además, se incluye histeresis y temporizador para evitar encendidos/apagados rápidos.


2) Elementos (nombres y significado)

Plazas (P...):

P0: Idle — sistema esperando medición.

P1: CO_measured — lectura disponible (token = lectura válida).

P2: CO_high — condición “CO por encima del umbral” (indica que la lectura supera el umbral alto).

P3: CO_normal — condición “CO por debajo del umbral bajo” (histeresis).

P4: Auto_enabled — token indica que el modo automático está activo.

P5: Manual_enabled — token indica modo manual activo (si está, bloquea auto).

P6: Fan_request_auto — pedido para encender ventilador desde lógica automática.

P7: Fan_request_manual_on — pedido ON desde dashboard (virtual switch = ON).

P8: Fan_request_manual_off — pedido OFF desde dashboard (virtual switch = OFF).

P9: Fan_on — ventilador físicamente encendido (token indica ventilador activo).

P10: Fan_off — ventilador físicamente apagado.

P11: MQTT_tx — permiso/cola para enviar mensaje MQTT (puede usar tokens para cola o semáforo).

P12: Safety_lock — recurso que evita acciones concurrentes peligrosas (mutex sobre fan).


Transiciones (T...):

T1: Measure — lectura del sensor, genera token en P1. (por timer periódico)

T2: Check_high — si lectura > UmbralHigh, mueve token a P2.

T3: Check_low — si lectura < UmbralLow, mueve token a P3.

T4: Auto_request_on — desde P2 y con P4 (Auto_enabled) y P12 (Safety_lock disponible) genera Fan_request_auto (P6).

T5: Auto_request_off — desde P3 y P4 genera petición de apagar (mover token a P8 or directly to Fan_off).

T6: Manual_on_cmd — llegada MQTT con comando ON: coloca token en P7 y coloca P5 (Manual_enabled).

T7: Manual_off_cmd — llegada MQTT OFF: coloca token en P8 (y si switch en OFF, podría devolver a AUTO).

T8: Arbitrate_manual — si llega Manual_enabled (P5) consume token Auto_enabled (P4) o bloquea T4/T5 (esto se modela con arcos inhibitor o consumiendo P4 temporariamente).

T9: Fan_activate — consume petición (P6 o P7) y Safety_lock (P12), mueve token a Fan_on (P9) y coloca P11 (para enviar MQTT de estado).

T10: Fan_deactivate — consume pedido de OFF (P8) y Safety_lock, mueve token a Fan_off (P10) y P11 (notificar).

T11: MQTT_send — consume P11 y envía mensajes (lectura+estado) por MQTT.

T12: Timeout_auto_release — si manual no es permanente, tras X segundos devuelve control a AUTO (pone token en P4 y quita P5).

T13: Safety_release — libera P12 después de completada la acción (o tras detectar que fan está estabilizado).


3) Marcado inicial (estado inicial)

Token en P0 (Idle) o directamente en P4 (Auto_enabled) + P10 (Fan_off) + P12 (Safety_lock).
Ejemplo: M0 = { P4:1, P10:1, P12:1 }.


4) Flujo: casos de uso principales (descripción paso a paso)

Caso A — lectura normal (sin peligro)

1. T1 Measure dispara periódicamente → token en P1.


2. T3 Check_low: si lectura < UmbralLow → token en P3.


3. Con P3 y P4 activo, T5 Auto_request_off puede dispararse → pone token en P8 (pedido OFF).


4. T10 Fan_deactivate consume P8 y P12 → mueve token a P10 (Fan_off) y genera P11 para notificar.


5. T11 MQTT_send envía estado y lectura.



Caso B — lectura alta -> encender ventilador (AUTO)

1. T1 Measure → P1.


2. T2 Check_high: lectura > UmbralHigh → P2.


3. Si P4 (Auto_enabled) está presente y P5 (Manual_enabled) no está, T4 Auto_request_on dispara → crea P6.


4. T9 Fan_activate consume P6 y P12 → pone token en P9 (Fan_on), quita P10, y agrega P11 (para notificación).


5. T11 MQTT_send publica lectura y estado fan:on.



Caso C — usuario fuerza ventilador desde dashboard (MANUAL)

1. Dashboard publica topic: home/CO/fan/cmd payload {"fan": "on", "mode":"manual"}.


2. Embebido recibe → T6 Manual_on_cmd dispara → coloca P7 (Fan_request_manual_on) y P5 (Manual_enabled).


3. Si P12 (Safety_lock) disponible, T9 Fan_activate consumirá P7→ P9 (Fan_on) y notificará.


4. Mientras P5 exista, T4/T5 (acciones automáticas) estarán bloqueadas (no pueden ejecutarse).


5. Usuario puede mandar off → T7 Manual_off_cmd dispara → P8 (request off). Si se desea volver a AUTO, dashboard puede mandar mode:auto o esperar T12 Timeout_auto_release.



5) Evitar conflictos — diseño del arbitraje

Implementar una plaza de modo (P4 / P5).

Cuando P5 (Manual) presente, las transiciones automáticas T4/T5 tienen un arco inhibidor desde P5 (es decir, no pueden disparar si P5 tiene token). Si tu herramienta no soporta inhibidores, haz que T6 consuma temporalmente el token P4 (Auto_enabled) y lo guarde en una plaza auxiliar para restaurarlo luego (T12).

Usar P12 (Safety_lock) como mutex para asegurar que Fan_activate y Fan_deactivate no se solapen.


6) Temporización y histeresis

Dos umbrales: UmbralHigh (p. ej. 100 ppm) y UmbralLow (p. ej. 60 ppm) para evitar oscilaciones.

Transiciones Check_high/Check_low pueden ser transiciones temporizadas o condicionales con conteo (ej.: lectura debe superar durante N lecturas consecutivas para disparar).

Auto_release (T12) puede ser temporizada (p. ej. 10 min de manual para volver a auto), o solo por comando explícito del dashboard.


7) Tópicos MQTT sugeridos (estándar práctico)

Publicar lecturas periódicas (retained opcional):

home/CO/reading → payload JSON: {"ppm": 123.4, "ts":"2025-11-15T21:00:00Z"} (QoS 1, retained=false/true según quieras last-value)


Estado del ventilador:

home/CO/fan/state → {"state":"on","mode":"auto"} (QoS1, retained=true)


Comandos desde dashboard:

home/CO/fan/cmd → payload {"state":"on","mode":"manual"} o {"state":"off","mode":"manual"} o {"mode":"auto"} (QoS1)


Confirmación/ack: el dispositivo puede publicar en home/CO/fan/ack.

Eventual alerta: home/CO/alert → {"level":"high","ppm":200} (para notificaciones)


Recomendaciones:

Usar QoS 1 para comandos y estados.

Marcar estado del fan como retained=true para que dashboard siempre muestre el último estado.

Validar payloads y autenticación (username/password o TLS) en broker.


8) Propiedades a verificar (análisis formal rápido)

Seguridad (safety): No puede existir Fan_on sin consumir Safety_lock — evitar solapamiento de solicitudes.

Liveness: Desde cualquier lectura alta, debería ser posible llegar a Fan_on (si Auto_enabled).

Ausencia de deadlocks: Asegúrate de que no haya lugar en que P12 quede consumido y nunca liberado (por ejemplo implementa T13 Safety_release siempre que finalice la acción).

No conflictos: Cuando Manual_enabled (P5) existe, T4/T5 quedan inhibidas.


9) Mapeo a implementación embebida (pseudocódigo, estructura)

Te doy un pseudo-mapeo sencillo para un ESP32 (puede hacerse en Arduino loop o en FreeRTOS tasks):

Estructura de tareas:

Task A: SensorTask (periodic, 1s) → lee CO, publica home/CO/reading, actualiza variable ppm. Luego evalúa umbrales y postea eventos a una cola (tokens virtuales).

Task B: MQTTTask → gestiona conexión, recibe home/CO/fan/cmd y pone evento Manual_on/Manual_off en la cola; publica fan/state.

Task C: ControlTask → consume eventos de la cola: aplica arbitraje (si modo manual, ignora auto), enciende/apaga GPIO del relé del ventilador, libera mutex, y pide notificación MQTT (manda P11).

Mutex/semáforo: safety_lock = xSemaphoreCreateBinary() (iniciado dado) — usado por ControlTask para asegurar exclusión al togglear el relé.

Histeresis: almacenar last_state y aplicar umbrales con contador de lecturas.


Pseudocódigo simplificado (sin FreeRTOS detallado):

float ppm;
bool auto_mode = true;
bool manual_mode = false;
bool fan_on = false;
SemaphoreHandle_t safety_lock;

void SensorTask(){
  while(1){
    ppm = readCO();
    mqtt_publish("home/CO/reading", json(ppm));
    if(!manual_mode && auto_mode){
      if(ppm > TH_HIGH && !fan_on) queue_post(EVENT_AUTO_ON);
      if(ppm < TH_LOW && fan_on) queue_post(EVENT_AUTO_OFF);
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void MQTTCallback(msg){
  // parse JSON
  if(msg.mode == "manual"){
    manual_mode = true;
    auto_mode = false;
    if(msg.state == "on") queue_post(EVENT_MANUAL_ON);
    else if(msg.state == "off") queue_post(EVENT_MANUAL_OFF);
  } else if(msg.mode == "auto") {
    manual_mode = false;
    auto_mode = true;
  }
}

void ControlTask(){
  while(1){
    event = queue_wait();
    if(event == EVENT_MANUAL_ON || event == EVENT_AUTO_ON){
      if(xSemaphoreTake(safety_lock, 100)){
         setFan(true); fan_on=true;
         mqtt_publish("home/CO/fan/state", json("on", manual_mode?"manual":"auto"));
         xSemaphoreGive(safety_lock);
      }
    }
    if(event == EVENT_MANUAL_OFF || event == EVENT_AUTO_OFF){
      if(xSemaphoreTake(safety_lock, 100)){
         setFan(false); fan_on=false;
         mqtt_publish("home/CO/fan/state", json("off", manual_mode?"manual":"auto"));
         xSemaphoreGive(safety_lock);
      }
    }
  }
}

10) Cosas prácticas / consideraciones

Debounce/confirmación: Requerir N lecturas consecutivas sobre el umbral antes de disparar (reduce falsas alarmas).

Logs y telemetría: publicar fan/state con retained=true para visualización estable.

Seguridad: usa MQTT con TLS si es posible y tópicos autenticados.

Hardware: usar relé o MOSFET según el ventilador (AC vs DC). Añadir protección contra picos en motores (snubber).

Failsafe: si la conexión MQTT se cae, el modo automático sigue funcionando localmente. No dependas solo del dashboard.

Testeo: simula inyecciones de tokens/lecturas para verificar la red de Petri (o usar herramientas como PIPE/WoPeD para simular).


11) Diagrama ASCII simplificado

[Measure] --T1--> (P1:CO_measured)
   |                     |
   v                     v
Check_high T2        Check_low T3
   |                     |
  (P2)                 (P3)
   |                     |
[T4 Auto_on]         [T5 Auto_off]   <-- inhibited if P5 (Manual_enabled)
   \                   /
    \                 /
     --> (P6) Fan_request_auto
            |
         [T9 Fan_activate] --(consumes Safety_lock)--> (P9 Fan_on) -> [T11 MQTT_send]

(además T6/T7 desde MQTT colocan tokens en P7/P8 y gestionan P5/P4)

