#include <iostream>
#include "Data/List.h"

int main() {
    List* l = new List(1);
    l->printGrid();
    std::cout << std::endl;
    char mov; // Cambiamos el tipo de variable a char para capturar una letra

    while (true) {
        std::cout << "Ingrese movimiento (W: Arriba, S: Abajo, A: Izquierda, D: Derecha, Q: Salir): ";
        std::cin >> mov;

        if (mov == 'Q' || mov == 'q') {
            break; // Salir del ciclo si el usuario ingresa 'Q' o 'q'
        }

        Movement movement;

        // Convertir la letra ingresada en un movimiento válido
        switch (mov) {
            case 'W':
            case 'w':
                movement = UP;
                break;
            case 'S':
            case 's':
                movement = DOWN;
                break;
            case 'A':
            case 'a':
                movement = LEFT;
                break;
            case 'D':
            case 'd':
                movement = RIGHT;
                break;
            default:
                std::cout << "Movimiento no válido. Por favor, ingrese W, S, A, D o Q." << std::endl;
                continue; // Continuar al siguiente ciclo si el movimiento no es válido
        }

        // Mover el personaje
        l->movePlayer(movement);

        // Imprimir el nuevo estado del tablero

        l->printGrid();
        std::cout << std::endl;
    }

    delete l; // Liberar la memoria antes de salir

    return 0;
}
