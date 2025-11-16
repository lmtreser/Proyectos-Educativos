🔄 FSM: Moore vs Mealy

Una Máquina de Estados Finitos (FSM) es un modelo de control secuencial que representa el comportamiento de un sistema reactivo mediante estados, eventos o señales de entrada, transiciones entre estados, y acciones o salidas.

🔹 FSM de tipo Moore

📌 Características

Las salidas dependen sólo del estado actual.

Las acciones asociadas a las salidas se determinan al entrar en un estado.

Más sencillo de implementar y entender (por eso es común en hardware y enseñanza).

No responde de forma inmediata a las entradas: primero pasa a otro estado y después cambia la salida.


📊 Ejemplo aplicado

Estados posibles:

NORMAL → Estado por defecto, sin peligro

ALERTA → Monóxido alto, ventilador encendido

SAFE → Recuperado, ventilador apagándose


Transiciones:

Entrar en ALERTA si nivelCO > umbral

Volver a NORMAL si nivelCO <= umbral - histéresis


Salidas:

Ventilador = ON sólo en estado ALERTA

LED rojo = ON sólo en estado ALERTA



---

🔹 FSM de tipo Mealy

📌 Características

Las salidas dependen tanto del estado actual como de las entradas.

Pueden reaccionar inmediatamente a una entrada sin cambiar de estado.

Útil en sistemas que requieren respuesta rápida.

Puede tener menos estados que la FSM de Moore equivalente.


📊 ¿Cómo complejizar nuestro ejemplo?

Podemos agregar una acción condicionada directamente por la entrada en un estado dado. Por ejemplo, estando en NORMAL, si el sensor muestra una lectura crítica, encender el ventilador inmediatamente incluso antes de cambiar de estado:

Estado: NORMAL

Entrada: nivelCO > umbral

Acción (Mealy) → Activar ventilador inmediatamente.

Luego, se transiciona al estado ALERTA para mantener el ventilador encendido.


---

📘 Comparativa rápida

Característica	Moore	Mealy

Salida depende de	Estado actual	Estado + Entradas
Respuesta a cambios	No inmediata	Puede ser inmediata
Cantidad de estados	Más	Menos
Implementación	Más simple, predecible	Más compacta, pero más compleja
Ideal para	Circuitos secuenciales clásicos	Protocolos, codificadores, respuestas rápidas



---

🛠️ Herramientas para tus alumnos

🔧 Para diseñar FSM:

Draw.io (diagramas simples)

Miro o Lucidchart (colaborativos)

Tina-Cloud FSM Editor (herramienta online)

Graphviz + DOT lenguaje


💻 Para implementar FSM:

Arduino: código estructurado con switch-case

Python o C: usar librerías de máquinas de estado (transitions en Python)

Node-RED: utilizar nodos de lógica de estado e inyectar condiciones



---

📑 Tips teóricos para enseñanza

Usar colores: Estados en verde, transiciones en flechas negras, acciones en rojo.

Dividir en 4 pasos: definir entradas, salidas, estados, transiciones.

Enfatizar el carácter secuencial: no es un programa que se ejecuta linealmente.

Relacionar con la vida real: molinetes, semáforos, sistemas de alerta.

Plantear una escalabilidad: pregúntales cómo agregar un buzzer o una notificación.



---

📈 Ejemplo FSM Mealy (simplificado en texto)

Estado: NORMAL
Entrada: CO < umbral       -> Salida: Ventilador OFF
Entrada: CO >= umbral      -> Salida: Ventilador ON (transición a ALERTA)

Estado: ALERTA
Entrada: CO < umbral-10    -> Salida: Ventilador ON (transición a NORMAL)
Entrada: CO >= umbral      -> Salida: Ventilador ON

Acá la acción de encender el ventilador en NORMAL depende de la entrada (valor del sensor), típica de Mealy.
