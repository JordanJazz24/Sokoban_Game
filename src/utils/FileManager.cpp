#include "FileManager.h"
#include "Constants.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <omp.h>


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

    // --- PASO 1: Leer el archivo UNA SOLA VEZ en un vector de strings ---
    std::vector<std::string> lines;
    std::string line;
    lines.reserve(100); // Reserva inicial para evitar realojamientos
    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }
    inputFile.close();

    // --- PASO 2: Calcular dimensiones desde la memoria (muy rápido) ---
    if (lines.empty()) {
        rows = 0;
        cols = 0;
        return nullptr;
    }
    rows = lines.size();
    
    size_t maxLen = 0;
    // Este bucle es lo suficientemente rápido como para no necesitar paralelización
    for (const auto& l : lines) {
        if (l.length() > maxLen) {
            maxLen = l.length();
        }
    }
    cols = static_cast<int>(maxLen);
    
    std::cout << "Tamaño de la matriz: " << rows << "x" << cols << std::endl;
    std::cout << "Hilos disponibles para OpenMP: " << omp_get_max_threads() << std::endl;

    // --- PASO 3: Asignar y llenar la matriz en un único bucle paralelo consolidado ---
    char** matrix = new char*[rows];

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < rows; ++i) {
        // Cada hilo asigna y llena su propia fila
        matrix[i] = new char[cols];
        const std::string& currentLine = lines[i];
        const int lineSize = currentLine.size();

        // Copiar contenido de la línea
        for (int j = 0; j < lineSize; ++j) {
            matrix[i][j] = currentLine[j];
        }

        // Rellenar el resto con espacios
        for (int j = lineSize; j < cols; ++j) {
            matrix[i][j] = ' ';
        }
    }

    return matrix;
}

void FileManager::freeMatrix(char** matrix, int rows) {
    if (matrix != nullptr) {
        // La versión secuencial es más eficiente para liberar memoria.
        // El overhead de paralelizar 'delete' es mayor que cualquier ganancia.
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
