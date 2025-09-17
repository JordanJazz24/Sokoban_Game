#include "Level.h"
#include "../utils/Constants.h"
#include <iostream>
#include <Windows.h>
#include <chrono>

Level::Level(int levelNum) : grid(nullptr), levelNumber(levelNum), loadTimeMs(0.0) {
    loadLevel();
}

Level::~Level() {
    delete grid;
}

bool Level::loadLevel() {
    // Iniciar medición del tiempo
    auto startTime = std::chrono::high_resolution_clock::now();
    
    int rows, cols;
    char** levelMatrix = FileManager::loadLevel(levelNumber, rows, cols);
    

    
    if (levelMatrix == nullptr) {
        std::cerr << "Error: No se pudo cargar el nivel " << levelNumber << std::endl;
        return false;
    }
    
    // Crear nuevo grid o reemplazar el existente
    try {
        delete grid;
        grid = new Grid(levelMatrix, rows, cols);
    } catch (const std::bad_alloc& e) {
        std::cerr << "Error de memoria al crear el grid: " << e.what() << std::endl;
        FileManager::freeMatrix(levelMatrix, rows);
        return false;
    }
    
    // Liberar matriz temporal
    FileManager::freeMatrix(levelMatrix, rows);
    
    // Limpiar movimientos
    movements.clear();
    // Finalizar medición del tiempo de carga del archivo
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    loadTimeMs = duration.count() / 1000.0;
    return true;
}

bool Level::makeMove(Player player, Movement movement) {
    if (!grid) return false;
    
    bool success = grid->movePlayer(player, movement);
    if (success) {
        movements.push_back(movement);
    }
    
    return success;
}

void Level::reset() {
    loadLevel(); // Recargar el nivel desde archivo
}

bool Level::isCompleted() const {
    return grid ? grid->isLevelCompleted() : false;
}

void Level::display() const {
    if (grid) {
        grid->printGrid();
    }
}

void Level::replay() const {
    if (!grid) return;
    
    // Reiniciar nivel temporalmente
    Level tempLevel(levelNumber);
    
    std::cout << "=== Reproduciendo nivel " << levelNumber << " ===" << std::endl;
    tempLevel.display();
    Sleep(1000);
    
    // Reproducir cada movimiento (usar PLAYER_1 para compatibilidad)
    for (const Movement& move : movements) {
        system("cls");
        tempLevel.makeMove(PLAYER_1, move);
        tempLevel.display();
        Sleep(500); // Pausa entre movimientos
    }
    
    std::cout << "=== Fin de la reproducción ===" << std::endl;
}

int Level::getLevelNumber() const {
    return levelNumber;
}

int Level::getMoveCount() const {
    return movements.size();
}

const std::vector<Movement>& Level::getMovements() const {
    return movements;
}

void Level::setMovements(const std::vector<Movement>& newMovements) {
    movements.clear();
    
    // Reiniciar nivel
    reset();
    
    // Aplicar movimientos (usar PLAYER_1 para compatibilidad con partidas guardadas)
    for (const Movement& move : newMovements) {
        makeMove(PLAYER_1, move);
    }
}

double Level::getLoadTime() const {
    return loadTimeMs;
}

double Level::getPlayer1CreationTime() const {
    return grid ? grid->getPlayer1CreationTime() : 0.0;
}

double Level::getPlayer2CreationTime() const {
    return grid ? grid->getPlayer2CreationTime() : 0.0;
}

double Level::getPlayer1LastMoveTime() const {
    return grid ? grid->getPlayer1LastMoveTime() : 0.0;
}

double Level::getPlayer2LastMoveTime() const {
    return grid ? grid->getPlayer2LastMoveTime() : 0.0;
}

double Level::getPlayer1TotalMoveTime() const {
    return grid ? grid->getPlayer1TotalMoveTime() : 0.0;
}

double Level::getPlayer2TotalMoveTime() const {
    return grid ? grid->getPlayer2TotalMoveTime() : 0.0;
}

int Level::getPlayer1MoveCount() const {
    return grid ? grid->getPlayer1MoveCount() : 0;
}

int Level::getPlayer2MoveCount() const {
    return grid ? grid->getPlayer2MoveCount() : 0;
}

double Level::getLastRefreshTime() const {
    return grid ? grid->getLastRefreshTime() : 0.0;
}

double Level::getLastValidationTime() const {
    return grid ? grid->getLastValidationTime() : 0.0;
}

void Level::setLastRefreshTime(double time) {
    if (grid) {
        grid->setLastRefreshTime(time);
    }
}

void Level::setLastValidationTime(double time) {
    if (grid) {
        grid->setLastValidationTime(time);
    }
}
