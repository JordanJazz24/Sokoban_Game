cmake_minimum_required(VERSION 3.10)
project(Sokoban_Game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Agregar archivos fuente
set(SOURCES
    main.cpp
    Data/List.cpp
)

# Agregar archivos de encabezado
set(HEADERS
    Data/List.h
    Data/Node.h
    GameLogic/GameMenu.h
)

# Crear el ejecutable
add_executable(Sokoban_Game ${SOURCES} ${HEADERS})

# Para Windows, agregar bibliotecas del sistema si es necesario
if(WIN32)
    target_link_libraries(Sokoban_Game)
endif()
