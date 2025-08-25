#include "FileManager.h"
#include "Constants.h"
#include <fstream>
#include <iostream>
#include <vector>

char** FileManager::loadLevel(int levelNumber, int& rows, int& cols) {
    std::string filename = getLevelFileName(levelNumber);
    std::string fullPath = findFile(filename);
    
    if (fullPath.empty()) {
        std::cerr << "Error: No se pudo encontrar el archivo " << filename << std::endl;
        return nullptr;
    }
    
    std::ifstream inputFile(fullPath);
    if (!inputFile) {
        std::cerr << "Error: No se pudo abrir el archivo " << fullPath << std::endl;
        return nullptr;
    }
    
    // Contar filas y encontrar la columna máxima
    rows = 0;
    int maxCols = 0;
    int currentCols = 0;
    char ch;
    
    while (inputFile.get(ch)) {
        if (ch != '\n') {
            currentCols++;
        } else {
            rows++;
            if (currentCols > maxCols) {
                maxCols = currentCols;
            }
            currentCols = 0;
        }
    }
    cols = maxCols;
    
    std::cout << "Tamaño de la matriz: " << rows << "x" << cols << std::endl;
    
    // Volver al principio del archivo
    inputFile.clear();
    inputFile.seekg(0, std::ios::beg);
    
    // Crear matriz
    char** matrix = new char*[rows];
    for (int i = 0; i < rows; i++) {
        matrix[i] = new char[cols];
        // Inicializar con espacios
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = ' ';
        }
    }
    
    // Llenar matriz
    int row = 0, col = 0;
    while (inputFile.get(ch) && row < rows) {
        if (ch != '\n') {
            if (col < cols) {
                matrix[row][col++] = ch;
            }
        } else {
            row++;
            col = 0;
        }
    }
    
    inputFile.close();
    return matrix;
}

void FileManager::freeMatrix(char** matrix, int rows) {
    if (matrix != nullptr) {
        for (int i = 0; i < rows; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;
    }
}

bool FileManager::saveGame(int currentLevel, const std::vector<Movement>& movements) {
    std::string savePath = findFile(GameConstants::SAVE_FILE);
    if (savePath.empty()) {
        savePath = GameConstants::SAVES_PATH + GameConstants::SAVE_FILE;
    }
    
    std::ofstream saveFile(savePath, std::ios::trunc);
    if (!saveFile.is_open()) {
        std::cerr << "Error: No se pudo crear el archivo de guardado en " << savePath << std::endl;
        return false;
    }
    
    // Guardar nivel actual
    saveFile << currentLevel << std::endl;
    
    // Guardar movimientos
    for (const Movement& movement : movements) {
        saveFile << static_cast<int>(movement) << " ";
    }
    
    saveFile.close();
    std::cout << "Partida guardada exitosamente en " << savePath << std::endl;
    return true;
}

bool FileManager::loadGame(int& currentLevel, std::vector<Movement>& movements) {
    std::string savePath = findFile(GameConstants::SAVE_FILE);
    
    if (savePath.empty()) {
        std::cout << "No se encontró una partida guardada." << std::endl;
        return false;
    }
    
    std::ifstream saveFile(savePath);
    if (!saveFile.is_open()) {
        std::cout << "No se pudo abrir el archivo de partida guardada." << std::endl;
        return false;
    }
    
    // Leer nivel actual
    saveFile >> currentLevel;
    
    // Limpiar vector de movimientos
    movements.clear();
    
    // Leer movimientos
    int movement;
    while (saveFile >> movement) {
        movements.push_back(static_cast<Movement>(movement));
    }
    
    saveFile.close();
    std::cout << "Partida cargada exitosamente." << std::endl;
    return true;
}

std::string FileManager::findFile(const std::string& filename) {
    std::vector<std::string> possiblePaths = {
        filename,                                    // Directorio actual
        "../" + filename,                           // Directorio padre
        "../../" + filename,                        // Dos niveles arriba
        GameConstants::LEVELS_PATH + filename,      // Carpeta de niveles
        "../" + GameConstants::LEVELS_PATH + filename,
        GameConstants::SAVES_PATH + filename,       // Carpeta de guardados
        "../" + GameConstants::SAVES_PATH + filename
    };
    
    for (const auto& path : possiblePaths) {
        std::ifstream testFile(path);
        if (testFile.is_open()) {
            testFile.close();
            return path;
        }
    }
    
    return "";  // No encontrado
}

std::string FileManager::getLevelFileName(int levelNumber) {
    return "level_" + std::string(levelNumber < 10 ? "0" : "") + std::to_string(levelNumber) + ".txt";
}
