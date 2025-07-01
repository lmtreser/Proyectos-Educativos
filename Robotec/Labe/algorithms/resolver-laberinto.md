# Cómo resolver un laberinto

Estos son algunos de los algoritmos para diseñar un robot resuelve laberintos:

- **Seguidor de paredes.** Se trata de un algoritmo para resolver laberintos simples. Consiste en seguir paralelo a una pared y si se llega a una intersección girar siempre hacia el mismo lado, izquierda o derecha, pero siempre hacia el mismo. Lógicamente puede no encontrarse el camino más corto. Este método no funcionará cuando el objetivo del laberinto está en el centro y hay un circuito cerrado rodeándolo, ya que es muy probable que vuelva al punto de origen sin llegar al centro.

- **Algoritmo de garantía.** Es una versión modificada del seguidor de paredes pero que es capaz de saltar entre islas resolviendo laberintos que el seguidor de paredes no puede resolver. Garantiza la resolución desde cualquier comienzo desde el perímetro hasta el objetivo en el centro, pero no es capaz de hacerlo al revés. No necesita marcar o recordar los lugares por los que ha pasado, pero si contar los giros que hace a derecha o izquierda, de manera que pueda saber que se encuentra en una isla y pueda saltar a otra zona en la dirección elegida como preferente.

- **Cadena.** Trata al laberinto como muchos pequeños laberintos encadenados y los resuelve en secuencia. En base utiliza el algoritmo de garantía y por lo tanto el seguidor de paredes. Aunque puede no encontrar el camino más corto si que trata de acortarlo ya que compara caminos alternativos y analiza si ya se ha pasado por un punto o no.
Vuelta atrás recursiva. Se trata de que el robot resuelve laberintos debe mapear en memoria las zonas por las que va pasando. Puede explorar en las cuatro direcciones con la condición de que si ya ha pasado por un camino evita pasar de nuevo. Siempre encuentra la solución aunque no sea la más corta.

- **Algoritmo de Trémaux.** Planteado por el ingeniero francés Charles Trémaux para resolver por personas en el interior de un laberinto. Es muy similar al método de la vuelta atrás recursiva y hay que seguir las siguientes normas: no seguir el mismo camino dos veces, tomar cualquier camino si se llega a una intersección nueva, si un camino nuevo lleva a una intersección conocida o a un callejón sin salida volver al inicio del camino y por último, si un camino viejo lleva a una intersección vieja seguir un camino nuevo, y si no lo hay seguir cualquiera. Siempre resuelve el laberinto y si se deja al robot en el interior de un laberinto siempre encuentra la salida.

## Recursos útiles

- [Cómo construir un robot micromouse que resuelve un laberinto](https://lab.bricogeek.com/tutorial/como-construir-un-robot-micromouse-que-resuelve-un-laberinto/como-resolver-un-laberinto)
- [Micromouse from scratch| Algorithm- Maze traversal|Shortest path|Floodfill](https://medium.com/@minikiraniamayadharmasiri/micromouse-from-scratch-algorithm-maze-traversal-shortest-path-floodfill-741242e8510)
- [Micromouse simulator](https://github.com/mackorone/mms)
- [Write a Micromouse maze-solving algorithm with Arduino](https://github.com/mackorone/mms-arduino)
