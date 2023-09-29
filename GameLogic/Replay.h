//
// Created by jorda on 8/30/2023.
//

#ifndef PROYECTO_SOKOBAN_REPLAY_H
#define PROYECTO_SOKOBAN_REPLAY_H

#include <vector>

class Replay {
private:
    std::vector<char> moves;  // Vector para almacenar los movimientos

public:
    void addMove(char move);   // Agregar un movimiento al vector
    const std::vector<char>& getMoves() const;  // Obtener el vector de movimientos
};



#endif //PROYECTO_SOKOBAN_REPLAY_H
