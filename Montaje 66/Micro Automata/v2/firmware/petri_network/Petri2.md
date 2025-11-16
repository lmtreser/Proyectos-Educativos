📘 Caso didáctico: Sensor de CO con ventilador automático (sin MQTT ni dashboard)

🔍 Objetivo

Medir la concentración de monóxido (CO).

Si supera cierto umbral, activar el ventilador automáticamente.

Apagarlo cuando baje.

Evitar que se activen y apaguen muy rápido (histeresis, control estable).

Enseñar el modelo mental de concurrencia y control con Redes de Petri.



---

🧱 Elementos reducidos

Plazas:

Plaza	Significado

P0: Idle	Sistema listo, esperando medición
P1: CO_measured	Lectura disponible del sensor
P2: CO_high	CO por encima del umbral alto
P3: CO_normal	CO por debajo del umbral bajo
P4: Fan_on	Ventilador encendido
P5: Fan_off	Ventilador apagado
P6: Safety_lock	Mutex para garantizar exclusividad


Transiciones:

Transición	Significado

T1: Measure	Lee sensor y deja token en P1
T2: Check_high	Detecta CO > umbral alto y pone token en P2
T3: Check_low	Detecta CO < umbral bajo y pone token en P3
T4: Fan_start	Activa el ventilador (si P2 y Safety_lock)
T5: Fan_stop	Apaga ventilador (si P3 y Safety_lock)


Marcado inicial sugerido:

P6 = 1 (lock disponible)
P5 = 1 (ventilador inicialmente apagado)
P0 = 1 (sistema listo)


---

🔄 Flujo ejemplo (gráfico simplificado ASCII)

┌──T2──> (P2:CO_high) ──T4─> (P4:Fan_on)
(P1:Medición)──┤
                 └──T3──> (P3:CO_normal) ──T5─> (P5:Fan_off)

(P6:Safety) bloquea tanto T4 como T5 alternativamente.


---

🧠 Teoría complementaria importante

1️⃣ Redes de Petri básicas

Representan procesos concurrentes.

Plazas describen estados; transiciones, cambios.

Tokens son los que fluyen y representan el recurso o estado habilitado.

Las condiciones que habilitan una transición son la presencia de tokens necesarios.


2️⃣ Siendo rigurosos matemáticamente

Una Red de Petri es un grafo bipartito dirigido, con función de peso en los arcos.

Puede representarse formalmente como un quinteto:
N = (P, T, F, W, M0)
Donde:

P: conjunto de plazas

T: conjunto de transiciones

F: arcos entre lugares/transiciones

W: pesos (opcional, para recursos múltiples)

M0: marcado inicial



3️⃣ Propiedades que podés enseñar:

Alcanzabilidad: ¿desde el estado inicial puede llegarse a Fan_on?

Vivasidad (Liveness): ninguna transición debe quedar bloqueada permanentemente.

Ausencia de deadlock: el sistema nunca debe quedar “congelado”.

Seguridad: no deben activarse transiciones conflictivas simultáneamente (acá lo controla el Safety_lock).



---

🛠️ ¿Cómo armar la Red de Petri en la práctica?

🧩 1. Dibujándola

Podés hacerlo con lápiz y papel (genial para estudiantes).

Usá símbolos:

○ = plaza

▭ = transición

· = token



Ejemplo dibujado:

[P6]●─╴> T4 ─╴> [P4]
         ▲
   [P2]●─┘

   [P6]●─╴> T5 ─╴> [P5]
         ▲
   [P3]●─┘

💻 2. Usando herramientas digitales

PIPE (Platform Independent Petri net Editor) → https://sourceforge.net/projects/pipe2/

WoPeD (Workflow Petri Net Designer) → https://woped.dhbw-karlsruhe.de

Tina (TIme petri Net Analyzer) → más avanzada (https://projects.laas.fr/tina/)

Modelá y simulá el sistema: podés colocar tokens, disparar manualmente transiciones, ver qué ramas son posibles, detectar errores.



---

📦 Relación con código embebido

Aunque las Redes de Petri no se programan "directamente" en código tradicional, su estructura se puede mapear perfectamente a:

Máquinas de estados (FSM) con condiciones (una transición es un if).

Tareas concurrentes con semáforos (RTOS, como viste en el ejemplo anterior).

Modelo productor-consumidor con colas (FreeRTOS queue notifications).


Podés usar la Red de Petri como documentación y modelo mental, y luego convertirla a funciones/tareas.


---

🧠 Tips para la enseñanza:

Empezá con ejemplos cotidianos (semáforo, control de acceso).

Luego pasá a embebidos con bajo nivel de concurrencia.

Mostrá cómo usar herramientas como PIPE para simular.

Cerrá el círculo enseñando cómo mapearlo a código real (Arduino, ESP32, PLC).
