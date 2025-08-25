/**
 * @file main.cpp
 * @brief Punto de entrada principal del juego Sokoban
 * @author JordanJazz24
 * @date 2025
 */

#include "game/Game.h"
#include <iostream>

/**
 * @brief Función principal del programa
 * @return Código de salida del programa
 */
int main() {
    try {
        Game sokobanGame;
        sokobanGame.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error inesperado: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Error inesperado desconocido." << std::endl;
        return 1;
    }
    
    return 0;
}
