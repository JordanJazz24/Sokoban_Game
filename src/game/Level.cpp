#include "Level.h"
#include "../utils/Constants.h"
#include <iostream>
#include <Windows.h>

Level::Level(int levelNum) : grid(nullptr), levelNumber(levelNum) {
    loadLevel();
}

Level::~Level() {
    delete grid;
}

bool Level::loadLevel() {
    int rows, cols;
    char** levelMatrix = FileManager::loadLevel(levelNumber, rows, cols);
    
    if (levelMatrix == nullptr) {
        std::cerr << "Error: No se pudo cargar el nivel " << levelNumber << std::endl;
        return false;
    }
    
    // Crear nuevo grid o reemplazar el existente
    delete grid;
    grid = new Grid(levelMatrix, rows, cols);
    
    // Liberar matriz temporal
    FileManager::freeMatrix(levelMatrix, rows);
    
    // Limpiar movimientos
    movements.clear();
    
    return true;
}

bool Level::makeMove(Movement movement) {
    if (!grid) return false;
    
    bool success = grid->movePlayer(movement);
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
    
    // Reproducir cada movimiento
    for (const Movement& move : movements) {
        system("cls");
        tempLevel.makeMove(move);
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
    
    // Aplicar movimientos
    for (const Movement& move : newMovements) {
        makeMove(move);
    }
}
