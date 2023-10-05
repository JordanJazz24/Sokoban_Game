//
// Created by jorda on 8/30/2023.
//

#ifndef PROYECTO_SOKOBAN_LIST_H
#define PROYECTO_SOKOBAN_LIST_H
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstdio>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include "Node.h"
enum Movement{UP, DOWN, LEFT, RIGHT};

using namespace std;
class List {
private:
    Node* head;      // Puntero al primer nodo (esquina superior izquierda) es mi grid
    Node* playerNode;
    bool playerInPoint = false;
    int numBoxes;    // Número de cajas en el nivel
    int numRows;     // Número de filas en la matriz
    int numCols;     // Número de columnas en la matriz
    std::stack<Node*> goalStack; // Pila para las cajas en la posición final


public:
    List(int level);  // Constructor para inicializar la lista y la matriz
    ~List();                   // Destructor para liberar la memoria de los nodos

    // Métodos para acceder y manipular la matriz
    char getSymbol(int row, int col) const;
    void setSymbol(int row, int col, char symbol);
    bool isValidMove( Node* dirNode);
    void createGrid(char** mat) ;
    void movePlayer(Movement movement);
    void printGrid() const;
    bool isCellFree(Node* dirNode);
    bool isCellGoal(Node* dirNode);
    bool isCellBox(Node* dirNode);
    bool isBoxInPoint(Node* dirNode);
    void swapSymbols(Node*& node);
    char** getLevel(int level);


    // Otros métodos necesarios para la gestión de la matriz
};



#endif //PROYECTO_SOKOBAN_LIST_H
