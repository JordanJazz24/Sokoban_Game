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
    double loadTimeMs;                  ///< Tiempo de carga del archivo en milisegundos
    
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
     * @brief Realiza un movimiento del jugador especificado
     * @param player Jugador que realiza el movimiento (PLAYER_1 o PLAYER_2)
     * @param movement Movimiento a realizar
     * @return true si el movimiento fue exitoso, false en caso contrario
     */
    bool makeMove(Player player, Movement movement);
    
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
    
    /**
     * @brief Obtiene el tiempo de carga del archivo del nivel
     * @return Tiempo de carga en milisegundos
     */
    double getLoadTime() const;
    
    /**
     * @brief Obtiene el tiempo de creación del jugador 1
     * @return Tiempo de creación en milisegundos
     */
    double getPlayer1CreationTime() const;
    
    /**
     * @brief Obtiene el tiempo de creación del jugador 2
     * @return Tiempo de creación en milisegundos
     */
    double getPlayer2CreationTime() const;
    
    /**
     * @brief Obtiene el tiempo del último movimiento del jugador 1
     * @return Tiempo de movimiento en milisegundos
     */
    double getPlayer1LastMoveTime() const;
    
    /**
     * @brief Obtiene el tiempo del último movimiento del jugador 2
     * @return Tiempo de movimiento en milisegundos
     */
    double getPlayer2LastMoveTime() const;
    
    /**
     * @brief Obtiene el tiempo total acumulado de movimientos del jugador 1
     * @return Tiempo total en milisegundos
     */
    double getPlayer1TotalMoveTime() const;
    
    /**
     * @brief Obtiene el tiempo total acumulado de movimientos del jugador 2
     * @return Tiempo total en milisegundos
     */
    double getPlayer2TotalMoveTime() const;
    
    /**
     * @brief Obtiene el número de movimientos realizados por el jugador 1
     * @return Número de movimientos
     */
    int getPlayer1MoveCount() const;
    
    /**
     * @brief Obtiene el número de movimientos realizados por el jugador 2
     * @return Número de movimientos
     */
    int getPlayer2MoveCount() const;
    
    /**
     * @brief Obtiene el tiempo del último refresh del display
     * @return Tiempo de refresh en milisegundos
     */
    double getLastRefreshTime() const;
    
    /**
     * @brief Obtiene el tiempo de la última validación de estado
     * @return Tiempo de validación en milisegundos
     */
    double getLastValidationTime() const;
    
    /**
     * @brief Establece el tiempo del último refresh del display
     * @param time Tiempo en milisegundos
     */
    void setLastRefreshTime(double time);
    
    /**
     * @brief Establece el tiempo de la última validación de estado
     * @param time Tiempo en milisegundos
     */
    void setLastValidationTime(double time);
    
private:
    /**
     * @brief Carga el nivel desde archivo
     * @return true si se cargó exitosamente, false en caso contrario
     */
    bool loadLevel();
};

#endif // LEVEL_H
