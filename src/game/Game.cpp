#include "Game.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <iomanip>
#include <chrono>

Game::Game() : currentLevel(nullptr), currentLevelNumber(1) {
    // Configurar la consola para usar UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

Game::~Game() {
    delete currentLevel;
}

void Game::run() {
    while (true) {
        displayMainMenu();
        char choice = waitForKeyPress();
        
        if (!processMenuChoice(choice)) {
            break; // Salir del juego
        }
        
        if (currentLevel != nullptr) {
            clearScreen();
            runLevel();
            delete currentLevel;
            currentLevel = nullptr;
        }
    }
}

void Game::displayMainMenu() {
    clearScreen();
    std::cout << GameConstants::WELCOME_MESSAGE << std::endl;
    std::cout << "1. Nueva Partida" << std::endl;
    std::cout << "2. Cargar Partida" << std::endl;
    std::cout << "3. Salir" << std::endl;
    std::cout << "Elija una opción: ";
}

bool Game::processMenuChoice(char choice) {
    switch (choice) {
        case '1':
            startNewGame(GameConstants::DEFAULT_LEVEL);
            return true;
        case '2':
            loadGame();
            return true;
        case '3':
            return false; // Salir
        default:
            std::cout << "Opción no válida. Por favor, seleccione 1, 2 o 3." << std::endl;
            system("pause");
            return true;
    }
}

void Game::startNewGame(int levelNumber) {
    currentLevelNumber = levelNumber;
    delete currentLevel;
    currentLevel = new Level(currentLevelNumber);
    gameState.currentLevel = currentLevelNumber;
    
    // Mostrar pantalla de tiempo de carga solo para nueva partida
    showLoadTimeScreen();
}

void Game::loadGame() {
    std::vector<Movement> savedMovements;
    int savedLevel;
    
    if (FileManager::loadGame(savedLevel, savedMovements)) {
        startNewGame(savedLevel);
        if (currentLevel) {
            currentLevel->setMovements(savedMovements);
            std::cout << "Partida cargada exitosamente." << std::endl;
        }
    } else {
        std::cout << "No se encontró una partida guardada." << std::endl;
    }
    system("pause");
}

void Game::saveGame() {
    if (currentLevel) {
        bool success = FileManager::saveGame(
            currentLevelNumber, 
            currentLevel->getMovements()
        );
        
        if (success) {
            std::cout << "Partida guardada exitosamente." << std::endl;
        } else {
            std::cout << "Error al guardar la partida." << std::endl;
        }
        waitForKeyPress();
    }
}

void Game::runLevel() {
    while (currentLevel != nullptr) {
        // Iniciar medición del tiempo de refresh del display completo
        auto refreshStart = std::chrono::high_resolution_clock::now();
        
        currentLevel->display();
        std::cout << std::endl;
        
        // ═══════════════════════════════════════════════════════════════════
        // ESTADÍSTICAS DEL JUEGO
        // ═══════════════════════════════════════════════════════════════════
        std::cout << "+-----------------------------------------------------------+" << std::endl;
        std::cout << "|        ESTADISTICAS Y RENDIMIENTO DEL JUEGO               |" << std::endl;
        std::cout << "+-----------------------------------------------------------+" << std::endl;
        
        // Movimientos de jugadores
        int player1Moves = currentLevel->getPlayer1MoveCount();
        int player2Moves = currentLevel->getPlayer2MoveCount();
        std::cout << "\n[MOVIMIENTOS]" << std::endl;
        std::cout << "   * Jugador 1 (@): " << std::setw(4) << player1Moves << " movimientos" << std::endl;
        std::cout << "   * Jugador 2 (&): " << std::setw(4) << player2Moves << " movimientos" << std::endl;
        std::cout << std::endl;
        
        // ═══════════════════════════════════════════════════════════════════
        // TIEMPOS DE PROCESAMIENTO Y ANÁLISIS DE RENDIMIENTO
        // ═══════════════════════════════════════════════════════════════════
        std::cout << "[METRICAS DE RENDIMIENTO - Analisis de Paralelizacion]" << std::endl;
        std::cout << "+------------------------------------------------+--------------+----------+" << std::endl;
        std::cout << "| Metodo / Operacion                             | Tiempo (ms)  | % Frame  |" << std::endl;
        std::cout << "+------------------------------------------------+--------------+----------+" << std::endl;
        
        // Finalizar medición del tiempo de refresh primero para calcular porcentajes
        auto refreshEnd = std::chrono::high_resolution_clock::now();
        auto refreshDuration = std::chrono::duration_cast<std::chrono::microseconds>(refreshEnd - refreshStart);
        double refreshTime = refreshDuration.count() / 1000.0;
        currentLevel->setLastRefreshTime(refreshTime);
        
        // Obtener tiempos individuales
        double fileLoadTime = currentLevel->getLoadTime();
        double gridCreationTime = currentLevel->getGridCreationTime();
        double displayRenderTime = currentLevel->getDisplayRenderTime();
        double totalLoadTime = currentLevel->getTotalLoadTime();
        
        // Calcular porcentajes respecto al refresh completo
        auto calcPercent = [refreshTime](double time) -> double {
            return (refreshTime > 0) ? (time / refreshTime * 100.0) : 0.0;
        };
        
        // 1. FASE DE CARGA INICIAL (solo se ejecuta al inicio del nivel)
        std::cout << "| [CARGA INICIAL DEL NIVEL]                      |              |          |" << std::endl;
        std::cout << "|   +- FileManager::loadLevel()                  | " 
                  << std::fixed << std::setprecision(3) << std::setw(10) 
                  << fileLoadTime << " ms |   N/A    |" << std::endl;
        
        std::cout << "|   +- Grid::createGridStructure()               | " 
                  << std::fixed << std::setprecision(3) << std::setw(10) 
                  << gridCreationTime << " ms |   N/A    |" << std::endl;
        
        std::cout << "|   +- Total Carga Nivel                         | " 
                  << std::fixed << std::setprecision(3) << std::setw(10) 
                  << totalLoadTime << " ms |   N/A    |" << std::endl;
        
        std::cout << "+------------------------------------------------+--------------+----------+" << std::endl;
        
        // 2. FASE DE RENDERIZADO (se ejecuta en cada frame)
        std::cout << "| [RENDERIZADO POR FRAME]                        |              |          |" << std::endl;
        std::cout << "|   +- Grid::printGrid()                         | " 
                  << std::fixed << std::setprecision(3) << std::setw(10) 
                  << displayRenderTime << " ms | " 
                  << std::setw(6) << std::setprecision(2) << calcPercent(displayRenderTime) << " % |" << std::endl;
        
        double uiOverhead = refreshTime - displayRenderTime;
        std::cout << "|   +- Overhead UI/Stats                         | " 
                  << std::fixed << std::setprecision(3) << std::setw(10) 
                  << uiOverhead << " ms | " 
                  << std::setw(6) << std::setprecision(2) << calcPercent(uiOverhead) << " % |" << std::endl;
        
        std::cout << "|   +- Refresh Total                             | " 
                  << std::fixed << std::setprecision(3) << std::setw(10) 
                  << refreshTime << " ms | 100.00 % |" << std::endl;
        
        std::cout << "+------------------------------------------------+--------------+----------+" << std::endl;
        
        // 3. ANÁLISIS DE RENDIMIENTO
        double fps = (refreshTime > 0) ? (1000.0 / refreshTime) : 0.0;
        std::cout << "| [ANALISIS DE RENDIMIENTO]                      |              |          |" << std::endl;
        std::cout << "|   * FPS Estimado: " << std::setw(5) << std::setprecision(1) << fps << " frames/seg           |              |          |" << std::endl;
        
        if (displayRenderTime > 16.67) {
            std::cout << "|   [!] Renderizado > 16.67ms (bajo de 60 FPS)  |              |          |" << std::endl;
        } else {
            std::cout << "|   [OK] Rendimiento optimo para 60+ FPS        |              |          |" << std::endl;
        }
        
        std::cout << "+------------------------------------------------+--------------+----------+" << std::endl;
        std::cout << std::endl;
        
        // ═══════════════════════════════════════════════════════════════════
        // CONTROLES
        // ═══════════════════════════════════════════════════════════════════
        std::cout << "[CONTROLES] [WASD] Jugador 1 | [Flechas] Jugador 2" << std::endl;
        std::cout << "[OPCIONES]  [Q] Salir | [G] Guardar | [R] Reiniciar" << std::endl;
        
        // Esperar hasta que haya al menos una entrada
        while (!isKeyAvailable()) {
            Sleep(10);
        }
        
        if (!processMultiPlayerInput()) {
            break; // Salir del nivel
        }
        
        clearScreen();
        
        if (currentLevel && currentLevel->isCompleted()) {
            handleLevelCompletion();
            break;
        }
    }
}

bool Game::processGameInput(char input) {
    Movement movement;
    
    switch (input) {
        case 'Q':
        case 'q':
            return false; // Salir del nivel
            
        case 'G':
        case 'g':
            saveGame();
            return true;
            
        case 'R':
        case 'r':
            currentLevel->reset();
            std::cout << "Nivel reiniciado." << std::endl;
            return true;
            
        default:
            if (keyToMovementPlayer1(input, movement)) {
                if (!currentLevel->makeMove(PLAYER_1, movement)) {
                    std::cout << "Movimiento inválido para Jugador 1." << std::endl;
                    Sleep(500);
                }
            } else {
                std::cout << "Tecla no válida." << std::endl;
                Sleep(500);
            }
            return true;
    }
}

bool Game::processMultiPlayerInput() {
    bool player1Moved = false;
    bool player2Moved = false;
    
    // Procesar múltiples entradas en un frame
    while (isKeyAvailable()) {
        int key = getKeyPress();
        
        // Comandos especiales
        if (key == 'Q' || key == 'q') {
            return false; // Salir del nivel
        }
        if (key == 'G' || key == 'g') {
            saveGame();
            continue;
        }
        if (key == 'R' || key == 'r') {
            currentLevel->reset();
            std::cout << "Nivel reiniciado." << std::endl;
            Sleep(1000);
            return true;
        }
        
        Movement movement;
        
        // Verificar movimientos del Jugador 1 (WASD)
        if (keyToMovementPlayer1(key, movement) && !player1Moved) {
            if (currentLevel->makeMove(PLAYER_1, movement)) {
                player1Moved = true;
            }
        }
        // Verificar movimientos del Jugador 2 (flechas)
        else if (keyToMovementPlayer2(key, movement) && !player2Moved) {
            if (currentLevel->makeMove(PLAYER_2, movement)) {
                player2Moved = true;
            }
        }
        
        // Limpiar buffer adicional para permitir entrada fluida
        Sleep(10);
    }
    
    return true;
}

void Game::handleLevelCompletion() {
    currentLevel->display();
    showLevelWinScreen();
    
    if (currentLevelNumber >= GameConstants::MAX_LEVELS) {
        showGameCompleteScreen();
        char choice = waitForKeyPress();
        
        while (choice != 'R' && choice != 'r' && choice != 13) {
            std::cout << "Opción no válida. Presione R para repetición o ENTER para reiniciar: ";
            choice = waitForKeyPress();
        }
        
        if (choice == 'R' || choice == 'r') {
            currentLevel->replay();
        }
        
        // Reiniciar el juego completo - volver al menú
        currentLevelNumber = 1;
        delete currentLevel;
        currentLevel = nullptr;
    } else {
        std::cout << "Presione ENTER para continuar al siguiente nivel, Q para salir, o R para ver repetición: ";
        char choice = waitForKeyPress();
        
        while (choice != 'R' && choice != 'r' && choice != 'Q' && choice != 'q' && choice != 13) {
            std::cout << "Opción no válida. Presione R, Q, o ENTER: ";
            choice = waitForKeyPress();
        }
        
        if (choice == 'R' || choice == 'r') {
            currentLevel->replay();
            std::cout << "Presione ENTER para continuar o Q para salir: ";
            choice = waitForKeyPress();
        }
        
        if (choice == 'Q' || choice == 'q') {
            // Regresar al menú principal
            delete currentLevel;
            currentLevel = nullptr;
        } else {
            // Avanzar al siguiente nivel (ENTER u otra tecla)
            advanceToNextLevel();
            // Continuar el juego con el nuevo nivel
            clearScreen();
            runLevel();
        }
    }
}

bool Game::advanceToNextLevel() {
    if (currentLevelNumber < GameConstants::MAX_LEVELS) {
        currentLevelNumber++;
        startNewGame(currentLevelNumber);
        return true;
    }
    return false;
}

void Game::showLevelWinScreen() {
    std::cout << "*** " << GameConstants::WIN_MESSAGE << " " << currentLevelNumber << " ***" << std::endl;
    std::cout << "Movimientos realizados: " << currentLevel->getMoveCount() << std::endl;
}

void Game::showGameCompleteScreen() {
    std::cout << GameConstants::COMPLETE_MESSAGE << std::endl;
    std::cout << "Presione ENTER para reiniciar el juego o R para ver la repetición del último nivel: ";
}

void Game::showLoadTimeScreen() {
    if (currentLevel) {
        clearScreen();
        std::cout << "====================================" << std::endl;
        std::cout << "      INFORMACIÓN DE CARGA" << std::endl;
        std::cout << "====================================" << std::endl;
        std::cout << std::endl;
        std::cout << "Nivel: " << currentLevelNumber << std::endl;
        std::cout << "Tiempo de carga del archivo: " << std::fixed << std::setprecision(3) 
                  << currentLevel->getLoadTime() << " ms" << std::endl;
        std::cout << std::endl;
        std::cout << "CONTROLES:" << std::endl;
        std::cout << "- Jugador 1: W/A/S/D" << std::endl;
        std::cout << "- Jugador 2: Flechas" << std::endl;
        std::cout << std::endl;
        std::cout << "====================================" << std::endl;
        std::cout << std::endl;
        std::cout << "Presione cualquier tecla para continuar al nivel..." << std::endl;
        
        waitForKeyPress();
    }
}

bool Game::keyToMovementPlayer1(char key, Movement& movement) {
    switch (key) {
        case 'W':
        case 'w':
            movement = UP;
            return true;
        case 'S':
        case 's':
            movement = DOWN;
            return true;
        case 'A':
        case 'a':
            movement = LEFT;
            return true;
        case 'D':
        case 'd':
            movement = RIGHT;
            return true;
        default:
            return false;
    }
}

bool Game::keyToMovementPlayer2(int key, Movement& movement) {
    switch (key) {
        case 72: // Flecha arriba
            movement = UP;
            return true;
        case 80: // Flecha abajo
            movement = DOWN;
            return true;
        case 75: // Flecha izquierda
            movement = LEFT;
            return true;
        case 77: // Flecha derecha
            movement = RIGHT;
            return true;
        default:
            return false;
    }
}

void Game::clearScreen() {
    system("cls");
}

char Game::waitForKeyPress() {
    return _getch();
}

bool Game::isKeyAvailable() {
    return _kbhit();
}

int Game::getKeyPress() {
    if (_kbhit()) {
        int key = _getch();
        // Si es una tecla especial (como las flechas), necesitamos el segundo código
        if (key == 0 || key == 224) {
            if (_kbhit()) {
                return _getch();
            }
        }
        return key;
    }
    return 0;
}
