//
// Created by jorda on 8/30/2023.
//

#ifndef PROYECTO_SOKOBAN_NODE_H
#define PROYECTO_SOKOBAN_NODE_H


struct Node {
    char symbol;  // Símbolo que representa el contenido del nodo
    Node* up;     // Puntero al nodo superior
    Node* down;   // Puntero al nodo inferior
    Node* left;   // Puntero al nodo izquierdo
    Node* right;  // Puntero al nodo derecho

    // Constructor para inicializar un nodo con un símbolo dado
    Node(char s) : symbol(s), up(nullptr), down(nullptr), left(nullptr), right(nullptr) {}
};


#endif //PROYECTO_SOKOBAN_NODE_H
