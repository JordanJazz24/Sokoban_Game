#include "Grid.h"
#include "../utils/Types.h"
#include <iostream>

Grid::Grid(char** levelMatrix, int rows, int cols) 
    : head(nullptr), playerNode(nullptr), playerInPoint(false), 
      numBoxes(0), numRows(rows), numCols(cols) {
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
                this->playerNode = temp;
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

bool Grid::movePlayer(Movement movement) {
    Node* nextNode = nullptr;
    Node* directionNode = nullptr;

    // Determinar dirección del movimiento
    switch (movement) {
        case UP:    directionNode = playerNode->up; break;
        case DOWN:  directionNode = playerNode->down; break;
        case LEFT:  directionNode = playerNode->left; break;
        case RIGHT: directionNode = playerNode->right; break;
        default: return false;
    }

    if (!directionNode) return false;

    // Verificar si puede moverse directamente
    if (isValidMove(directionNode)) {
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
    if (nextNode != nullptr) {
        // Manejar jugador en objetivo
        if (isCellGoal(nextNode)) {
            if (playerInPoint) {
                playerNode->symbol = GOAL;
            } else {
                playerNode->symbol = EMPTY;
                playerInPoint = true;
            }
            nextNode->symbol = PLAYER;
            playerNode = nextNode;
        } else {
            if (playerInPoint) {
                playerNode->symbol = GOAL;
                playerInPoint = false;
            } else {
                playerNode->symbol = EMPTY;
            }
            nextNode->symbol = PLAYER;
            playerNode = nextNode;
        }
        return true;
    }

    return false;
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
    playerNode = nullptr;
    head = nullptr;
    playerInPoint = false;
    
    createGridStructure(levelMatrix);
}

bool Grid::isLevelCompleted() const {
    return static_cast<int>(goalStack->size()) == numBoxes;
}

int Grid::getBoxesInGoals() const {
    return goalStack->size();
}

bool Grid::isValidMove(Node* targetNode) const {
    return targetNode && (targetNode->symbol == EMPTY || targetNode->symbol == GOAL);
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

void Grid::swapSymbols(Node*& targetNode) {
    char temp = playerNode->symbol;
    playerNode->symbol = targetNode->symbol;
    targetNode->symbol = temp;
    playerNode = targetNode;
}
