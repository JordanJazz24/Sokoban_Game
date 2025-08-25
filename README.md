# 🎮 Sokoban Game - Refactorizado

**Autor:** [JordanJazz24](https://github.com/JordanJazz24)  
**Versión:** 2.0.0 (Refactorizado)  
**Estado:** ✅ Completamente funcional  

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

## 🚀 **Compilación y Ejecución:**

### **Método Rápido (VS Code):**
1. Abrir el proyecto en VS Code
2. **F5** o **Ctrl+Shift+P** → "Tasks: Run Build Task"
3. ¡Listo! El juego se compila y ejecuta automáticamente

### **Método Terminal:**
```bash
# Configurar y compilar
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
cmake --build .

# Ejecutar (assets se copian automáticamente)
./Sokoban_Game.exe
```

### **Scripts Automatizados:**
```bash
# Desde el directorio raíz:
scripts/setup_and_run.bat  # Compilar y ejecutar todo automáticamente
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

- **C++17** con características modernas
- **CMake** para gestión de build
- **Gestión automática de memoria** sin leaks
- **Assets automáticos** copiados al build
- **Multiplataforma** (Windows configurado, fácil extensión)
- **Documentación Doxygen** integrada

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
