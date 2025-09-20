# 😈 Briar Engine

> ⚠️ **PROYECTO EN DESARROLLO TEMPRANO** - Motor gráfico experimental para aprendizaje

Un motor gráfico básico desarrollado en C++ con OpenGL, creado principalmente con fines educativos y experimentación con gráficos 3D. Este proyecto está en sus primeras etapas de desarrollo y las funcionalidades son bastante básicas.

![OpenGL](https://img.shields.io/badge/OpenGL-3.3+-blue.svg)
![C++](https://img.shields.io/badge/C++-20-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20macOS%20%7C%20Linux-green.svg)
![Status](https://img.shields.io/badge/Status-Early%20Development-orange.svg)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)

## ✨ Características Actuales (Básicas)

### 🎨 Renderizado
- **Motor OpenGL 3.3** con funcionalidad básica de renderizado
- **Shaders simples** vertex y fragment básicos incluidos
- **Iluminación básica** con soporte para una luz (implementación en progreso para múltiples luces)
- **Carga de modelos OBJ** básica (sin soporte completo para materiales)
- **Sistema de texturas** fundamental
- **Framebuffer básico** para renderizado off-screen

### 🏗️ Arquitectura Básica
- **Clases básicas** para objetos (Object, Mesh, Light)
- **Cámara 3D simple** con controles básicos de primera persona
- **Sistema de mallas** con carga de modelos OBJ
- **Gestión básica de materiales** (en desarrollo)

### 🖥️ Interfaz de Usuario (ImGui)
- **Editor básico** con paneles ImGui
- **Inspector simple** para propiedades básicas de objetos
- **Explorador de archivos** integrado (funcionalidad básica)
- **Vista de escena** con lista de objetos
- **Estadísticas básicas** (FPS, posición de cámara)
- **Controles de iluminación** ambiente básicos

### 📊 Implementación Técnica
- **VAO/VBO/EBO** para gestión de geometría OpenGL
- **Smart pointers** para gestión básica de memoria
- **GLFW** para ventanas y eventos
- **Compilación con Makefile** para múltiples plataformas

## ⚠️ Limitaciones Actuales

- **Sistema de iluminación incompleto** - Solo soporte básico para una luz
- **Carga de materiales limitada** - MTL parsing en desarrollo
- **Sin sistema de física** 
- **Sin animaciones**
- **Sin sistema de audio**
- **Sin optimizaciones avanzadas** (culling, batching, etc.)
- **Interfaz de usuario básica** - Funcionalidad limitada en el editor
- **Sin soporte para formatos modernos** como GLTF
- **Documentación del código limitada**

## 🚀 Instalación y Configuración

### Prerequisitos

#### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install build-essential cmake pkg-config
sudo apt-get install libglfw3-dev libgl1-mesa-dev
sudo apt-get install libglm-dev
```

#### macOS
```bash
# Usando Homebrew
brew install glfw glm assimp
```

#### Windows (MSYS2/MinGW)
```bash
pacman -S --noconfirm --needed mingw-w64-x86_64-toolchain
pacman -S --noconfirm --needed mingw-w64-x86_64-glfw
```

### Dependencias Incluidas
El motor incluye las siguientes librerías como parte del código fuente:
- **ImGui** (1.90+) - Interfaz gráfica inmediata
- **GLAD** - Loader de funciones OpenGL
- **stb_image** - Carga de texturas
- **nlohmann/json** - Parsing JSON
- **tiny_obj_loader** - Carga de modelos OBJ
- **GLM** - Matemáticas 3D (headers solamente)

### Compilación

1. **Clonar el repositorio:**
```bash
git clone https://github.com/PILTRAFILLA317/Briar_Engine.git
cd Briar_Engine
```

2. **Compilar con Make:**
```bash
make
```

3. **Ejecutar el motor:**
```bash
./BriarEngine
```

4. **Limpiar archivos objeto:**
```bash
make clean
```

## 🎮 Uso Básico del Motor

### Controles de Cámara
- **Clic derecho + movimiento del mouse**: Rotar cámara (funcionalidad básica)
- **WASD**: Mover cámara
- **ESC**: Cerrar aplicación

### Interfaz del Editor (Básica)

#### Menú Principal
- **Objects** > Create Empty, Create Light (funcionalidad limitada)
- **Window** > Paneles básicos

#### Paneles Disponibles
- **Engine Stats**: FPS y posición de cámara básica
- **Object List**: Lista simple de objetos en la escena
- **Properties**: Propiedades básicas del objeto seleccionado (limitado)
- **File System**: Explorador básico de archivos
- **Scene/Game View**: Vistas de renderizado básicas

> **Nota**: Muchas funciones del menú y paneles están en desarrollo o tienen funcionalidad limitada.

### Carga de Modelos (Básica)
```cpp
// Funcionalidad básica - solo algunos modelos OBJ simples
engine.AddOBJModel("MiModelo", "assets/models/modelo.obj");
```

### Creación de Luces (En Desarrollo)
```cpp
// Funcionalidad básica de luces
Light* light = new Light("Mi Luz", Light::POINT, glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
engine.AddObject(std::shared_ptr<Object>(light));
```

## 🏗️ Arquitectura Básica del Código

### Estructura de Directorios
```
Briar_Engine/
├── Engine/              # Código fuente del motor
│   ├── src/            # Implementaciones (.cpp)
│   ├── includes/       # Headers (.hpp)
│   ├── glad/          # Loader OpenGL
│   ├── glfw/          # Biblioteca de ventanas
│   ├── imgui/         # Interfaz gráfica
│   ├── stb/           # Utilidades de imagen
│   └── json/          # Parser JSON
├── assets/             # Recursos básicos
│   ├── shaders/       # Shaders GLSL básicos
│   ├── textures/      # Algunas texturas de ejemplo
│   └── models/        # Modelos 3D de prueba
└── Makefile           # Sistema de compilación simple
```

### Clases Principales (Implementación Básica)

#### Engine
Clase principal con funcionalidad básica:
- Inicialización básica de OpenGL, GLFW e ImGui
- Ciclo de renderizado simple
- Gestión básica de eventos
- Coordinación básica entre sistemas

#### Camera
Sistema básico de cámara 3D:
```cpp
class Camera {
    glm::vec3 Position;
    glm::vec3 Orientation;
    glm::vec3 Up;
    float speed;        // Velocidad básica
    float sensitivity;  // Sensibilidad básica
};
```

#### Object
Clase base simple para objetos:
```cpp
class Object {
    std::string name;
    std::string type;
    glm::vec3 position;    // Posición básica
    glm::vec3 rotation;    // Rotación básica
    glm::vec3 scale;       // Escala básica
};
```

#### Mesh
```cpp
class Mesh : public Object {
    std::vector<SubMesh> subMeshes;  // Implementación básica
    void Draw(Shader& shader, Camera& camera);  // Funcionalidad básica
};
```

#### Light
Sistema básico de iluminación (en desarrollo):
```cpp
class Light : public Object {
    enum LightType { DIRECTIONAL, POINT, SPOT };  // Tipos definidos pero implementación básica
    glm::vec3 color;
    float intensity;
    float range;  // No completamente implementado
};
```

### Sistema de Shaders (Básico)
El motor incluye shaders GLSL básicos:

**Vertex Shader** (`defaultVert.glsl`):
- Transformación básica de vértices
- Cálculo de posición simple
- Preparación básica para iluminación

**Fragment Shader** (`defaultFrag.glsl`):
- Iluminación básica (implementación simplificada)
- Soporte limitado para texturas
- Una sola luz por escena actualmente

## 🛠️ Desarrollo y Extensión (Para Desarrolladores)

> **Nota**: Este proyecto está abierto a contribuciones y mejoras. La implementación actual es básica y hay mucho espacio para mejoras.

### Áreas Que Necesitan Desarrollo
1. **Sistema de iluminación** - Completar soporte para múltiples luces
2. **Carga de materiales** - Implementar parsing completo de MTL
3. **Editor mejorado** - Expandir funcionalidad de ImGui
4. **Optimizaciones** - Implementar técnicas básicas de optimización
5. **Documentación** - Mejorar comentarios en el código

### Para Contribuir
1. Heredar de la clase `Object` para nuevos tipos
2. Implementar métodos `Draw()` básicos
3. Seguir la estructura existente del código

## 🐛 Problemas Conocidos

### Errores Comunes y Limitaciones

**Funcionalidad limitada:**
- Muchas características están en desarrollo inicial
- El sistema de iluminación solo soporta una luz básica
- La interfaz de usuario tiene funcionalidad mínima

**Errores de compilación con OpenGL:**
- Verificar que GLFW esté instalado correctamente
- Asegurar que los drivers de OpenGL estén actualizados

**Modelos no se cargan correctamente:**
- Solo funciona con modelos OBJ simples
- El soporte para materiales MTL es limitado
- Verificar que las rutas de archivos sean correctas

**Interfaz ImGui no responde:**
- Funcionalidad básica implementada
- Algunos botones/menús pueden no tener funcionalidad completa

## 🚧 Estado del Proyecto y Roadmap

### Versión Actual: Pre-Alpha
- ✅ Renderizado básico OpenGL
- ✅ Carga básica de modelos OBJ  
- ✅ Cámara 3D simple
- ✅ Interfaz ImGui básica
- ✅ Sistema básico de objetos

### Próximas Mejoras Planificadas
- [ ] Completar sistema de múltiples luces
- [ ] Mejorar carga de materiales MTL
- [ ] Expandir funcionalidad del editor
- [ ] Añadir más tipos de primitivas
- [ ] Mejorar documentación del código
- [ ] Sistema básico de física
- [ ] Soporte para más formatos de modelo

## 🤝 Contribución

Este es un **proyecto de aprendizaje** y las contribuciones son muy bienvenidas, especialmente de otros desarrolladores que estén aprendiendo gráficos 3D.

### Cómo Contribuir
1. Fork el repositorio
2. Crear una rama para tu feature (`git checkout -b feature/MejoraBásica`)
3. Commit tus cambios (`git commit -m 'Añadir funcionalidad básica'`)
4. Push a la rama (`git push origin feature/MejoraBásica`)
5. Abrir un Pull Request

### Áreas Donde Se Necesita Ayuda
- [ ] **Documentación del código** - Añadir comentarios explicativos
- [ ] **Sistema de iluminación** - Completar implementación de múltiples luces
- [ ] **Carga de materiales** - Mejorar parsing de archivos MTL
- [ ] **Interfaz de usuario** - Expandir funcionalidad del editor ImGui
- [ ] **Optimización básica** - Implementar técnicas simples de optimización
- [ ] **Testing** - Probar en diferentes plataformas
- [ ] **Ejemplos** - Crear tutoriales básicos de uso

### Para Nuevos Contribuidores
- El código está diseñado para ser educativo, no para rendimiento
- Se priorizan implementaciones claras sobre optimización
- Todas las mejoras, por pequeñas que sean, son valiosas
- No hay problema en hacer preguntas en los issues

## 📄 Licencias

Este proyecto utiliza múltiples licencias para diferentes componentes:

- **Código del motor**: MIT License
- **ImGui**: MIT License  
- **GLFW**: zlib/libpng License
- **stb_image**: MIT License o Public Domain

Ver archivos de licencia individuales en las carpetas respectivas.
---
*Proyecto educativo desarrollado con ❤️ por PILTRAFILLA317*  
*Estado: Early Development - Contribuciones bienvenidas*
