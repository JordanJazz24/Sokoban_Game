# Makefile para Sokoban Game
# Uso: make, make clean, make run

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
TARGET = Sokoban_Game
BUILD_DIR = build
SOURCES = main.cpp Data/List.cpp
HEADERS = Data/List.h Data/Node.h GameLogic/GameMenu.h

# Configuración por defecto
.PHONY: all cmake-build clean run setup help

all: cmake-build

# Compilación usando CMake (recomendado)
cmake-build:
	@echo "Configurando con CMake..."
	@cmake -G "MinGW Makefiles" -B $(BUILD_DIR) -S .
	@echo "Compilando con CMake..."
	@cmake --build $(BUILD_DIR)
	@echo "Copiando archivos de niveles..."
	@powershell -Command "Copy-Item 'Nivel*.txt' '$(BUILD_DIR)/' -Force" 2>NUL || copy "Nivel*.txt" "$(BUILD_DIR)\" >NUL 2>&1
	@echo "✓ Compilación completada: $(BUILD_DIR)/$(TARGET).exe"

# Compilación directa (alternativa)
direct-build: $(BUILD_DIR)/$(TARGET).exe

$(BUILD_DIR)/$(TARGET).exe: $(SOURCES) $(HEADERS)
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/$(TARGET).exe $(SOURCES) -I.
	@powershell -Command "Copy-Item 'Nivel*.txt' '$(BUILD_DIR)/' -Force" 2>NUL || copy "Nivel*.txt" "$(BUILD_DIR)\" >NUL 2>&1
	@echo "✓ Compilación directa completada: $(BUILD_DIR)/$(TARGET).exe"

# Ejecutar el juego
run: cmake-build
	@echo "Ejecutando $(TARGET)..."
	@cd $(BUILD_DIR) && ./$(TARGET).exe

# Limpiar archivos de compilación
clean:
	@echo "Limpiando archivos de compilación..."
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"
	@echo "✓ Limpieza completada"

# Configuración inicial
setup: clean cmake-build
	@echo "✓ Setup completado. Usa 'make run' para ejecutar."

# Ayuda
help:
	@echo "Comandos disponibles:"
	@echo "  make              - Compilar con CMake (recomendado)"
	@echo "  make cmake-build  - Compilar usando CMake"
	@echo "  make direct-build - Compilar directamente con g++"
	@echo "  make run          - Compilar y ejecutar"
	@echo "  make clean        - Limpiar archivos de compilación"
	@echo "  make setup        - Configuración inicial completa"
	@echo "  make help         - Mostrar esta ayuda"
