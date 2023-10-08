#include <iostream>
#include <Windows.h>
#include <conio.h> // Para _getch()
#include "SDL.h"
#include <SDL_mixer.h>

#include "../Data/List.h"
struct ReplayMove {
    Movement movement;
    // Agrega cualquier otra información necesaria aquí
};

class GameMenu {
public:
    GameMenu() : game(nullptr), currentLevel(1){
        movements = new vector<Movement>();
    }

    void run() {
        while (true) {
            displayMenu();

            char choice = _getch(); // Capturar la elección del usuario

            switch (choice) {
                case '1': // Nueva partida
                    startNewGame(currentLevel);
                    break;
                case '2': // Cargar partida (debes implementar esta funcionalidad)
                    loadGame();
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
    vector<Movement> *movements;

    void displayMenu() {
        system("cls"); // Limpiar la consola
        std::cout << "Bienvenido a Sokoban MIEDO!!" << std::endl;
        std::cout << "1. Nueva Partida" << std::endl;
        std::cout << "2. Cargar Partida" << std::endl;
        std::cout << "3. Salir" << std::endl;
        std::cout << "Elija una opción: ";
    }

    void replay(){
        startNewGame(currentLevel);
      for (int i = 0; i < movements->size(); ++i) {
          game->movePlayer(movements->at(i));
          Sleep(100);
          system("cls");
          game->printGrid();
      }
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

            std::cout << "Presione una tecla (W: Arriba, S: Abajo, A: Izquierda, D: Derecha, Q: Salir, R: Repetir, G: Guardar partida): \n";
            char mov = _getch(); // Utilizamos _getch() para capturar una tecla sin necesidad de presionar "Enter"

            if (mov == 'Q' || mov == 'q') {
                currentLevel = 1;
                startNewGame(currentLevel);
                break; // Salir del juego
            }

            if (mov == 'G' || mov == 'g') {
                saveGame(); // Guardar la partida
                system("cls");
                std::cout << "Partida guardada. Presione Enter para continuar." << std::endl;
                _getch();
                system("cls");
                continue;
            }
            Movement movement;

            // Convertir la tecla ingresada en un movimiento válido
            switch (mov) {
                case 'W':
                case 'w':
                    movement = UP;
                    movements->push_back(movement);

                    break;
                case 'S':
                case 's':
                    movement = DOWN;
                    movements->push_back(movement);

                    break;
                case 'A':
                case 'a':
                    movement = LEFT;
                    movements->push_back(movement);

                    break;
                case 'D':
                case 'd':
                    movement = RIGHT;
                    movements->push_back(movement);

                    break;
                case 'R':
                case 'r':
                    game->  resetLevel(currentLevel);
                    if (!movements->empty()) {
                        movements->clear();
                    }
                    break;
                default:
                    std::cout << "Tecla no válida. Por favor, ingrese W, S, A, D, R o Q." << std::endl;
                    system("pause");
                    system ("cls");
                    continue; // Continuar al siguiente ciclo si la tecla no es válida
            }

            if(mov != 'R' && mov != 'r'){
                game->movePlayer(movement);
            }
            else{
                system ("cls");
                continue;
            }


            system ("cls");
            if (game->goalStack->size() == game->numBoxes) {
                game->printGrid();
                std::cout << "***¡Felicidades! Has ganado el nivel " << currentLevel << ".***" << std::endl;
                if (currentLevel >= 3) {
                    std::cout << "¡Completaste todos los niveles, sos el puto amo lml !" << std::endl;
                    std::cout << "Presione Enter para continuar al reiniciar el juego o 'R' para ver la repeticion del ultimo nivel: \n";
                    system("pause");
                }else {
                    std::cout << "Presione Enter para continuar al siguiente nivel o 'Q' para salir: \n";
                    std::cout << "--Si deseA ver una repeticion presione 'R'--\n";
                }



                char continueChoice = _getch();
                while(continueChoice != 'R' && continueChoice != 'r' && continueChoice != 'Q' && continueChoice != 'q' && continueChoice != 13){
                    if (currentLevel >= 3) {
                        std::cout << "Opción no válida. Por favor, ingrese R o presione ENTER para reinciiar el juego\n";}
                    else {
                        std::cout << "Opción no válida. Por favor, ingrese R o Q, o  presion ENTER" << std::endl;
                    }
                    continueChoice = _getch();
                }

                while (continueChoice == 'R' || continueChoice == 'r'){
                    replay();
                    if (currentLevel >= 3) {
                        std::cout << "¡Completaste todos los niveles, sos el puto amo lml !" << std::endl;
                        std::cout << "Presione Enter para reiniciar el juego 'R' volver a ver la Repeticion: \n";
                    }else {
                        std::cout << "Presione Enter para continuar al siguiente nivel o 'Q' para salir: \n";
                        std::cout << "--Si deseA ver una repeticion presione 'R'--\n";
                    }

                    continueChoice = _getch();
                    while(continueChoice != 'R' && continueChoice != 'r' && continueChoice != 'Q' && continueChoice != 'q' && continueChoice != 13){
                        if (currentLevel >= 3) {
                            std::cout << "Opción no válida. Por favor, ingrese R o presione ENTER para reinciiar el juego\n";}
                        else {
                            std::cout << "Opción no válida. Por favor, ingrese R o Q, o  presion ENTER" << std::endl;
                        }
                        continueChoice = _getch();
                    }
                }

                if (continueChoice == 'Q' || continueChoice == 'q') {
                    currentLevel = 1;
                    startNewGame(currentLevel);// reiniciar el juego
                    movements->clear();
                    return; // Salir del juego
                }
                else{
                    if (currentLevel >= 3) {
                        currentLevel = 1;
                        startNewGame(currentLevel);// reiniciar el juego
                        movements->clear();
                        return; // Salir del juego
                    }
                    currentLevel++; // Avanzar al siguiente nivel
                    movements->clear();
                    system ("cls");
                    startNewGame(currentLevel); }// Crear una nueva instancia del juego
            }//
            std::cout << std::endl;
        }
    }

    void saveGame() {
        // Abrir el archivo de guardado en modo escritura (esto lo creará o sobrescribirá)
        std::ofstream saveFile("partida.txt", std::ios::trunc);

        if (saveFile.is_open()) {
            // Guardar el nivel actual en el archivo
            saveFile << currentLevel << std::endl;

            // Guardar la lista de movimientos en el archivo
            for (const Movement& movement : *movements) {
                saveFile << static_cast<int>(movement) << " ";
            }

            saveFile.close();

            std::cout << "Partida guardada exitosamente." << std::endl;
        } else {
            std::cout << "Error al guardar la partida." << std::endl;
        }
    }


    void loadGame() {
        // Abrir el archivo de guardado en modo lectura
        std::ifstream saveFile("partida.txt");

        if (saveFile.is_open()) {
            // Leer el nivel actual desde el archivo
            saveFile >> currentLevel;

            // Limpiar la lista de movimientos
            movements->clear();

            // Leer la lista de movimientos desde el archivo
            int movement;
            while (saveFile >> movement) {
                movements->push_back(static_cast<Movement>(movement));
            }

            saveFile.close();

            // Crear una nueva instancia del juego con el nivel cargado
            startNewGame(currentLevel);
            for (int i = 0; i < movements->size(); ++i) {
                game->movePlayer(movements->at(i));
            }

            std::cout << "Partida cargada exitosamente." << std::endl;
        } else {
            std::cout << "No se encontró una partida guardada." << std::endl;
        }
    }


};

