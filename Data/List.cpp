//
// Created by jorda on 8/30/2023.
//

#include "List.h"
List::List(int level) : head(nullptr) {
    // Crear la matriz a partir del archivo
    char** mat = getLevel(level);
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


char **List::getLevel(int nivel ) { //función para obtener el nivel del archivo no importa el tamaño de la matriz
    ifstream inputFile("Nivel " + std::to_string(nivel) + ".txt");

    if (!inputFile) {
        cerr << "Error al abrir el archivo." << endl;
        return nullptr;
    }

    numRows = 0;
    numCols = 0;
    int maxCols = 0; // Variable para almacenar la longitud máxima de la columna

    // Contar el número de filas y la longitud máxima de la columna
    char ch;
    int acumuladorCols = 0;
    while (inputFile.get(ch)) {
        if (ch != '\n') {
            acumuladorCols++;
        } else {
            numRows++;
            if (acumuladorCols > maxCols) {
                maxCols = acumuladorCols;
            }
            acumuladorCols = 0;
        }
    }
    numCols = maxCols;
    cout<<"tamano de la matriz: "<<numRows<<"x"<<maxCols<<endl;
    //    }

    // Volver al principio del archivo
    inputFile.clear();
    inputFile.seekg(0, ios::beg);

    // Crear una matriz char** de acuerdo al número de filas y la longitud máxima de la columna
    char** mat = new char*[numRows];
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
        } else {
            row++;
            col = 0;
        }
    }

    inputFile.close(); // Cierra el archivo de entrada

    return mat;
}

void List::createGrid(char **mat) {
    Node* head_main = NULL;
    Node* prev, *upper = new Node(-1);
    for (int i = 0; i < numRows; i++) {
        Node* head_row;
        Node* prev = new Node(-1);

        for (int j = 0; j < numCols; j++) {
            Node* temp = new Node(mat[i][j]);

            if (j == 0) head_row = temp;
            if (i == 0 && j == 0) head_main = temp;

            temp->left = prev;
            prev->right = temp;
            if (i == numRows - 1) temp->down = NULL;

            if (!upper->right) {
                upper->right = new Node(-1);
            }
            upper = upper->right;

            temp->up = upper;
            upper->down = temp;
            prev = temp;

            if (j == numCols - 1) prev->right = NULL;
        }

        upper = head_row->left;
    }

    head= head_main;
}
