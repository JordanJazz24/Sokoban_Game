#ifndef GRID_H
#define GRID_H

#include "../utils/Types.h"
#include <stack>

/**
 * @class Node
 * @brief Representa un nodo en la grid enlazada
 */
class Node {
public:
    char symbol;        ///< Símbolo que representa el contenido del nodo
    Node* up;          ///< Puntero al nodo superior
    Node* down;        ///< Puntero al nodo inferior
    Node* left;        ///< Puntero al nodo izquierdo
    Node* right;       ///< Puntero al nodo derecho

    Node(char s) : symbol(s), up(nullptr), down(nullptr), left(nullptr), right(nullptr) {}
};

/**
 * @class Grid
 * @brief Clase responsable de la representación y manipulación del grid del juego
 * 
 * Principios aplicados:
 * - Responsabilidad Única: Se encarga únicamente del grid y movimientos
 * - Abierto/Cerrado: Extensible para nuevos tipos de movimiento
 */
class Grid {
private:
    Node* head;                     ///< Puntero al primer nodo (esquina superior izquierda)
    Node* playerNode;              ///< Puntero al nodo del jugador
    bool playerInPoint;            ///< Si el jugador está en un punto objetivo
    int numBoxes;                  ///< Número de cajas en el nivel
    int numRows;                   ///< Número de filas en la matriz
    int numCols;                   ///< Número de columnas en la matriz
    std::stack<Node*>* goalStack;  ///< Pila para las cajas en la posición final

public:
    /**
     * @brief Constructor que inicializa el grid con un nivel
     * @param levelMatrix Matriz de caracteres del nivel
     * @param rows Número de filas
     * @param cols Número de columnas
     */
    Grid(char** levelMatrix, int rows, int cols);
    
    /**
     * @brief Destructor que libera la memoria de los nodos
     */
    ~Grid();
    
    /**
     * @brief Mueve al jugador en la dirección especificada
     * @param movement Dirección del movimiento
     * @return true si el movimiento fue exitoso, false en caso contrario
     */
    bool movePlayer(Movement movement);
    
    /**
     * @brief Imprime el estado actual del grid
     */
    void printGrid() const;
    
    /**
     * @brief Reinicia el grid con un nuevo nivel
     * @param levelMatrix Nueva matriz del nivel
     * @param rows Número de filas
     * @param cols Número de columnas
     */
    void resetGrid(char** levelMatrix, int rows, int cols);
    
    /**
     * @brief Verifica si el nivel está completado
     * @return true si todas las cajas están en objetivos, false en caso contrario
     */
    bool isLevelCompleted() const;
    
    /**
     * @brief Obtiene el número de cajas en objetivos
     * @return Número de cajas en posición final
     */
    int getBoxesInGoals() const;

private:
    /**
     * @brief Crea la estructura enlazada del grid
     * @param matrix Matriz de caracteres del nivel
     */
    void createGridStructure(char** matrix);
    
    /**
     * @brief Libera la memoria de todos los nodos
     */
    void clearGrid();
    
    /**
     * @brief Verifica si un movimiento es válido
     * @param targetNode Nodo destino del movimiento
     * @return true si el movimiento es válido, false en caso contrario
     */
    bool isValidMove(Node* targetNode) const;
    
    /**
     * @brief Verifica si una celda está libre
     * @param node Nodo a verificar
     * @return true si la celda está libre, false en caso contrario
     */
    bool isCellFree(Node* node) const;
    
    /**
     * @brief Verifica si una celda es un objetivo
     * @param node Nodo a verificar
     * @return true si es un objetivo, false en caso contrario
     */
    bool isCellGoal(Node* node) const;
    
    /**
     * @brief Verifica si una celda contiene una caja
     * @param node Nodo a verificar
     * @return true si contiene una caja, false en caso contrario
     */
    bool isCellBox(Node* node) const;
    
    /**
     * @brief Verifica si una celda contiene una caja en objetivo
     * @param node Nodo a verificar
     * @return true si contiene una caja en objetivo, false en caso contrario
     */
    bool isBoxInGoal(Node* node) const;
    
    /**
     * @brief Intercambia los símbolos entre dos nodos
     * @param targetNode Nodo con el que intercambiar
     */
    void swapSymbols(Node*& targetNode);
    
    /**
     * @brief Maneja el movimiento de una caja
     * @param boxNode Nodo de la caja a mover
     * @param targetNode Nodo destino de la caja
     * @return true si se pudo mover la caja, false en caso contrario
     */
    bool handleBoxMovement(Node* boxNode, Node* targetNode);
};

#endif // GRID_H
