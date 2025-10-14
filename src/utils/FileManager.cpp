#include "FileManager.h"
#include "Constants.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

// ============================================================================
// VERSIÓN SECUENCIAL (comentar para usar la versión paralela)
// ============================================================================
// #define USE_SEQUENTIAL_VERSION

#ifdef USE_SEQUENTIAL_VERSION

char** FileManager::loadLevel(int levelNumber, int& rows, int& cols) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
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

    // --- PASO 1: Leer el archivo ---
    std::vector<std::string> lines;
    std::string line;
    lines.reserve(150);
    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }
    inputFile.close();

    if (lines.empty()) {
        rows = 0;
        cols = 0;
        return nullptr;
    }
    rows = lines.size();
    
    // --- PASO 2: Calcular dimensiones ---
    size_t maxLen = 0;
    for (const auto& l : lines) {
        if (l.length() > maxLen) {
            maxLen = l.length();
        }
    }
    cols = static_cast<int>(maxLen);
    
    std::cout << "\n🔹 VERSIÓN SECUENCIAL" << std::endl;
    std::cout << "📊 Tamaño de la matriz: " << rows << "x" << cols << std::endl;

    // --- PASO 3: Procesar matriz con análisis y validación SECUENCIAL ---
    char** matrix = new char*[rows];
    
    // Contadores para estadísticas
    int playerCount = 0, boxCount = 0, goalCount = 0;
    int wallCount = 0, invalidCharCount = 0;
    
    for (int i = 0; i < rows; ++i) {
        matrix[i] = new char[cols];
        const std::string& currentLine = lines[i];
        const int lineSize = currentLine.size();
        
        // Procesar cada carácter con validación y análisis
        for (int j = 0; j < lineSize; ++j) {
            char c = currentLine[j];
            
            // 1. VALIDACIÓN de caracteres
            bool isValid = (c == '#' || c == ' ' || c == '$' || 
                           c == '.' || c == '@' || c == '&' || 
                           c == '+' || c == '*' || c == '!');
            
            if (!isValid) {
                invalidCharCount++;
                c = ' ';
            }
            
            // 2. ANÁLISIS de elementos del juego
            switch (c) {
                case '@': case '&': case '+':
                    playerCount++;
                    break;
                case '$': case '*':
                    boxCount++;
                    break;
                case '.':
                    goalCount++;
                    break;
                case '#':
                    wallCount++;
                    break;
            }
            
            // 3. NORMALIZACIÓN de caracteres especiales
            if (c == '+') c = '@';
            if (c == '*') c = '$';
            
            // 4. SIMULACIÓN de trabajo adicional (procesamiento por carácter)
            volatile int dummy = 0;
            for (int k = 0; k < 50; ++k) {
                dummy += (c * i * j + k) % 13;
            }
            
            matrix[i][j] = c;
        }
        
        // Rellenar espacios
        for (int j = lineSize; j < cols; ++j) {
            matrix[i][j] = ' ';
        }
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    
    /*
    // Reportar resultados
    std::cout << "\n📈 Análisis completado:" << std::endl;
    std::cout << "   👤 Jugadores: " << playerCount << std::endl;
    std::cout << "   📦 Cajas: " << boxCount << std::endl;
    std::cout << "   🎯 Objetivos: " << goalCount << std::endl;
    std::cout << "   🧱 Paredes: " << wallCount << std::endl;
    if (invalidCharCount > 0) {
        std::cout << "   ⚠️  Caracteres inválidos: " << invalidCharCount << std::endl;
    }
    std::cout << "\n⏱️  TIEMPO TOTAL (SECUENCIAL): " << duration.count() / 1000.0 
              << " ms\n" << std::endl;

              system("pause");
              */
    return matrix;
}

#else

// ============================================================================
// VERSIÓN PARALELA CON OpenMP (activada por defecto)
// ============================================================================

char** FileManager::loadLevel(int levelNumber, int& rows, int& cols) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
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

    // --- PASO 1: Leer el archivo ---
    std::vector<std::string> lines;
    std::string line;
    lines.reserve(150);
    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }
    inputFile.close();

    if (lines.empty()) {
        rows = 0;
        cols = 0;
        return nullptr;
    }
    rows = lines.size();
    
    // --- PASO 2: Calcular dimensiones ---
    size_t maxLen = 0;
    for (const auto& l : lines) {
        if (l.length() > maxLen) {
            maxLen = l.length();
        }
    }
    cols = static_cast<int>(maxLen);
    
    const int numThreads = omp_get_max_threads();
    std::cout << "\n🚀 VERSIÓN PARALELA (OpenMP)" << std::endl;
    std::cout << "📊 Tamaño de la matriz: " << rows << "x" << cols << std::endl;
    std::cout << "🔧 Hilos disponibles: " << numThreads << std::endl;

    // --- PASO 3: Procesar matriz con análisis y validación PARALELA ---
    char** matrix = new char*[rows];
    
    // Contadores compartidos (usamos reduction para evitar race conditions)
    int playerCount = 0, boxCount = 0, goalCount = 0;
    int wallCount = 0, invalidCharCount = 0;
    
    #pragma omp parallel for schedule(dynamic, 4) \
        reduction(+:playerCount, boxCount, goalCount, wallCount, invalidCharCount)
    for (int i = 0; i < rows; ++i) {
        matrix[i] = new char[cols];
        const std::string& currentLine = lines[i];
        const int lineSize = currentLine.size();
        
        // Procesar cada carácter con validación y análisis
        for (int j = 0; j < lineSize; ++j) {
            char c = currentLine[j];
            
            // 1. VALIDACIÓN de caracteres
            bool isValid = (c == '#' || c == ' ' || c == '$' || 
                           c == '.' || c == '@' || c == '&' || 
                           c == '+' || c == '*' || c == '!');
            
            if (!isValid) {
                invalidCharCount++;
                c = ' ';
            }
            
            // 2. ANÁLISIS de elementos del juego
            switch (c) {
                case '@': case '&': case '+':
                    playerCount++;
                    break;
                case '$': case '*':
                    boxCount++;
                    break;
                case '.':
                    goalCount++;
                    break;
                case '#':
                    wallCount++;
                    break;
            }
            
            // 3. NORMALIZACIÓN de caracteres especiales
            if (c == '+') c = '@';
            if (c == '*') c = '$';
            
            // 4. SIMULACIÓN de trabajo adicional (procesamiento por carácter)
            volatile int dummy = 0;
            for (int k = 0; k < 50; ++k) {
                dummy += (c * i * j + k) % 13;
            }
            
            matrix[i][j] = c;
        }
        
        // Rellenar espacios
        for (int j = lineSize; j < cols; ++j) {
            matrix[i][j] = ' ';
        }
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    /*    // Reportar resultados
    std::cout << "\n📈 Análisis completado:" << std::endl;
    std::cout << "   👤 Jugadores: " << playerCount << std::endl;
    std::cout << "   📦 Cajas: " << boxCount << std::endl;
    std::cout << "   🎯 Objetivos: " << goalCount << std::endl;
    std::cout << "   🧱 Paredes: " << wallCount << std::endl;
    if (invalidCharCount > 0) {
        std::cout << "   ⚠️  Caracteres inválidos: " << invalidCharCount << std::endl;
    }
    std::cout << "\n⏱️  TIEMPO TOTAL (PARALELO): " << duration.count() / 1000.0 
              << " ms\n" << std::endl;
              system("pause");
*/
    return matrix;
}

#endif

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
