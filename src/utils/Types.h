#ifndef TYPES_H
#define TYPES_H

/**
 * @file Types.h
 * @brief Definiciones de tipos y enumeraciones para el juego Sokoban
 */

/**
 * @enum Movement
 * @brief Representa los posibles movimientos del jugador
 */
enum Movement {
    UP,     ///< Movimiento hacia arriba
    DOWN,   ///< Movimiento hacia abajo
    LEFT,   ///< Movimiento hacia la izquierda
    RIGHT   ///< Movimiento hacia la derecha
};

/**
 * @enum CellType
 * @brief Representa los tipos de celdas en el juego
 */
enum CellType {
    EMPTY = ' ',        ///< Celda vacía
    WALL = '#',         ///< Pared
    PLAYER = '@',       ///< Jugador
    BOX = '$',          ///< Caja
    GOAL = '.',         ///< Objetivo
    BOX_ON_GOAL = '!',  ///< Caja en objetivo
    PLAYER_ON_GOAL = '+' ///< Jugador en objetivo
};

/**
 * @struct Position
 * @brief Representa una posición en el grid
 */
struct Position {
    int row;    ///< Fila
    int col;    ///< Columna
    
    Position(int r = 0, int c = 0) : row(r), col(c) {}
    
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
};

/**
 * @struct GameState
 * @brief Representa el estado actual del juego
 */
struct GameState {
    int currentLevel;           ///< Nivel actual
    Position playerPosition;    ///< Posición del jugador
    bool playerOnGoal;         ///< Si el jugador está en un objetivo
    
    GameState(int level = 1) : currentLevel(level), playerOnGoal(false) {}
};

#endif // TYPES_H
