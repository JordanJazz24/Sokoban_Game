@echo off
echo ================================
echo    Sokoban Game - Setup Script
echo ================================
echo.

echo [1/4] Creando directorio build...
if not exist "build" (
    mkdir build
    echo ✓ Directorio build creado
) else (
    echo ✓ Directorio build ya existe
)

echo.
echo [2/4] Configurando proyecto con CMake...
cmake -G "MinGW Makefiles" -B build -S .
if %ERRORLEVEL% neq 0 (
    echo ❌ Error al configurar con CMake
    echo Asegurate de que CMake y MinGW esten instalados
    pause
    exit /b 1
)
echo ✓ Configuración exitosa

echo.
echo [3/4] Compilando proyecto...
cmake --build build
if %ERRORLEVEL% neq 0 (
    echo ❌ Error al compilar
    pause
    exit /b 1
)
echo ✓ Compilación exitosa

echo.
echo [4/4] Copiando archivos de niveles...
copy "Nivel*.txt" "build\" >nul 2>&1
echo ✓ Archivos de niveles copiados

echo.
echo ================================
echo      ¡Setup completado! 
echo ================================
echo.
echo Para ejecutar el juego:
echo   cd build
echo   Sokoban_Game.exe
echo.
echo O ejecuta: setup_and_run.bat
echo.
pause
