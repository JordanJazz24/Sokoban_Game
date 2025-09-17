#include "Grid.h"
#include "../utils/Types.h"
#include <iostream>
#include <chrono>

Grid::Grid(char** levelMatrix, int rows, int cols) 
    : head(nullptr), player1Node(nullptr), player2Node(nullptr), 
      player1InPoint(false), player2InPoint(false),
      numBoxes(0), numRows(rows), numCols(cols),
      player1CreationTime(0.0), player2CreationTime(0.0),
      player1LastMoveTime(0.0), player2LastMoveTime(0.0),
      player1TotalMoveTime(0.0), player2TotalMoveTime(0.0),
      player1MoveCount(0), player2MoveCount(0),
      lastRefreshTime(0.0), lastValidationTime(0.0) {
    goalStack = new std::stack<Node*>();
    createGridStructure(levelMatrix);
}

Grid::~Grid() {
    clearGrid();
    delete goalStack;
}

void Grid::createGridStructure(char** matrix) {
    Node* head_main = nullptr;
    Node* upper = new Node(-1);

    for (int i = 0; i < numRows; i++) {
        Node* head_row = nullptr;
        Node* prev = new Node(-1);

        for (int j = 0; j < numCols; j++) {
            Node* temp = new Node(matrix[i][j]);

            // Identificar elementos especiales
            if (matrix[i][j] == PLAYER) {
                auto startTime = std::chrono::high_resolution_clock::now();
                this->player1Node = temp;
                auto endTime = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
                this->player1CreationTime = duration.count() / 1000.0;
            }
            if (matrix[i][j] == PLAYER2) {
                auto startTime = std::chrono::high_resolution_clock::now();
                this->player2Node = temp;
                auto endTime = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
                this->player2CreationTime = duration.count() / 1000.0;
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
    // Iniciar medición del tiempo de movimiento
    auto startTime = std::chrono::high_resolution_clock::now();
    
    Node* playerNode = getPlayerNode(player);
    bool& playerInPoint = (player == PLAYER_1) ? player1InPoint : player2InPoint;
    
    if (!playerNode) {
        // Finalizar medición aunque falle
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        double moveTime = duration.count() / 1000.0;
        if (player == PLAYER_1) {
            player1LastMoveTime = moveTime;
            player1TotalMoveTime += moveTime;
        } else {
            player2LastMoveTime = moveTime;
            player2TotalMoveTime += moveTime;
        }
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
            // Finalizar medición aunque falle
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
            double moveTime = duration.count() / 1000.0;
            if (player == PLAYER_1) {
                player1LastMoveTime = moveTime;
                player1TotalMoveTime += moveTime;
            } else {
                player2LastMoveTime = moveTime;
                player2TotalMoveTime += moveTime;
            }
            return false;
    }

    if (!directionNode) {
        // Finalizar medición aunque falle
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        double moveTime = duration.count() / 1000.0;
        if (player == PLAYER_1) {
            player1LastMoveTime = moveTime;
            player1TotalMoveTime += moveTime;
        } else {
            player2LastMoveTime = moveTime;
            player2TotalMoveTime += moveTime;
        }
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

    // Finalizar medición del tiempo de movimiento
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    double moveTime = duration.count() / 1000.0;
    
    if (player == PLAYER_1) {
        player1LastMoveTime = moveTime;
        player1TotalMoveTime += moveTime;
        if (success) {
            player1MoveCount++;
        }
    } else {
        player2LastMoveTime = moveTime;
        player2TotalMoveTime += moveTime;
        if (success) {
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
    player1CreationTime = 0.0;
    player2CreationTime = 0.0;
    player1LastMoveTime = 0.0;
    player2LastMoveTime = 0.0;
    player1TotalMoveTime = 0.0;
    player2TotalMoveTime = 0.0;
    player1MoveCount = 0;
    player2MoveCount = 0;
    lastRefreshTime = 0.0;
    lastValidationTime = 0.0;
    
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

double Grid::getPlayer1CreationTime() const {
    return player1CreationTime;
}

double Grid::getPlayer2CreationTime() const {
    return player2CreationTime;
}

double Grid::getPlayer1LastMoveTime() const {
    return player1LastMoveTime;
}

double Grid::getPlayer2LastMoveTime() const {
    return player2LastMoveTime;
}

double Grid::getPlayer1TotalMoveTime() const {
    return player1TotalMoveTime;
}

double Grid::getPlayer2TotalMoveTime() const {
    return player2TotalMoveTime;
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

double Grid::getLastValidationTime() const {
    return lastValidationTime;
}

void Grid::setLastRefreshTime(double time) {
    lastRefreshTime = time;
}

void Grid::setLastValidationTime(double time) {
    lastValidationTime = time;
}
