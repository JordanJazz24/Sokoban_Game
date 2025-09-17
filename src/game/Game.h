#ifndef GAME_H
#define GAME_H

#include "Level.h"
#include "../utils/Types.h"
#include "../utils/FileManager.h"
#include "../utils/Constants.h"

/**
 * @class Game
 * @brief Clase principal que coordina todo el juego
 * 
 * Principios aplicados:
 * - Responsabilidad Única: Coordina el flujo general del juego
 * - Abierto/Cerrado: Extensible para nuevas funcionalidades
 * - Principio de inversión de dependencias: Depende de abstracciones
 */
class Game {
private:
    Level* currentLevel;        ///< Nivel actual
    int currentLevelNumber;     ///< Número del nivel actual
    GameState gameState;        ///< Estado del juego
    
public:
    /**
     * @brief Constructor del juego
     */
    Game();
    
    /**
     * @brief Destructor del juego
     */
    ~Game();
    
    /**
     * @brief Inicia el bucle principal del juego
     */
    void run();
    
private:
    /**
     * @brief Muestra el menú principal
     */
    void displayMainMenu();
    
    /**
     * @brief Procesa la entrada del menú principal
     * @param choice Opción seleccionada
     * @return true para continuar, false para salir
     */
    bool processMenuChoice(char choice);
    
    /**
     * @brief Inicia una nueva partida
     * @param levelNumber Número del nivel a iniciar
     */
    void startNewGame(int levelNumber = 1);
    
    /**
     * @brief Carga una partida guardada
     */
    void loadGame();
    
    /**
     * @brief Guarda la partida actualmente
     */
    void saveGame();
    
    /**
     * @brief Ejecuta el bucle principal de un nivel
     */
    void runLevel();
    
    /**
     * @brief Muestra la pantalla de tiempo de carga del nivel
     */
    void showLoadTimeScreen();
    
    /**
     * @brief Procesa la entrada durante el juego
     * @param input Tecla presionada
     * @return true para continuar el nivel, false para salir
     */
    bool processGameInput(char input);
    
    /**
     * @brief Maneja la finalización de un nivel
     */
    void handleLevelCompletion();
    
    /**
     * @brief Avanza al siguiente nivel
     * @return true si hay siguiente nivel, false si se completaron todos
     */
    bool advanceToNextLevel();
    
    /**
     * @brief Muestra la pantalla de victoria del nivel
     */
    void showLevelWinScreen();
    
    /**
     * @brief Muestra la pantalla de victoria del juego completo
     */
    void showGameCompleteScreen();
    
    /**
     * @brief Convierte entrada de teclado a movimiento para el jugador 1 (WASD)
     * @param key Tecla presionada
     * @param movement Referencia donde almacenar el movimiento
     * @return true si es un movimiento válido, false en caso contrario
     */
    bool keyToMovementPlayer1(char key, Movement& movement);
    
    /**
     * @brief Convierte entrada de teclado a movimiento para el jugador 2 (flechas)
     * @param key Código de tecla presionada
     * @param movement Referencia donde almacenar el movimiento
     * @return true si es un movimiento válido, false en caso contrario
     */
    bool keyToMovementPlayer2(int key, Movement& movement);
    
    /**
     * @brief Procesa entradas de ambos jugadores simultáneamente
     * @return true para continuar el nivel, false para salir
     */
    bool processMultiPlayerInput();
    
    /**
     * @brief Limpia la pantalla
     */
    void clearScreen();
    
    /**
     * @brief Espera a que el usuario presione una tecla
     * @return Tecla presionada
     */
    char waitForKeyPress();
    
    /**
     * @brief Verifica si hay una tecla disponible sin bloquear
     * @return true si hay una tecla disponible, false en caso contrario
     */
    bool isKeyAvailable();
    
    /**
     * @brief Obtiene la tecla presionada sin bloquear
     * @return Código de la tecla presionada o 0 si no hay ninguna
     */
    int getKeyPress();
};

#endif // GAME_H
