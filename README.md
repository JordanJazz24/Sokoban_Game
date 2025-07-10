# Sokoban_Game

**Autor:** [JordanJazz24](https://github.com/JordanJazz24)  
**Repositorio:** [Sokoban_Game](https://github.com/JordanJazz24/Sokoban_Game)  
**Lenguaje Principal:** C++  
**Estado:** En desarrollo

---

## Descripción

Sokoban_Game es una implementación en C++ del clásico juego de lógica Sokoban. El objetivo del juego es mover todas las cajas a sus posiciones objetivo utilizando la menor cantidad de movimientos posible, en un entorno de consola. La arquitectura del proyecto está diseñada para ser escalable y eficiente, utilizando estructuras de datos enlazadas para la representación de los niveles y la lógica del juego.

---

## Características principales

- **Lectura dinámica de niveles:**  
  Permite cargar distintos niveles desde archivos de texto externos (por ejemplo, `Nivel 1.txt`, `Nivel 2.txt`), facilitando la expansión y personalización de desafíos.
- **Implementación modular:**  
  Separación clara entre lógica del juego (`GameLogic`), estructuras de datos (`Data`), y entrada/salida.
- **Gestión eficiente de memoria:**  
  Uso de listas enlazadas y punteros inteligentes para representar la grilla del juego y los elementos interactivos.
- **Lógica de movimientos y validaciones:**  
  Métodos robustos para validar y ejecutar movimientos del jugador y las cajas, incluyendo detección de colisiones y condiciones de victoria.
- **Soporte para reinicio de nivel y múltiples niveles:**  
  Función para reiniciar el nivel actual y avanzar a nuevos desafíos fácilmente.
- **Diseño extensible:**  
  Preparado para añadir nuevas funcionalidades, como almacenamiento de récords, más niveles o interfaz gráfica.

---

## Estructura del proyecto

```
Sokoban_Game/
├── main.cpp
├── Data/
│   ├── List.cpp     # Implementación de la grilla y la lógica de movimiento
│   ├── List.h
│   └── Node.h       # Definición de nodos enlazados
├── GameLogic/
│   └── GameMenu.h   # Lógica de menú y gestión de partidas
├── Nivel 1.txt      # Ejemplo de nivel jugable
├── Nivel 2.txt      # Otro nivel jugable
└── CMakeLists.txt   # Archivo de configuración CMake
```

---

## Instrucciones de uso

1. **Compilación del proyecto:**
   - Asegúrate de tener CMake y un compilador de C++ compatible (C++17 o superior).
   - Clona el repositorio y ejecuta:
     ```bash
     mkdir build
     cd build
     cmake ..
     make
     ```

2. **Ubicación de archivos de nivel:**
   - Mueve los archivos de nivel (`Nivel 1.txt`, `Nivel 2.txt`, etc.) dentro de la carpeta de salida, por ejemplo:  
     `...\Sokoban_Game\cmake-build-debug`
   - Esto es necesario para que el juego lea los niveles correctamente.

3. **Ejecución:**
   - Ejecuta el binario generado, normalmente:
     ```bash
     ./Sokoban_Game
     ```

---

## Ejemplo de formato de nivel

Un nivel es un archivo de texto donde:

- `#` representa una pared
- `@` representa al jugador
- `$` representa una caja
- `.` representa un objetivo
- Espacio en blanco es una celda vacía

Ejemplo (`Nivel 1.txt`):
```
######################
#          #        ##
#  $   .    #  $     #
#     $.    #       ##
####   @         #####
   ######   . . #
        ########
```

---

## Extensión y personalización

Puedes crear tus propios niveles siguiendo el formato anterior y agregándolos como `Nivel N.txt`.  
Para añadir nuevas mecánicas o interfaz gráfica, expande la lógica en `GameLogic/GameMenu.h` y las estructuras en `Data/List.h`.

---

## Contacto

Para sugerencias o soporte, contacta a [JordanJazz24](https://github.com/JordanJazz24).

---

**Este proyecto demuestra calidad en el manejo de memoria, estructuras de datos y lógica de juegos en C++, siendo una excelente base para experimentación y aprendizaje avanzado.**
