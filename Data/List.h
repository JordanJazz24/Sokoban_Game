//
// Created by jorda on 8/30/2023.
//

#ifndef PROYECTO_SOKOBAN_LIST_H
#define PROYECTO_SOKOBAN_LIST_H
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>
#include "Node.h"
using namespace std;
class List {
private:
    Node* head;      // Puntero al primer nodo (esquina superior izquierda)
    Node* playerNode;
    int numRows;     // Número de filas en la matriz
    int numCols;     // Número de columnas en la matriz

public:
    List(int level);  // Constructor para inicializar la lista y la matriz
    ~List();                   // Destructor para liberar la memoria de los nodos

    // Métodos para acceder y manipular la matriz
    char getSymbol(int row, int col) const;
    void setSymbol(int row, int col, char symbol);
    void createGrid(char** mat) ;
    void movePlayer(Node*& playerNode, char direction) ;
    void printGrid() const;
    char** getLevel(int level);


    // Otros métodos necesarios para la gestión de la matriz
};



#endif //PROYECTO_SOKOBAN_LIST_H
