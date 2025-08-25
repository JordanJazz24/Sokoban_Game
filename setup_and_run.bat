@echo off
echo ================================
echo  Sokoban Game - Compilar y Ejecutar
echo ================================
echo.

call setup.bat
if %ERRORLEVEL% neq 0 (
    echo ❌ Error en la configuración
    pause
    exit /b 1
)

echo.
echo Iniciando juego...
echo ================================
echo.
cd build
Sokoban_Game.exe

echo.
echo Juego terminado. Presiona cualquier tecla para salir.
pause >nul
