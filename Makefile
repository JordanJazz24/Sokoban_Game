# Makefile para Sokoban Game - Estructura Refactorizada
# Uso: make, make clean, make run

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
TARGET = Sokoban_Game
BUILD_DIR = build
SRC_DIR = src

# Archivos fuente organizados
GAME_SOURCES = $(SRC_DIR)/game/Game.cpp $(SRC_DIR)/game/Level.cpp $(SRC_DIR)/game/Grid.cpp
UTILS_SOURCES = $(SRC_DIR)/utils/FileManager.cpp
MAIN_SOURCE = $(SRC_DIR)/main.cpp
ALL_SOURCES = $(MAIN_SOURCE) $(GAME_SOURCES) $(UTILS_SOURCES)

# Archivos de encabezado para dependencias
HEADERS = $(wildcard $(SRC_DIR)/**/*.h) $(wildcard $(SRC_DIR)/*.h)

# Configuración por defecto
.PHONY: all cmake-build clean run setup help

all: cmake-build

# Compilación usando CMake (recomendado)
cmake-build:
	@echo "🔧 Configurando con CMake..."
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
	@cd $(BUILD_DIR) && cmake -G "MinGW Makefiles" ..
	@echo "🏗️  Compilando con CMake..."
	@cd $(BUILD_DIR) && cmake --build .
	@echo "✅ Compilación completada: $(BUILD_DIR)/$(TARGET).exe"

# Compilación directa (alternativa)
direct-build: $(BUILD_DIR)/$(TARGET).exe

$(BUILD_DIR)/$(TARGET).exe: $(ALL_SOURCES) $(HEADERS)
	@echo "🏗️  Compilación directa..."
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
	@if not exist "$(BUILD_DIR)\assets" mkdir "$(BUILD_DIR)\assets"
	@if not exist "$(BUILD_DIR)\assets\levels" mkdir "$(BUILD_DIR)\assets\levels"
	@if not exist "$(BUILD_DIR)\assets\saves" mkdir "$(BUILD_DIR)\assets\saves"
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/$(TARGET).exe $(ALL_SOURCES) -I$(SRC_DIR)
	@echo "📁 Copiando assets..."
	@copy "assets\levels\*.txt" "$(BUILD_DIR)\assets\levels\" >NUL 2>&1
	@copy "assets\saves\.gitkeep" "$(BUILD_DIR)\assets\saves\" >NUL 2>&1
	@echo "✅ Compilación directa completada: $(BUILD_DIR)/$(TARGET).exe"

# Ejecutar el juego
run: cmake-build
	@echo "🎮 Ejecutando $(TARGET)..."
	@cd $(BUILD_DIR) && ./$(TARGET).exe

# Limpiar archivos de compilación
clean:
	@echo "🧹 Limpiando archivos de compilación..."
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"
	@echo "✅ Limpieza completada"

# Configuración inicial completa
setup: clean cmake-build
	@echo "✅ Setup completado. Usa 'make run' para ejecutar."

# Test de compilación sin ejecutar
test: cmake-build
	@echo "✅ Test de compilación exitoso"

# Verificar estructura del proyecto
check:
	@echo "📋 Verificando estructura del proyecto..."
	@echo "Archivos fuente encontrados:"
	@if exist "$(SRC_DIR)\main.cpp" echo "  ✅ $(SRC_DIR)\main.cpp"
	@if exist "$(SRC_DIR)\game\Game.cpp" echo "  ✅ $(SRC_DIR)\game\Game.cpp"
	@if exist "$(SRC_DIR)\game\Level.cpp" echo "  ✅ $(SRC_DIR)\game\Level.cpp"
	@if exist "$(SRC_DIR)\game\Grid.cpp" echo "  ✅ $(SRC_DIR)\game\Grid.cpp"
	@if exist "$(SRC_DIR)\utils\FileManager.cpp" echo "  ✅ $(SRC_DIR)\utils\FileManager.cpp"
	@echo "Assets encontrados:"
	@if exist "assets\levels\level_01.txt" echo "  ✅ assets\levels\level_01.txt"
	@if exist "assets\levels\level_02.txt" echo "  ✅ assets\levels\level_02.txt"
	@if exist "assets\levels\level_03.txt" echo "  ✅ assets\levels\level_03.txt"

# Ayuda
help:
	@echo "📖 Comandos disponibles:"
	@echo "  make              - Compilar con CMake (recomendado)"
	@echo "  make cmake-build  - Compilar usando CMake"
	@echo "  make direct-build - Compilar directamente con g++"
	@echo "  make run          - Compilar y ejecutar"
	@echo "  make clean        - Limpiar archivos de compilación"
	@echo "  make setup        - Configuración inicial completa"
	@echo "  make test         - Test de compilación"
	@echo "  make check        - Verificar estructura del proyecto"
	@echo "  make help         - Mostrar esta ayuda"
