#include "Grid.h"
#include "../utils/Types.h"
#include <iostream>
#include <chrono>
#include <omp.h>
#include <atomic>
#include <vector>

// ============================================================================
// DEFINIR VERSIÓN A USAR (comentar/descomentar según necesites)
// ============================================================================
 #define USE_SEQUENTIAL_VERSION  // Descomentar para versión SERIAL

Grid::Grid(char** levelMatrix, int rows, int cols) 
    : head(nullptr), player1Node(nullptr), player2Node(nullptr), 
      player1InPoint(false), player2InPoint(false),
      numBoxes(0), numRows(rows), numCols(cols),
      player1MoveCount(0), player2MoveCount(0),
      lastRefreshTime(0.0), gridCreationTime(0.0), displayRenderTime(0.0) {
    goalStack = new std::stack<Node*>();
    createGridStructure(levelMatrix);
}

Grid::~Grid() {
    clearGrid();
    delete goalStack;
}

#ifdef USE_SEQUENTIAL_VERSION

// ============================================================================
// VERSIÓN 1: createGridStructure() - SECUENCIAL
// ============================================================================
void Grid::createGridStructure(char** matrix) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Contadores para análisis
    int playerCount = 0, boxCount = 0, goalCount = 0, wallCount = 0;
    
    Node* head_main = nullptr;
    Node* upper = new Node(-1);

    for (int i = 0; i < numRows; i++) {
        Node* head_row = nullptr;
        Node* prev = new Node(-1);

        for (int j = 0; j < numCols; j++) {
            char symbol = matrix[i][j];
            
            // 1. ANÁLISIS de elementos (trabajo adicional)
            switch (symbol) {
                case PLAYER: case PLAYER2: case PLAYER_ON_GOAL: case PLAYER2_ON_GOAL:
                    playerCount++;
                    break;
                case BOX: case BOX_ON_GOAL:
                    boxCount++;
                    break;
                case GOAL:
                    goalCount++;
                    break;
                case WALL:
                    wallCount++;
                    break;
            }
            
            // 2. VALIDACIÓN de integridad (trabajo adicional)
            bool isValidSymbol = (symbol == WALL || symbol == EMPTY || symbol == BOX ||
                                 symbol == GOAL || symbol == PLAYER || symbol == PLAYER2 ||
                                 symbol == PLAYER_ON_GOAL || symbol == PLAYER2_ON_GOAL ||
                                 symbol == BOX_ON_GOAL);
            if (!isValidSymbol) {
                symbol = EMPTY; // Normalizar caracteres inválidos
            }
            
            // 3. SIMULACIÓN de procesamiento por nodo (trabajo adicional)
            volatile int dummy = 0;
            for (int k = 0; k < 30; ++k) {
                dummy += (symbol * i * j + k) % 7;
            }
            
            // 4. CREAR nodo con símbolo procesado
            Node* temp = new Node(symbol);

            // Identificar elementos especiales
            if (symbol == PLAYER) {
                this->player1Node = temp;
            }
            if (symbol == PLAYER2) {
                this->player2Node = temp;
            }
            if (symbol == BOX) {
                this->numBoxes++;
            }

            if (j == 0) head_row = temp;
            if (i == 0 && j == 0) head_main = temp;

            // Conectar horizontalmente
            temp->left = prev;
            prev->right = temp;

            // Conectar verticalmente
            if (i == numRows - 1) temp->down = nullptr;
            if (!upper->right) {
                upper->right = new Node(-1);
            }
            upper = upper->right;

            temp->up = upper;
            upper->down = temp;
            prev = temp;

            if (j == numCols - 1) prev->right = nullptr;
        }

        upper = head_row->left;
    }

    head = head_main;
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    gridCreationTime = duration.count() / 1000.0;
    
 
}

#else

// ============================================================================
// VERSIÓN 2: createGridStructure() - PARALELA (OpenMP)
// ============================================================================
void Grid::createGridStructure(char** matrix) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Contadores atómicos para thread-safety
    std::atomic<int> playerCount(0), boxCount(0), goalCount(0), wallCount(0);
    
    // Crear estructura de nodos (pre-asignación en paralelo)
    std::vector<std::vector<Node*>> nodeMatrix(numRows, std::vector<Node*>(numCols, nullptr));
    
    // FASE 1: Creación de nodos en paralelo con análisis
    #pragma omp parallel for schedule(dynamic, 4)
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            char symbol = matrix[i][j];
            
            // 1. ANÁLISIS de elementos (trabajo adicional)
            switch (symbol) {
                case PLAYER: case PLAYER2: case PLAYER_ON_GOAL: case PLAYER2_ON_GOAL:
                    playerCount++;
                    break;
                case BOX: case BOX_ON_GOAL:
                    boxCount++;
                    break;
                case GOAL:
                    goalCount++;
                    break;
                case WALL:
                    wallCount++;
                    break;
            }
            
            // 2. VALIDACIÓN de integridad (trabajo adicional)
            bool isValidSymbol = (symbol == WALL || symbol == EMPTY || symbol == BOX ||
                                 symbol == GOAL || symbol == PLAYER || symbol == PLAYER2 ||
                                 symbol == PLAYER_ON_GOAL || symbol == PLAYER2_ON_GOAL ||
                                 symbol == BOX_ON_GOAL);
            if (!isValidSymbol) {
                symbol = EMPTY;
            }
            
            // 3. SIMULACIÓN de procesamiento por nodo (trabajo adicional)
            volatile int dummy = 0;
            for (int k = 0; k < 30; ++k) {
                dummy += (symbol * i * j + k) % 7;
            }
            
            // 4. CREAR nodo
            nodeMatrix[i][j] = new Node(symbol);
            
            // Identificar elementos especiales
            if (symbol == PLAYER) {
                this->player1Node = nodeMatrix[i][j];
            }
            if (symbol == PLAYER2) {
                this->player2Node = nodeMatrix[i][j];
            }
            if (symbol == BOX) {
                this->numBoxes++;
            }
        }
    }
    
    // FASE 2: Conectar nodos (secuencial, ya que requiere punteros)
    Node* upper = new Node(-1);
    Node* head_main = nullptr;
    
    for (int i = 0; i < numRows; i++) {
        Node* prev = new Node(-1);
        
        for (int j = 0; j < numCols; j++) {
            Node* temp = nodeMatrix[i][j];
            
            if (j == 0 && i == 0) head_main = temp;
            
            // Conectar horizontalmente
            temp->left = prev;
            prev->right = temp;
            
            // Conectar verticalmente
            if (i == numRows - 1) temp->down = nullptr;
            if (!upper->right) {
                upper->right = new Node(-1);
            }
            upper = upper->right;
            
            temp->up = upper;
            upper->down = temp;
            prev = temp;
            
            if (j == numCols - 1) prev->right = nullptr;
        }
        
        upper = nodeMatrix[i][0]->left;
    }
    
    head = head_main;
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    gridCreationTime = duration.count() / 1000.0;
    
 
}

#endif

void Grid::clearGrid() {
    Node* current = head;
    while (current) {
        Node* next = current->right;
        Node* rowNode = current;
        while (rowNode) {
            Node* toDelete = rowNode;
            rowNode = rowNode->down;
            delete toDelete;
        }
        current = next;
    }
}

bool Grid::movePlayer(Player player, Movement movement) {
    Node* playerNode = getPlayerNode(player);
    bool& playerInPoint = (player == PLAYER_1) ? player1InPoint : player2InPoint;
    
    if (!playerNode) {
        return false;
    }
    
    Node* nextNode = nullptr;
    Node* directionNode = nullptr;

    // Determinar dirección del movimiento
    switch (movement) {
        case UP:    directionNode = playerNode->up; break;
        case DOWN:  directionNode = playerNode->down; break;
        case LEFT:  directionNode = playerNode->left; break;
        case RIGHT: directionNode = playerNode->right; break;
        default: 
            return false;
    }

    if (!directionNode) {
        return false;
    }

    // Verificar si puede moverse directamente
    if (isValidMove(player, directionNode)) {
        nextNode = directionNode;
    }
    // Verificar si hay una caja que se puede mover
    else if (isCellBox(directionNode) || isBoxInGoal(directionNode)) {
        Node* boxTarget = nullptr;
        switch (movement) {
            case UP:    boxTarget = directionNode->up; break;
            case DOWN:  boxTarget = directionNode->down; break;
            case LEFT:  boxTarget = directionNode->left; break;
            case RIGHT: boxTarget = directionNode->right; break;
        }

        if (boxTarget && handleBoxMovement(directionNode, boxTarget)) {
            nextNode = directionNode;
        }
    }

    // Realizar el movimiento del jugador
    bool success = false;
    if (nextNode != nullptr) {
        char playerSymbol = (player == PLAYER_1) ? PLAYER : PLAYER2;
        char playerOnGoalSymbol = (player == PLAYER_1) ? PLAYER_ON_GOAL : PLAYER2_ON_GOAL;
        
        // Manejar jugador en objetivo
        if (isCellGoal(nextNode)) {
            if (playerInPoint) {
                playerNode->symbol = GOAL;
            } else {
                playerNode->symbol = EMPTY;
                playerInPoint = true;
            }
            nextNode->symbol = playerOnGoalSymbol;
            
            if (player == PLAYER_1) {
                player1Node = nextNode;
            } else {
                player2Node = nextNode;
            }
        } else {
            if (playerInPoint) {
                playerNode->symbol = GOAL;
                playerInPoint = false;
            } else {
                playerNode->symbol = EMPTY;
            }
            nextNode->symbol = playerSymbol;
            
            if (player == PLAYER_1) {
                player1Node = nextNode;
            } else {
                player2Node = nextNode;
            }
        }
        success = true;
    }

    // Actualizar contadores de movimientos
    if (success) {
        if (player == PLAYER_1) {
            player1MoveCount++;
        } else {
            player2MoveCount++;
        }
    }

    return success;
}

bool Grid::handleBoxMovement(Node* boxNode, Node* targetNode) {
    // Verificar si la caja se puede mover
    if (isCellFree(targetNode)) {
        // Mover caja normal a espacio libre
        if (isBoxInGoal(boxNode)) {
            boxNode->symbol = GOAL;
            if (goalStack->size() > 0) {
                goalStack->pop();
            }
        } else {
            boxNode->symbol = EMPTY;
        }
        targetNode->symbol = BOX;
        return true;
    } 
    else if (isCellGoal(targetNode)) {
        // Mover caja a objetivo
        if (isBoxInGoal(boxNode)) {
            boxNode->symbol = GOAL;
            // No cambiar goalStack ya que sigue siendo una caja en objetivo
        } else {
            boxNode->symbol = EMPTY;
            goalStack->push(targetNode);
        }
        targetNode->symbol = BOX_ON_GOAL;
        return true;
    }

    return false;
}

#ifdef USE_SEQUENTIAL_VERSION

// ============================================================================
// VERSIÓN 1: printGrid() - SECUENCIAL
// ============================================================================
void Grid::printGrid() const {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Contadores para análisis del display
    int totalCells = 0, specialChars = 0;
    
    Node* downptr = head;
    while (downptr) {
        Node* rightptr = downptr;
        while (rightptr) {
            char symbol = rightptr->symbol;
            
            // 1. ANÁLISIS del carácter (trabajo adicional)
            totalCells++;
            if (symbol != EMPTY && symbol != WALL) {
                specialChars++;
            }
            
            // 2. VALIDACIÓN de símbolo (trabajo adicional)
            bool isValid = (symbol == WALL || symbol == EMPTY || symbol == BOX ||
                           symbol == GOAL || symbol == PLAYER || symbol == PLAYER2 ||
                           symbol == PLAYER_ON_GOAL || symbol == PLAYER2_ON_GOAL ||
                           symbol == BOX_ON_GOAL);
            
            // 3. SIMULACIÓN de procesamiento de color/estilo (trabajo adicional)
            volatile int colorCode = 0;
            for (int k = 0; k < 20; ++k) {
                colorCode += (symbol * totalCells + k) % 5;
            }
            
            // 4. RENDERIZADO del carácter
            char displayChar = isValid ? symbol : EMPTY;
            std::cout << displayChar << " ";
            
            rightptr = rightptr->right;
        }
        std::cout << "\n";
        downptr = downptr->down;
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    const_cast<Grid*>(this)->displayRenderTime = duration.count() / 1000.0;
}

#else

// ============================================================================
// VERSIÓN 2: printGrid() - PARALELA (OpenMP)
// ============================================================================
void Grid::printGrid() const {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Contadores atómicos
    std::atomic<int> totalCells(0), specialChars(0);
    
    // FASE 1: Recolectar filas en paralelo con análisis
    std::vector<std::string> rows(numRows);
    
    #pragma omp parallel
    {
        // Cada hilo procesa filas
        #pragma omp for schedule(dynamic, 2)
        for (int rowIdx = 0; rowIdx < numRows; ++rowIdx) {
            std::string rowStr;
            rowStr.reserve(numCols * 2); // Pre-reservar espacio
            
            // Navegar a la fila correcta
            Node* downptr = head;
            for (int i = 0; i < rowIdx && downptr; ++i) {
                downptr = downptr->down;
            }
            
            if (!downptr) continue;
            
            Node* rightptr = downptr;
            int cellCount = 0;
            int specialCount = 0;
            
            while (rightptr) {
                char symbol = rightptr->symbol;
                
                // 1. ANÁLISIS del carácter (trabajo adicional)
                cellCount++;
                if (symbol != EMPTY && symbol != WALL) {
                    specialCount++;
                }
                
                // 2. VALIDACIÓN de símbolo (trabajo adicional)
                bool isValid = (symbol == WALL || symbol == EMPTY || symbol == BOX ||
                               symbol == GOAL || symbol == PLAYER || symbol == PLAYER2 ||
                               symbol == PLAYER_ON_GOAL || symbol == PLAYER2_ON_GOAL ||
                               symbol == BOX_ON_GOAL);
                
                // 3. SIMULACIÓN de procesamiento de color/estilo (trabajo adicional)
                volatile int colorCode = 0;
                for (int k = 0; k < 20; ++k) {
                    colorCode += (symbol * cellCount + k) % 5;
                }
                
                // 4. AGREGAR carácter al buffer de la fila
                char displayChar = isValid ? symbol : EMPTY;
                rowStr += displayChar;
                rowStr += ' ';
                
                rightptr = rightptr->right;
            }
            
            rows[rowIdx] = rowStr;
            
            // Actualizar contadores globales
            totalCells += cellCount;
            specialChars += specialCount;
        }
    }
    
    // FASE 2: Imprimir filas (secuencial para mantener orden)
    for (const auto& row : rows) {
        std::cout << row << "\n";
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    const_cast<Grid*>(this)->displayRenderTime = duration.count() / 1000.0;
}

#endif

void Grid::resetGrid(char** levelMatrix, int rows, int cols) {
    clearGrid();
    
    // Limpiar goalStack
    while (!goalStack->empty()) {
        goalStack->pop();
    }
    
    // Reinicializar variables
    numRows = rows;
    numCols = cols;
    numBoxes = 0;
    player1Node = nullptr;
    player2Node = nullptr;
    head = nullptr;
    player1InPoint = false;
    player2InPoint = false;
    player1MoveCount = 0;
    player2MoveCount = 0;
    lastRefreshTime = 0.0;
    gridCreationTime = 0.0;
    displayRenderTime = 0.0;
    
    createGridStructure(levelMatrix);
}

bool Grid::isLevelCompleted() const {
    return static_cast<int>(goalStack->size()) == numBoxes;
}

int Grid::getBoxesInGoals() const {
    return goalStack->size();
}

bool Grid::isValidMove(Player player, Node* targetNode) const {
    if (!targetNode) return false;
    
    // No puede moverse si hay otro jugador en la celda destino
    if (isPlayerInNode(targetNode)) return false;
    
    return (targetNode->symbol == EMPTY || targetNode->symbol == GOAL);
}

Node* Grid::getPlayerNode(Player player) const {
    return (player == PLAYER_1) ? player1Node : player2Node;
}

bool Grid::isPlayerInNode(Node* node) const {
    if (!node) return false;
    return (node->symbol == PLAYER || node->symbol == PLAYER2 || 
            node->symbol == PLAYER_ON_GOAL || node->symbol == PLAYER2_ON_GOAL);
}

bool Grid::isCellFree(Node* node) const {
    return node && node->symbol == EMPTY;
}

bool Grid::isCellGoal(Node* node) const {
    return node && node->symbol == GOAL;
}

bool Grid::isCellBox(Node* node) const {
    return node && node->symbol == BOX;
}

bool Grid::isBoxInGoal(Node* node) const {
    return node && node->symbol == BOX_ON_GOAL;
}

void Grid::swapSymbols(Player player, Node*& targetNode) {
    Node* playerNode = getPlayerNode(player);
    if (!playerNode) return;
    
    char temp = playerNode->symbol;
    playerNode->symbol = targetNode->symbol;
    targetNode->symbol = temp;
    
    if (player == PLAYER_1) {
        player1Node = targetNode;
    } else {
        player2Node = targetNode;
    }
}

int Grid::getPlayer1MoveCount() const {
    return player1MoveCount;
}

int Grid::getPlayer2MoveCount() const {
    return player2MoveCount;
}

double Grid::getLastRefreshTime() const {
    return lastRefreshTime;
}

void Grid::setLastRefreshTime(double time) {
    lastRefreshTime = time;
}

double Grid::getGridCreationTime() const {
    return gridCreationTime;
}

double Grid::getDisplayRenderTime() const {
    return displayRenderTime;
}
