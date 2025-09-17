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
 * @enum Player
 * @brief Identifica a los jugadores
 */
enum Player {
    PLAYER_1,  ///< Jugador 1 (controles WASD)
    PLAYER_2   ///< Jugador 2 (controles flechas)
};

/**
 * @enum CellType
 * @brief Representa los tipos de celdas en el juego
 */
enum CellType {
    EMPTY = ' ',        ///< Celda vacía
    WALL = '#',         ///< Pared
    PLAYER = '@',       ///< Jugador 1
    PLAYER2 = '&',      ///< Jugador 2
    BOX = '$',          ///< Caja
    GOAL = '.',         ///< Objetivo
    BOX_ON_GOAL = '!',  ///< Caja en objetivo
    PLAYER_ON_GOAL = '+', ///< Jugador 1 en objetivo
    PLAYER2_ON_GOAL = '%' ///< Jugador 2 en objetivo
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
    Position player1Position;   ///< Posición del jugador 1
    Position player2Position;   ///< Posición del jugador 2
    bool player1OnGoal;        ///< Si el jugador 1 está en un objetivo
    bool player2OnGoal;        ///< Si el jugador 2 está en un objetivo
    
    GameState(int level = 1) : currentLevel(level), player1OnGoal(false), player2OnGoal(false) {}
};

#endif // TYPES_H
