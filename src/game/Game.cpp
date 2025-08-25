#include "Game.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>

Game::Game() : currentLevel(nullptr), currentLevelNumber(1) {
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
        currentLevel->display();
        std::cout << std::endl;
        std::cout << GameConstants::CONTROLS_MESSAGE << std::endl;
        
        char input = waitForKeyPress();
        
        if (!processGameInput(input)) {
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
            if (keyToMovement(input, movement)) {
                if (!currentLevel->makeMove(movement)) {
                    std::cout << "Movimiento inválido." << std::endl;
                    Sleep(500);
                }
            } else {
                std::cout << "Tecla no válida." << std::endl;
                Sleep(500);
            }
            return true;
    }
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

bool Game::keyToMovement(char key, Movement& movement) {
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

void Game::clearScreen() {
    system("cls");
}

char Game::waitForKeyPress() {
    return _getch();
}
