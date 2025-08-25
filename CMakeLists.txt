cmake_minimum_required(VERSION 3.10)
project(Sokoban_Game VERSION 1.0.0 LANGUAGES CXX)

# Configurar estándar C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configurar directorios
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Incluir directorios
include_directories(${SRC_DIR})

# Archivos fuente organizados por módulos
set(GAME_SOURCES
    ${SRC_DIR}/game/Game.cpp
    ${SRC_DIR}/game/Level.cpp
    ${SRC_DIR}/game/Grid.cpp
)

set(UTILS_SOURCES
    ${SRC_DIR}/utils/FileManager.cpp
)

set(MAIN_SOURCE
    ${SRC_DIR}/main.cpp
)

# Todos los archivos fuente
set(ALL_SOURCES
    ${MAIN_SOURCE}
    ${GAME_SOURCES}
    ${UTILS_SOURCES}
)

# Crear ejecutable
add_executable(${PROJECT_NAME} ${ALL_SOURCES})

# Configurar propiedades del ejecutable
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "Sokoban_Game"
    DEBUG_POSTFIX "_debug"
)

# Configuración específica para Windows
if(WIN32)
    # Agregar definiciones para Windows
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
    
    # Copiar assets automáticamente después de la compilación
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
        COMMENT "Copiando assets al directorio de salida..."
    )
endif()

# Configuración de Debug/Release
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
    message(STATUS "Configuración: Debug")
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
    message(STATUS "Configuración: Release")
endif()

# Mostrar información de compilación
message(STATUS "Compilando Sokoban Game v${PROJECT_VERSION}")
message(STATUS "Estándar C++: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compilador: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Directorio de salida: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# Opción para habilitar warnings adicionales
option(ENABLE_WARNINGS "Habilitar warnings adicionales" ON)

if(ENABLE_WARNINGS)
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()
