#ifndef LEVEL_H
#define LEVEL_H

#include "Grid.h"
#include "../utils/Types.h"
#include "../utils/FileManager.h"
#include <vector>

/**
 * @class Level
 * @brief Clase responsable del manejo de niveles individuales
 * 
 * Principios aplicados:
 * - Responsabilidad Única: Se encarga únicamente de la lógica de niveles
 * - Dependencia por inversión: Depende de abstracciones (Grid, FileManager)
 */
class Level {
private:
    Grid* grid;                         ///< Grid del nivel actual
    int levelNumber;                    ///< Número del nivel actual
    std::vector<Movement> movements;    ///< Historial de movimientos
    
public:
    /**
     * @brief Constructor que carga un nivel específico
     * @param levelNum Número del nivel a cargar
     */
    explicit Level(int levelNum);
    
    /**
     * @brief Destructor que libera recursos
     */
    ~Level();
    
    /**
     * @brief Realiza un movimiento del jugador
     * @param movement Movimiento a realizar
     * @return true si el movimiento fue exitoso, false en caso contrario
     */
    bool makeMove(Movement movement);
    
    /**
     * @brief Reinicia el nivel al estado inicial
     */
    void reset();
    
    /**
     * @brief Verifica si el nivel está completado
     * @return true si el nivel está completado, false en caso contrario
     */
    bool isCompleted() const;
    
    /**
     * @brief Muestra el estado actual del nivel
     */
    void display() const;
    
    /**
     * @brief Reproduce todos los movimientos realizados
     */
    void replay() const;
    
    /**
     * @brief Obtiene el número del nivel
     * @return Número del nivel
     */
    int getLevelNumber() const;
    
    /**
     * @brief Obtiene el número de movimientos realizados
     * @return Número de movimientos
     */
    int getMoveCount() const;
    
    /**
     * @brief Obtiene el historial de movimientos
     * @return Vector con los movimientos realizados
     */
    const std::vector<Movement>& getMovements() const;
    
    /**
     * @brief Establece los movimientos (para cargar partida)
     * @param newMovements Vector de movimientos a establecer
     */
    void setMovements(const std::vector<Movement>& newMovements);
    
private:
    /**
     * @brief Carga el nivel desde archivo
     * @return true si se cargó exitosamente, false en caso contrario
     */
    bool loadLevel();
};

#endif // LEVEL_H
