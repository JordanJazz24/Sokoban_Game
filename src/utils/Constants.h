#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

/**
 * @file Constants.h
 * @brief Constantes del juego Sokoban
 */

namespace GameConstants {
    // Configuración de niveles
    const int MAX_LEVELS = 3;
    const int DEFAULT_LEVEL = 1;
    
    // Rutas de archivos
    const std::string LEVELS_PATH = "assets/levels/";
    const std::string SAVES_PATH = "assets/saves/";
    const std::string SAVE_FILE = "partida.txt";
    
    // Mensajes del juego
    const std::string WELCOME_MESSAGE = "Bienvenido a Sokoban MIEDO!!";
    const std::string WIN_MESSAGE = "¡Felicidades! Has ganado el nivel";
    const std::string COMPLETE_MESSAGE = "¡Completaste todos los niveles, sos el puto amo lml !";
    
    // Controles
    const std::string CONTROLS_MESSAGE = 
        "Presione una tecla (W: Arriba, S: Abajo, A: Izquierda, D: Derecha, Q: Salir, R: Repetir, G: Guardar partida):";
}

#endif // CONSTANTS_H
