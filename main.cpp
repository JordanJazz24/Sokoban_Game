#include <iostream>
#include <Windows.h>
#include <conio.h> // Para _getch()
#include "Data/List.h"

int main() {
    List* l = new List(1);
    l->printGrid();
    std::cout << std::endl;

    while (true) {
        std::cout << "Presione una tecla (W: Arriba, S: Abajo, A: Izquierda, D: Derecha, Q: Salir): ";

        char mov = _getch(); // Utilizamos _getch() para capturar una tecla sin necesidad de presionar "Enter"

        if (mov == 'Q' || mov == 'q') {
            break; // Salir del ciclo si el usuario ingresa 'Q' o 'q'
        }

        Movement movement;

        // Convertir la tecla ingresada en un movimiento válido
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
                std::cout << "Tecla no válida. Por favor, ingrese W, S, A, D o Q." << std::endl;
                continue; // Continuar al siguiente ciclo si la tecla no es válida
        }

        // Mover el personaje
        l->movePlayer(movement);

        // Limpiar la consola antes de imprimir el nuevo estado del tablero
        system("cls");

        l->printGrid();
        std::cout << std::endl;
    }

    delete l; // Liberar la memoria antes de salir

    return 0;
}
