# 🛠️ Guía de Desarrollo - Sokoban Game

## 📋 Configuración Automática Completa

Este proyecto incluye **configuración automática completa** para VS Code. ¡Solo abre el proyecto y todo funcionará!

### 🚀 Configuración Instantánea

#### Para usuarios de VS Code:
1. **Abrir el proyecto** en VS Code
2. **Instalar extensiones recomendadas** (VS Code te preguntará automáticamente)
3. **Presionar F5** o usar **Ctrl+Shift+P** → "Tasks: Run Build Task"
4. **¡Listo!** El juego se compilará y ejecutará automáticamente

#### Para usuarios de terminal:
```bash
# Opción 1: Script automático (Windows)
./setup_and_run.bat

# Opción 2: CMake manual
cmake -G "MinGW Makefiles" -B build -S .
cmake --build build
copy "Nivel*.txt" "build\"
./build/Sokoban_Game.exe

# Opción 3: Makefile
make run
```

---

## 📁 Archivos de Configuración Incluidos

### `.vscode/tasks.json` - Tareas Automatizadas
| Tarea | Función | Dependencias |
|-------|---------|--------------|
| `CMake: Configure` | Configurar proyecto | - |
| `CMake: Build` | **Compilar** (tarea principal) | Configure |
| `CMake: Clean Build` | Limpiar compilación | - |
| `Setup: Copy Level Files` | Copiar niveles a build/ | - |
| `CMake: Build and Setup` | Compilar + copiar archivos | Build + Copy |
| `Run: Sokoban Game` | **Ejecutar juego** | Build and Setup |
| `Full Setup and Run` | **Todo automático** | Todas las anteriores |

### `.vscode/launch.json` - Configuración de Depuración
- **Debug Sokoban Game**: Ejecutar con depurador
- **Run Sokoban Game**: Ejecutar sin depurador
- Automáticamente compila antes de ejecutar

### `.vscode/settings.json` - Configuración del Workspace
- Configuración de CMake automática
- Asociaciones de archivos C++
- Configuración de IntelliSense

### `.vscode/c_cpp_properties.json` - IntelliSense
- Paths de include configurados
- Estándar C++17
- Configuración de compilador MinGW

### `.vscode/extensions.json` - Extensiones Recomendadas
- C/C++ Extension Pack
- CMake Tools
- Configuración automática

---

## 🎯 Comandos Rápidos

### En VS Code:
- **F5**: Compilar y ejecutar con depurador
- **Ctrl+F5**: Compilar y ejecutar sin depurador
- **Ctrl+Shift+P** → "Tasks: Run Build Task": Solo compilar
- **Ctrl+Shift+P** → "Full Setup and Run": Configurar, compilar y ejecutar

### En Terminal:
```bash
# Setup completo
./setup_and_run.bat

# Solo compilar
cmake --build build

# Solo ejecutar (si ya está compilado)
./build/Sokoban_Game.exe
```

---

## 🔧 Estructura de Compilación

```
Flujo de Compilación Automático:
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│ CMake Configure │ -> │  CMake Build    │ -> │  Copy Files     │
│ (MinGW)         │    │  (C++17)        │    │  (Nivel*.txt)   │
└─────────────────┘    └─────────────────┘    └─────────────────┘
                                                       │
                                                       v
                                             ┌─────────────────┐
                                             │   Run Game      │
                                             │ (build/exe)     │
                                             └─────────────────┘
```

---

## 🐛 Solución de Problemas

### Error: "CMake not found"
```bash
# Verificar instalación
cmake --version

# Si no está instalado, descargar de: https://cmake.org/
```

### Error: "g++ not found"
```bash
# Verificar MinGW
g++ --version

# Si no está instalado, descargar MinGW-w64
```

### Error: "No se encuentran archivos de niveles"
```bash
# Los archivos se copian automáticamente, pero si hay problemas:
copy "Nivel*.txt" "build\"
```

### Rebuild completo:
```bash
# Limpiar todo y recompilar
./setup.bat

# O manualmente:
rmdir /s build
cmake -G "MinGW Makefiles" -B build -S .
cmake --build build
```

---

## 📝 Modificar y Extender

### Agregar nuevos archivos .cpp:
1. Agregar a `CMakeLists.txt` en la sección `SOURCES`
2. Las tareas automáticamente lo incluirán

### Agregar nuevos niveles:
1. Crear `Nivel N.txt` siguiendo el formato
2. Se copiará automáticamente al compilar

### Cambiar configuración de compilación:
- Modificar `.vscode/tasks.json` para ajustar flags
- Modificar `CMakeLists.txt` para opciones de CMake

---

## ✅ Testing Automatizado

El proyecto incluye verificación automática de:
- ✅ Compilación exitosa
- ✅ Enlazado correcto
- ✅ Archivos de niveles disponibles
- ✅ Ejecutable funcional

---

**🎯 Todo está configurado para que funcione inmediatamente. ¡Solo abre VS Code y presiona F5!**
