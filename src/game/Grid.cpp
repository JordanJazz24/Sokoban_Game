#include "Grid.h"
#include "../utils/Types.h"
#include <iostream>
#include <chrono>

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

void Grid::createGridStructure(char** matrix) {
    // Iniciar medición del tiempo de construcción del grid
    auto startTime = std::chrono::high_resolution_clock::now();
    
    Node* head_main = nullptr;
    Node* upper = new Node(-1);

    for (int i = 0; i < numRows; i++) {
        Node* head_row = nullptr;
        Node* prev = new Node(-1);

        for (int j = 0; j < numCols; j++) {
            Node* temp = new Node(matrix[i][j]);

            // Identificar elementos especiales
            if (matrix[i][j] == PLAYER) {
                this->player1Node = temp;
            }
            if (matrix[i][j] == PLAYER2) {
                this->player2Node = temp;
            }
            if (matrix[i][j] == BOX) {
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
    
    // Finalizar medición del tiempo de construcción del grid
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    gridCreationTime = duration.count() / 1000.0;
}

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

void Grid::printGrid() const {
    // Iniciar medición del tiempo de renderizado del display
    auto startTime = std::chrono::high_resolution_clock::now();
    
    Node* downptr = head;
    Node* rightptr;
    while (downptr) {
        rightptr = downptr;
        while (rightptr) {
            std::cout << rightptr->symbol << " ";
            rightptr = rightptr->right;
        }
        std::cout << "\n";
        downptr = downptr->down;
    }
    
    // Finalizar medición del tiempo de renderizado del display
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    // Necesitamos hacer cast para modificar en función const
    const_cast<Grid*>(this)->displayRenderTime = duration.count() / 1000.0;
}

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
