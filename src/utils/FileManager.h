#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include "Types.h"

/**
 * @class FileManager
 * @brief Clase responsable del manejo de archivos (carga de niveles, guardado/carga de partidas)
 * 
 * Principio de Responsabilidad Única: Se encarga únicamente del I/O de archivos
 */
class FileManager {
public:
    /**
     * @brief Carga un nivel desde archivo
     * @param levelNumber Número del nivel a cargar
     * @param rows Referencia donde se almacenará el número de filas
     * @param cols Referencia donde se almacenará el número de columnas
     * @return Matriz de caracteres representando el nivel, nullptr si hay error
     */
    static char** loadLevel(int levelNumber, int& rows, int& cols);
    
    /**
     * @brief Carga un nivel desde archivo con medición de tiempo
     * @param levelNumber Número del nivel a cargar
     * @param rows Referencia donde se almacenará el número de filas
     * @param cols Referencia donde se almacenará el número de columnas
     * @param loadTimeMs Referencia donde se almacenará el tiempo de carga en ms
     * @return Matriz de caracteres representando el nivel, nullptr si hay error
     */
    static char** loadLevel(int levelNumber, int& rows, int& cols, double& loadTimeMs);
    
    /**
     * @brief Libera la memoria de una matriz de nivel
     * @param matrix Matriz a liberar
     * @param rows Número de filas de la matriz
     */
    static void freeMatrix(char** matrix, int rows);
    
    /**
     * @brief Guarda el estado actual del juego
     * @param currentLevel Nivel actual
     * @param movements Vector de movimientos realizados
     * @return true si se guardó exitosamente, false en caso contrario
     */
    static bool saveGame(int currentLevel, const std::vector<Movement>& movements);
    
    /**
     * @brief Carga un juego guardado
     * @param currentLevel Referencia donde se almacenará el nivel cargado
     * @param movements Referencia donde se almacenarán los movimientos
     * @return true si se cargó exitosamente, false en caso contrario
     */
    static bool loadGame(int& currentLevel, std::vector<Movement>& movements);
    
private:
    /**
     * @brief Busca un archivo en múltiples rutas posibles
     * @param filename Nombre del archivo a buscar
     * @return Ruta completa del archivo si se encuentra, cadena vacía si no
     */
    static std::string findFile(const std::string& filename);
    
    /**
     * @brief Obtiene el nombre del archivo de nivel
     * @param levelNumber Número del nivel
     * @return Nombre del archivo del nivel
     */
    static std::string getLevelFileName(int levelNumber);
};

#endif // FILEMANAGER_H
