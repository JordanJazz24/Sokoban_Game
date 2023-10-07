#include <iostream>
#include <Windows.h>
#include <conio.h> // Para _getch()
#include "../Data/List.h"

class GameMenu {
public:
    GameMenu() : game(nullptr), currentLevel(1){}

    void run() {
        while (true) {
            displayMenu();

            char choice = _getch(); // Capturar la elección del usuario

            switch (choice) {
                case '1': // Nueva partida
                    startNewGame(currentLevel);
                    break;
                case '2': // Cargar partida (debes implementar esta funcionalidad)
                    std::cout << "Funcionalidad de carga de partida no implementada todavía." << std::endl;
                    break;
                case '3': // Salir del juego
                    if (game != nullptr) {
                        delete game; // Liberar la memoria antes de salir
                    }
                    return;
                default:
                    std::cout << "Opción no válida. Por favor, seleccione 1, 2 o 3." << std::endl;
                    system("pause");
                    break;
            }

            // Entrar al bucle principal del juego si se eligió una opción válida
            if (game != nullptr) {
                system ("cls");
                runGame();
                delete game; // Liberar la memoria después de terminar la partida
                game = nullptr;
            }
        }
    }

private:
    List* game;
    int currentLevel = 1;

    void displayMenu() {
        system("cls"); // Limpiar la consola
        std::cout << "Bienvenido a Sokoban MIEDO!!" << std::endl;
        std::cout << "1. Nueva Partida" << std::endl;
        std::cout << "2. Cargar Partida" << std::endl;
        std::cout << "3. Salir" << std::endl;
        std::cout << "Elija una opción: ";
    }



    void startNewGame( int level) {
        if (game != nullptr) {
            delete game; // Liberar la memoria si ya hay un juego en curso
        }
        game = new List(level); // Crear una nueva instancia del juego
    }

    void runGame() {
        while (true) {
            game->printGrid();
            std::cout << std::endl;

            std::cout << "Presione una tecla (W: Arriba, S: Abajo, A: Izquierda, D: Derecha, Q: Salir, R: Repetir): \n";

            char mov = _getch(); // Utilizamos _getch() para capturar una tecla sin necesidad de presionar "Enter"

            if (mov == 'Q' || mov == 'q') {
                currentLevel = 1;
                startNewGame(currentLevel);
                break; // Salir del juego
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
                case 'R':
                case 'r':
                    game->resetLevel();
                    break;
                default:
                    std::cout << "Tecla no válida. Por favor, ingrese W, S, A, D, R o Q." << std::endl;
                    system("pause");
                    system ("cls");
                    continue; // Continuar al siguiente ciclo si la tecla no es válida
            }

            // Mover el personaje
            game->movePlayer(movement);

            system ("cls");
            if (game->goalStack.size() == game->numBoxes) {
                game->printGrid();
                std::cout << "***¡Felicidades! Has ganado el nivel " << currentLevel << ".***" << std::endl;
                if (currentLevel >= 3) {
                    std::cout << "¡Completaste todos los niveles, sos el puto amo lml !" << std::endl;
                    system("pause");
                    currentLevel = 1;
                    return; // Salir del juego
                }
                currentLevel++; // Avanzar al siguiente nivel
                std::cout << "Presione Enter para continuar al siguiente nivel o 'Q' para salir: \n";
                char continueChoice = _getch();
                while (continueChoice != 'Q' && continueChoice != 'q' && continueChoice != 13) {
                    std::cout << "Opción no válida. Por favor, presione Enter o 'Q'." << std::endl;
                    continueChoice = _getch();
                }
                if (continueChoice == 'Q' || continueChoice == 'q') {
                    return; // Salir del juego
                } else {

                    system ("cls");
                    startNewGame(currentLevel); // Cargar el siguiente nivel
                }
            }
            std::cout << std::endl;
        }
    }

};

