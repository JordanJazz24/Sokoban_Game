@echo off
echo ====================================
echo    EJECUTANDO SOKOBAN GAME
echo ====================================
echo.

REM Cambiar al directorio del proyecto
cd /d "d:\Program Files\GitHub Repos\Sokoban_Game"

REM Compilar el proyecto si es necesario
echo Compilando el proyecto...
cmake --build build
if errorlevel 1 (
    echo.
    echo ERROR: No se pudo compilar el proyecto
    pause
    exit /b 1
)

echo.
echo ====================================
echo    INICIANDO JUEGO...
echo ====================================
echo.

REM Ejecutar el juego
.\build\Sokoban_Game.exe

echo.
echo ====================================
echo    JUEGO TERMINADO
echo ====================================
echo.
pause