# Sugerencias

El experto Jose Luis Bruni de [Robótica Educativa](https://www.roboticaeducativa.com/) nos sugiere lo siguiente:

## Laberinto

- La forma del robot debería tender a ser "redonda", con dos motores con sus ruedas en el centro físico y dos ruedas "locas", una en cada extremo (facilitando el giro en su eje).

- Los laterales deberían poder deslizarse sin que afecte el roce en una pared. Se podrían utilizar rueditas.

- Los motores podrían llevar un encoder. Otra posibilidad es que al final de cada recta, que apoyen contra esa pared para quedar alineados, asegurando un giro a 90 ó 180 grados.

- Utilizar tres sensores ultrasónicos o infrarrojos.

- Tips de programación: ante una salida lateral, ¿qué hace? ¿Sigue o dobla? Definir si cuenta las curvas para volver sobre sus pasos y tomar esa salida lateral.

- Si hace el recorrido pegado a un lateral, tardará más, pero saldrá del laberinto. Elegir: ¿rapidez o seguridad?

## Seguidor de línea

- Usar dos o tres sensores infrarrojos.

- Velocidad: elegir rapidez o seguridad.

- Nivel de luz: autodetectar diferencias (blanco/negro) y guardar el valor medio (izquierdo + derecho / 2).

## Sumo

- El inicio deberá ser mediante un switch presionado, al liberar, contar 5 segundos y arrancar.

- Referente a a forma: frente inclinado, con borde flexible que roce el piso.

- Motores/Ruedas: la adherencia de las ruedas es primordial. Mucha fuerza y que no patinen, no sirve. Que sea ágil para girar sin perder contacto con el contrincante.

- Sensores: ideal de 3 sensores ultrasónicos cubriendo un abanico cercano a los 180 grados (ver el lóbulo de cada sensor).

- Sensor detector de borde: a mi criterio no es necesario.

- Posibles estrategias: hacerlo muy bajito, dificultando la detección por parte del contrincante (¿bordes redondeados?). Duplicar la tensión de los motores en ataque, luego de haber detectado (¿mantenerlo así?). No perder el contacto con el oponente.
