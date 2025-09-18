# 🎮 Sokoban Game - Refactorizado

**Autor:** [JordanJazz24](https://github.com/JordanJazz24)  
**Versión:** 2.0.0 (Refactorizado)  
**Estado:** ✅ Completamente funcional con VS Code + MSYS2  

---

## 🚀 **INICIO RÁPIDO - VS Code + MSYS2:**

### **⚡ Para comenzar inmediatamente:**
1. **Instalar MSYS2** → [https://www.msys2.org/](https://www.msys2.org/)
2. **Abrir terminal MSYS2** → `pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake`
3. **Abrir proyecto en VS Code** → **F5** → ¡Listo!

✨ **Todo está pre-configurado. Solo presiona F5 y el juego se compilará y ejecutará automáticamente.**

---

## 🚀 **¡Proyecto completamente refactorizado!**

Este proyecto ha sido **completamente reorganizado** aplicando principios de ingeniería de software y buenas prácticas de programación.

### 📁 **Nueva Estructura Profesional:**

```
Sokoban_Game/
├── 📁 src/                          # Código fuente
│   ├── main.cpp                     # Punto de entrada
│   ├── 📁 game/                     # Lógica del juego
│   │   ├── Game.h/.cpp             # Coordinador principal
│   │   ├── Level.h/.cpp            # Manejo de niveles
│   │   └── Grid.h/.cpp             # Grid del juego (ex-List)
│   └── 📁 utils/                    # Utilidades
│       ├── Types.h                 # Tipos y enumeraciones
│       ├── Constants.h             # Constantes del juego
│       └── FileManager.h/.cpp      # Manejo de archivos
├── 📁 assets/                       # Recursos
│   ├── levels/                     # Niveles del juego
│   │   ├── level_01.txt
│   │   ├── level_02.txt
│   │   └── level_03.txt
│   └── saves/                      # Partidas guardadas
├── 📁 scripts/                      # Scripts de automatización
├── 📁 docs/                         # Documentación
├── 📁 build/                        # Archivos de compilación
├── 📁 .vscode/                      # Configuración VS Code
└── CMakeLists.txt                   # Configuración CMake moderna
```

---

## 🔧 **Principios Aplicados:**

### **SOLID Principles:**
- **✅ Single Responsibility:** Cada clase tiene una responsabilidad específica
- **✅ Open/Closed:** Extensible sin modificar código existente
- **✅ Liskov Substitution:** Jerarquías bien definidas
- **✅ Interface Segregation:** Interfaces específicas y cohesivas
- **✅ Dependency Inversion:** Dependencias hacia abstracciones

### **Buenas Prácticas:**
- **✅ Separación de responsabilidades**
- **✅ Encapsulación apropiada**
- **✅ Gestión automática de memoria**
- **✅ Documentación completa con Doxygen**
- **✅ Estructura de carpetas profesional**
- **✅ Configuración automática de assets**

---

## 🚀 **Compilación y Ejecución con VS Code + MSYS2:**

### **⚡ Configuración Instantánea:**

#### **1. Prerrequisitos - MSYS2:**
1. **Instalar MSYS2** desde [https://www.msys2.org/](https://www.msys2.org/)
2. **Abrir terminal MSYS2** y ejecutar:
   ```bash
   # Actualizar sistema
   pacman -Syu
   
   # Instalar herramientas de desarrollo
   pacman -S mingw-w64-x86_64-toolchain
   pacman -S mingw-w64-x86_64-cmake
   pacman -S mingw-w64-x86_64-ninja
   pacman -S mingw-w64-x86_64-gdb
   ```

#### **2. Ejecución en VS Code (Método Principal):**
```
🎯 SÚPER FÁCIL - TODO CONFIGURADO AUTOMÁTICAMENTE:
```
1. **Abrir proyecto** en VS Code
2. **Instalar extensiones** (VS Code te preguntará automáticamente)
3. **Presionar F5** → ¡Compila y ejecuta automáticamente!

**Comandos disponibles:**
- **F5**: Compilar + Ejecutar con Debug
- **Ctrl+F5**: Compilar + Ejecutar sin Debug  
- **Ctrl+Shift+P** → "Tasks: Run Build Task": Solo compilar
- **Ctrl+Shift+P** → "MSYS2: Run Game": Solo ejecutar

#### **3. Método PowerShell (Alternativo):**
```powershell
# Script automático completo
.\setup_msys2.bat

# O manualmente paso a paso:
C:\msys64\mingw64\bin\cmake.exe -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
C:\msys64\mingw64\bin\cmake.exe --build build --config Debug -j 4
.\build\Sokoban_Game_debug.exe
```

#### **4. Terminal MSYS2 (Para desarrolladores):**
```bash
# Desde MSYS2 MINGW64 terminal:
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build -j 4
./build/Sokoban_Game.exe
```

---

## ⚙️ **Configuración VS Code - MSYS2:**

### **📁 Archivos Incluidos (Configuración Automática):**

El proyecto incluye configuración **completamente automática** para VS Code:

| Archivo | Propósito |
|---------|-----------|
| `.vscode/settings.json` | Configuración CMake + MSYS2 |
| `.vscode/tasks.json` | Tareas de compilación automatizadas |
| `.vscode/launch.json` | Configuración de debug |
| `.vscode/c_cpp_properties.json` | IntelliSense + Paths de MSYS2 |
| `.vscode/extensions.json` | Extensiones recomendadas |

### **🎯 Tareas Disponibles (Ctrl+Shift+P → Tasks):**

| Tarea | Función | Shortcut |
|-------|---------|----------|
| **MSYS2: Configure CMake** | Configurar proyecto | - |
| **MSYS2: Build Project** | Compilar (tarea por defecto) | **Ctrl+Shift+P** → Build |
| **MSYS2: Build and Setup** | Compilar + copiar assets | - |
| **MSYS2: Run Game** | Ejecutar juego | - |
| **MSYS2: Clean Build** | Limpiar build directory | - |
| **MSYS2: Check Tools** | Verificar herramientas | - |

### **🐛 Debug & Run:**

| Configuración | Propósito | Shortcut |
|---------------|-----------|----------|
| **Debug Sokoban (MSYS2)** | Ejecutar con debugger | **F5** |
| **Run Sokoban (MSYS2)** | Ejecutar sin debugger | **Ctrl+F5** |

### **🔧 Solución de Problemas:**

#### Error: "Herramientas no encontradas"
```powershell
# Verificar instalación MSYS2:
C:\msys64\mingw64\bin\gcc.exe --version
C:\msys64\mingw64\bin\cmake.exe --version

# Si faltan, instalar en MSYS2:
pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake
```

#### Error: "Assets no encontrados"  
```powershell
# Los assets se copian automáticamente, pero si hay problemas:
Copy-Item -Recurse -Force "assets" "build/assets"
```

#### Rebuild completo:
```powershell
# Limpiar y recompilar:
Remove-Item -Recurse -Force "build"; .\setup_msys2.bat
```

---

## 🎯 **Mejoras Implementadas:**

### **Arquitectura:**
- **Modularización completa** del código
- **Separación clara** entre lógica de juego, UI y datos
- **Gestión inteligente de archivos** con múltiples rutas de búsqueda
- **Sistema robusto** de manejo de errores

### **Funcionalidad:**
- **Copia automática de assets** durante compilación
- **Búsqueda inteligente** de archivos de niveles
- **Sistema mejorado** de guardado/carga
- **Replay system** con timing visual

### **Desarrollo:**
- **IntelliSense completo** configurado
- **Debugging** completamente funcional
- **Tasks automatizadas** para compilación
- **Documentación técnica** completa

---

## 🎮 **Controles del Juego:**

| Tecla | Acción |
|-------|--------|
| **W/A/S/D** | Mover jugador |
| **R** | Reiniciar nivel |
| **G** | Guardar partida |
| **Q** | Salir |

---

## 🔧 **Características Técnicas:**

### **Desarrollo:**
- **C++17** con características modernas
- **CMake** para gestión de build multiplataforma
- **MSYS2 MinGW-w64** como toolchain principal
- **VS Code** completamente configurado con:
  - IntelliSense automático
  - Debug integrado (GDB)
  - Tareas de build automatizadas
  - Configuración MSYS2 pre-establecida

### **Sistema:**
- **Gestión automática de memoria** sin memory leaks
- **Assets automáticos** copiados durante compilación
- **Multiplataforma** (Windows/MSYS2 configurado)
- **Build system robusto** con detección de errores

### **Calidad de Código:**
- **Documentación Doxygen** completa
- **Principios SOLID** aplicados
- **Separación clara de responsabilidades**
- **Testing preparado** para expansión futura

---

## 📈 **Comparación: Antes vs Después**

| Aspecto | Antes | Después |
|---------|-------|---------|
| **Archivos de código** | 5 archivos mezclados | 10+ archivos organizados |
| **Responsabilidades** | Mezcladas | Claramente separadas |
| **Mantenibilidad** | Difícil | Excelente |
| **Extensibilidad** | Limitada | Altamente extensible |
| **Testabilidad** | Compleja | Fácil testing modular |
| **Documentación** | Básica | Profesional completa |
| **Build system** | Manual | Completamente automatizado |

---

## ✅ **Estado de Verificación:**

- **✅ Compilación exitosa** con nueva arquitectura
- **✅ Ejecución funcional** de todos los niveles
- **✅ Assets automáticos** copiándose correctamente
- **✅ Guardado/carga** funcionando
- **✅ Todas las funcionalidades** preservadas
- **✅ Configuración VS Code** actualizada
- **✅ Documentación** completamente actualizada

---

**🎯 El proyecto ahora sigue estándares profesionales de desarrollo y está preparado para expansión futura manteniendo excelente calidad de código.**
