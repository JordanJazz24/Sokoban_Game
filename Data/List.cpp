//
// Created by jorda on 8/30/2023.
//

#include <vector>
#include "List.h"

List::List(int level) : head(nullptr) {
    // Inicializar el número de filas y columnas
    numRows = 0;
    numCols = 0;
    numBoxes= 0;
    goalStack = new std::stack<Node*>();
    playerNode = nullptr;
    head = nullptr;
    // Crear la matriz a partir del archivo
    char** mat = getLevel(level); //
    createGrid(mat);
}

List::~List() {
    // Liberar la memoria de los nodos enlazados // no he revisado si funciona al 100
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

char List::getSymbol(int row, int col) const {
    // Obtener el símbolo en la posición (row, col)
    Node* node = head;
    for (int i = 1; i < row; ++i) {
        node = node->down;
    }
    for (int j = 1; j < col; ++j) {
        node = node->right;
    }
    return node->symbol;
}

void List::setSymbol(int row, int col, char symbol) {
    // Establecer el símbolo en la posición (row, col)
    Node* node = head;
    for (int i = 1; i < row; ++i) { //si es una estructura de 4x4, para llegar al elemento del lugar 4x1
        node = node->down;           //tendríamos que bajar solo 3 veces porque el head apunta al lugar 1x1
    }
    for (int j = 1; j < col; ++j) {
        node = node->right;
    }
    node->symbol = symbol;
}

void List::printGrid() const {
    Node* downptr = head;
    Node* rightptr;
    while (downptr) {
        rightptr = downptr;
        while (rightptr) {
            cout << (rightptr->symbol) << " ";
            rightptr = rightptr->right;
        }
        cout << "\n";
        downptr = downptr->down;
    }
}


char** List::getLevel(int nivel) { //función para obtener el nivel del archivo no importa el tamaño de la matriz
    ifstream inputFile("Nivel " + std::to_string(nivel) + ".txt");

    if (!inputFile) {
        cerr << "Error al abrir el archivo." << endl;
        return nullptr;
    }


    int maxCols = 0; // Variable para almacenar la longitud máxima de la columna

    // Contar el número de filas y la longitud máxima de la columna
    char ch;
    int acumuladorCols = 0;
    while (inputFile.get(ch)) {
        if (ch != '\n') {
            acumuladorCols++;
        }
        else {
            numRows++;
            if (acumuladorCols > maxCols) {
                maxCols = acumuladorCols;
            }
            acumuladorCols = 0;
        }
    }
    numCols = maxCols;
    cout << "tamano de la matriz: " << numRows << "x" << maxCols << endl;
    //    }

    // Volver al principio del archivo
    inputFile.clear();
    inputFile.seekg(0, ios::beg);

    // Crear una matriz char** de acuerdo al número de filas y la longitud máxima de la columna
    char** mat = new char* [numRows];
    for (int i = 0; i < numRows; i++) {
        mat[i] = new char[maxCols];
    }
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < maxCols; j++) {
            mat[i][j] = ' ';
        }
    }

    // Leer el archivo caracter por caracter y llenar la matriz
    int row = 0, col = 0;
    while (inputFile.get(ch)) {
        if (ch != '\n') {
            mat[row][col++] = ch;
        }
        else {
            row++;
            col = 0;
        }
    }

    inputFile.close(); // Cierra el archivo de entrada

    return mat;
}

void List::createGrid(char** mat) {
    // Inicialización de punteros
    Node* head_main = nullptr;    // Puntero al nodo principal (cabeza de la lista principal)
    Node * upper = new Node(-1);  // Punteros para rastrear nodos anteriores y superiores

    // Iterar a través de las filas
    for (int i = 0; i < numRows; i++) {
        Node* head_row = nullptr; // Puntero al nodo principal de la fila actual
        Node* prev = new Node(-1); // Nodo temporal para rastrear el nodo anterior en la fila actual

        // Iterar a través de las columnas
        for (int j = 0; j < numCols; j++) {
            Node* temp = new Node(mat[i][j]); // Crear un nuevo nodo con el valor del carácter en la matriz


            if (mat[i][j] == '@') {
                // Si encontramos el caracter del jugador, almacenamos una referencia a este nodo
                this->playerNode = temp;
            }

            if (mat[i][j] == '$') {
                // Si encontramos el caracter de una caja, incrementamos el número de cajas
                this->numBoxes++;
            }

            if (j == 0) head_row = temp; // Si es la primera columna, establecer como cabeza de fila
            if (i == 0 && j == 0) head_main = temp; // Si es la primera celda, establecer como cabeza principal

            // Conectar nodos en dirección horizontal (izquierda y derecha)
            temp->left = prev;
            prev->right = temp;

            // Conectar nodos en dirección vertical (arriba y abajo)
            if (i == numRows - 1) temp->down = NULL; // Si es la última fila, establecer como NULL
            if (!upper->right) {
                upper->right = new Node(-1); // Crear un nodo superior si es necesario
            }
            upper = upper->right;

            temp->up = upper;
            upper->down = temp;
            prev = temp;

            if (j == numCols - 1) prev->right = NULL; // Si es la última columna, establecer como NULL
        }

        upper = head_row->left; // Mover el puntero superior a la columna izquierda de la fila siguiente
    }

    head = head_main; // Establecer la cabeza de la lista principal
}

void List::movePlayer(Movement movement) {
    Node* nextNode = nullptr;

    // Determinar el nodo en la dirección deseada
    switch (movement) {
        case UP:
            if (isValidMove(playerNode->up)) { //si el movimiento es válido
                nextNode = playerNode->up;
            }
            else if (isCellBox(playerNode->up)) { //si el jugador se quiere mover a una caja
                //mover la caja
                if (isCellFree(playerNode->up->up)) {//
                    playerNode->up->symbol = ' ';
                    playerNode->up->up->symbol = '$';
                    //mover al jugador
                    nextNode = playerNode->up;
                }
                if (isCellGoal(playerNode->up->up)) {
                    playerNode->up->symbol = ' ';
                    playerNode->up->up->symbol = '!';
                    goalStack->push(playerNode->up->up); // Apilar el nodo en la pila de cajas en posición final
                    //mover al jugador
                    nextNode = playerNode->up;
                }
            } else if (isBoxInPoint(playerNode->up)) { //si el jugador  mover una caja que ya estaba en un punto
                //mover la caja
                if (isCellFree(playerNode->up->up)) {
                    playerNode->up->symbol = '.';
                    playerNode->up->up->symbol = '$';
                    if (goalStack->size() > 0) {
                        goalStack->pop(); // Desapilar el nodo de la pila de cajas en posición final
                    }
                    //mover al jugador
                    nextNode = playerNode->up;
                }
                if (isCellGoal(playerNode->up->up)) { //mueve la caja a otro punto
                    // Verificar si nextNode es una caja en una posición final
                    playerNode->up->symbol = '.';
                    playerNode->up->up->symbol = '!';

                    /*no debo apilar porque ya estaba en un punto*/

                    //mover al jugador
                    nextNode = playerNode->up;
                }
            }
            break;
        case DOWN:
            if (isValidMove(playerNode->down)) {
                nextNode = playerNode->down;
            }
            if (isCellBox(playerNode->down)) { //si el jugador se quiere mover a una caja
                //mover la caja
                if (isCellFree(playerNode->down->down)) {
                    playerNode->down->symbol = ' ';
                    playerNode->down->down->symbol = '$';
                    //mover al jugador
                    nextNode = playerNode->down;
                }
                if (isCellGoal(playerNode->down->down)) {
                    playerNode->down->symbol = ' ';
                    playerNode->down->down->symbol = '!';
                    goalStack->push(playerNode->down->down); // Apilar el nodo en la pila de cajas en posición final
                    //mover al jugador
                    nextNode = playerNode->down;
                }
            }else if (isBoxInPoint(playerNode->down)) { //si el jugador  mover una caja que ya estaba en un punto
                //mover la caja
                if (isCellFree(playerNode->down->down)) {
                    playerNode->down->symbol = '.';
                    playerNode->down->down->symbol = '$';
                    if (goalStack->size() > 0) {
                        goalStack->pop(); // Desapilar el nodo de la pila de cajas en posición final
                    }

                    //mover al jugador
                    nextNode = playerNode->down;
                }
                if (isCellGoal(playerNode->down->down)) {
                    playerNode->down->symbol = '.';
                    playerNode->down->down->symbol = '!';
                    //no debo apilar porque ya estaba en un punto

                    //mover al jugador
                    nextNode = playerNode->down;
                }
            }
            break;
        case LEFT:
            if (isValidMove(playerNode->left)) {
                nextNode = playerNode->left;
            }
            if (isCellBox(playerNode->left)) { //si el jugador se quiere mover a una caja
                //mover la caja
                if (isCellFree(playerNode->left->left)) {
                    playerNode->left->symbol = ' ';
                    playerNode->left->left->symbol = '$';
                    //mover al jugador
                    nextNode = playerNode->left;
                }
                if  (isCellGoal(playerNode->left->left)) {
                    playerNode->left->symbol = ' ';
                    playerNode->left->left->symbol = '!';
                    goalStack->push(playerNode->left->left); // Apilar el nodo en la pila de cajas en posición final
                    //mover al jugador
                    nextNode = playerNode->left;
                }
            }else if (isBoxInPoint(playerNode->left)) { //si el jugador  mover una caja que ya estaba en un punto
                //mover la caja
                if (isCellFree(playerNode->left->left)) {
                    playerNode->left->symbol = '.';
                    playerNode->left->left->symbol = '$';
                    if (goalStack->size() > 0) {
                        goalStack->pop(); // Desapilar el nodo de la pila de cajas en posición final
                    }
                    //mover al jugador
                    nextNode = playerNode->left;
                }
                if  (isCellGoal(playerNode->left->left)) {
                    playerNode->left->symbol = '.';
                    playerNode->left->left->symbol = '!';

                    //no debo apilar porque ya estaba en un punto
                    //mover al jugador
                    nextNode = playerNode->left;
                }
            }
            break;

        case RIGHT:
            if (isValidMove(playerNode->right)) {
                nextNode = playerNode->right;
            }
            if (isCellBox(playerNode->right)) { //si el jugador se quiere mover a una caja
                //mover la caja
                if (isCellFree(playerNode->right->right)) {
                    playerNode->right->symbol = ' ';
                    playerNode->right->right->symbol = '$';
                    //mover al jugador
                    nextNode = playerNode->right;
                }
                if (isCellGoal(playerNode->right->right)) {
                    playerNode->right->symbol = ' ';
                    playerNode->right->right->symbol = '!';
                    goalStack->push(playerNode->right->right); // Apilar el nodo en la pila de cajas en posición final
                    //mover al jugador
                    nextNode = playerNode->right;
                }
            }else if (isBoxInPoint(playerNode->right)) { //si el jugador  mover una caja que ya estaba en un punto
                //mover la caja
                if (isCellFree(playerNode->right->right)) {
                    playerNode->right->symbol = '.';
                    playerNode->right->right->symbol = '$';
                    if (goalStack->size() > 0) {
                        goalStack->pop(); // Desapilar el nodo de la pila de cajas en posición final
                    }

                    //mover al jugador
                    nextNode = playerNode->right;
                }
                if (isCellGoal(playerNode->right->right)) {
                    playerNode->right->symbol = '.';
                    playerNode->right->right->symbol = '!';
                    //no debo apilar porque ya estaba en un punto

                    //mover al jugador
                    nextNode = playerNode->right;
                }
            }

            break;
        default:
            break;
    }

    // Verificar si el movimiento es válido (por ejemplo, si nextNode no es nulo)
    if (nextNode != nullptr) {


        if(isCellGoal(nextNode)){
            if(playerInPoint){ //si el jugador ya estaba en un punto y se mueve a otro punto
                nextNode->symbol = '.';
                swapSymbols(nextNode);
                return;
            }
            playerInPoint = true;
            nextNode->symbol = ' ';
            swapSymbols(nextNode);
            return;
        }
        if (playerInPoint) { //si el jugador estaba en un punto y se mueve a un espacio vacio
            nextNode->symbol = '.';
            swapSymbols(nextNode);
            playerInPoint = false;
        }
        else{

            swapSymbols(nextNode);
        }
    }
}

bool List::isValidMove(Node* dirNode) { //recibe el nodo al que se quiere mover

    if (dirNode->symbol == ' ' || dirNode->symbol=='.' ) { //si el simbolo del nodo
        return true;                                       //al que se quiere mover es un espacio o un punto
    }
    cout << "Movimiento invalido" << endl;
    return false;
}

bool List::isCellFree(Node *dirNode) {
    if (dirNode->symbol == ' ' ) { //jugador puede moverse a un espacio vacio o a un punto
        return true;
    }
    return false;
}

bool List::isCellGoal(Node *dirNode) {
    if (dirNode->symbol == '.') { //
        return true;
    }
    return false;
}

bool List::isCellBox(Node *dirNode) {
    if (dirNode->symbol == '$') { //jugador puede moverse a un espacio vacio o a un punto
        return true;
    }
    return false;
}

void List::swapSymbols(Node *&nextNode) {
    char temp = playerNode->symbol;
    playerNode->symbol = nextNode->symbol;
    nextNode->symbol = temp;
    this->playerNode = nextNode;

}

bool List::isBoxInPoint(Node *dirNode) {
    if (dirNode->symbol == '!') { //jugador puede moverse a un espacio vacio o a un punto
        return true;
    }
    return false;
}

void List::resetLevel(int level) {

    List::~List(); // Liberar la memoria de los nodos enlazado

    // Inicializar el número de filas y columnas
    numRows = 0;
    numCols = 0;
    numBoxes= 0;
    goalStack = new std::stack<Node*>();
    playerNode = nullptr;
    head = nullptr;

    // volver a crear la matriz a partir del archivo
    char** mat = getLevel(level); //
    createGrid(mat);

}


