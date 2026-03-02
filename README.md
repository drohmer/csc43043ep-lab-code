# CSC 43043 EP - Computer Graphics Lab Code

Lab exercises for the course **CSC 43043 EP** - Introduction to C++ and Computer Graphics.

- Course page (French): https://graphicscomputing.fr/course/2025_2026/CSC_43043_EP/lab/
- Course page (English): https://graphicscomputing.fr/course/2025_2026/CSC_43043_EP/lab-en/

## Compilation and Installation

### Prerequisites

| Platform | Requirements |
|----------|-------------|
| **Windows** | [CMake](https://cmake.org/download/), [Visual Studio Community](https://visualstudio.microsoft.com/) (with "Desktop development with C++" module) |
| **Mac** | [CMake](https://cmake.org/download/), [VS Code](https://code.visualstudio.com/), [Homebrew](https://brew.sh/) |
| **Linux** | [CMake](https://cmake.org/download/), [VS Code](https://code.visualstudio.com/), build-essential, pkg-config |

### Windows

1. Install CMake (enable "Add CMake to the system PATH" during setup).
2. Install Visual Studio Community with the **Desktop development with C++** module.
3. Navigate to the exercise directory and run the generation script:
   ```
   00_test_installation/scripts/visual-studio-generate.bat
   ```
4. Open the generated `build/project.sln` in Visual Studio.
5. Compile and run using the "Local Windows Debugger" button.

> **Note:** The code directory path must not contain accents or spaces. When adding new `.cpp` files, re-run the generation script.

### Mac

1. Install dependencies:
   ```bash
   brew install cmake pkg-config ninja glfw
   ```
2. Build:
   ```bash
   cd 00_test_installation
   mkdir build && cd build
   cmake ..
   make
   ./project
   ```

If GLFW is not found, edit `CMakeLists.txt` and set `MACOS_GLFW_PRECOMPILED` to `ON`, then open `cgp/third_party/glfw/macos/lib/libglfw.3.dylib` via Finder (right-click > Open) before recompiling.

### Linux (Ubuntu)

1. Install dependencies:
   ```bash
   sudo apt-get update
   sudo apt-get install build-essential pkg-config cmake libglfw3-dev
   ```
2. Build:
   ```bash
   cd 00_test_installation
   mkdir build && cd build
   cmake ..
   make
   ./project
   ```

### Editing Code

Open the `vscode.code-workspace` file at the root of each exercise in VS Code. Select the **RelWithDebInfo** configuration for debug compilation.

Full installation details: https://graphicscomputing.fr/course/2025_2026/CSC_43043_EP/lab/content/z0_installation/

## Lab Sessions

### 00 - Test Installation
`00_test_installation/`

Verify that the development environment and dependencies (OpenGL, GLFW, CGP library) are correctly set up.

### 01 - Introduction to C++
`01_first_program/`

Introduction to C++ fundamentals: functions, basic types, collections (`std::vector`, `std::array`, `std::map`), and file I/O.

| Exercise | Directory | Description |
|----------|-----------|-------------|
| Hello World | `a_introduction/` | Minimal program, `std::cout`, functions, basic types |
| ASCII Tree | `b_ascii_tree/` | Generate an ASCII Christmas tree, string manipulation, file export |
| ASCII Histogram | `c_ascii_histogram/` | Read a text file, count word occurrences with `std::map`, display a histogram |
| PPM Image | `d_ppm_image/` | Export images in PPM format: gradients, checkerboards, noise, colored disks |

### 02 - Types and Pointers
`02_types_pointers/`

Memory management, pointers, dynamic allocation, bitwise operations, and image compression.

| Exercise | Directory | Description |
|----------|-----------|-------------|
| Pointers | `a_basic_pointers/` | Pointer traversal, element swapping, dynamic allocation, memory leaks |
| Buffer Manipulation | `b_buffer_manipulation/` | Bitwise operations, extracting values from packed data |
| Image Encoding | `c_image_encoding/` | RGB565 encoding/decoding, Run Length Encoding (RLE) compression |

### 03 - Classes
`03_classes/`

Object-oriented programming: structures, classes, constructors, operator overloading, inheritance, and polymorphism.

| Exercise | Directory | Description |
|----------|-----------|-------------|
| Vec3 Class | `a_vec3/` | Implement a 3D vector class: norm, dot/cross products, operator overloading, mat3 class |
| Hierarchy | `b_hierarchy/` | Inheritance and polymorphism with an Entity hierarchy (Player, Enemy, NPC) |
| Dynamic Image | `c_dynamic_image/` | Dynamic memory management in a class context |

### 04 - Graphics and Interaction
`04_graphique/`

Introduction to graphical rendering with the CGP/OpenGL framework and GUI interaction with ImGui.

| Exercise | Directory | Description |
|----------|-----------|-------------|
| GUI Interaction | `a_gui/` | Interactive 2D shape management: polymorphic shape classes, ImGui sliders and controls |
| Software Renderer | `b_renderer/` | Implement a software rasterizer: triangle projection, lighting, depth buffer, mesh rendering |

### 05 - Shaders
`05_shaders/`

OpenGL shader programming with GLSL: vertex and fragment shaders, uniforms, and visual effects.

| Exercise | Directory | Description |
|----------|-----------|-------------|
| Shader Manipulation | `a_shader_manipulation/` | Load meshes, edit vertex/fragment shaders, pass uniforms, animate deformable surfaces |
| Shading Effects | `b_shading_effect/` | Implement fog, light attenuation, and distance-based illumination effects |

### 06 - Modeling
`06_modeling/`

3D terrain and object modeling using height fields and geometric primitives.

Exercises: terrain generation with superposition of Gaussians, cylinder and cone mesh creation, tree assembly from primitives, placement on terrain with collision avoidance.

### 07 - Textures
`07_texture/`

Texture mapping techniques, procedural generation, and animation.

| Exercise | Directory | Description |
|----------|-----------|-------------|
| UV Textures | `a_uv_texture/` | Texture coordinates, UV mapping on parametric surfaces, wrapping modes |
| Billboards | `b_billboards/` | Semi-transparent billboard rendering, alpha blending, depth sorting |
| Perlin Noise | `c_perlin_noise/` | Procedural terrain generation with multi-octave Perlin noise |
| Animation | `d_animation/` | Procedural animation: bubble particles, smoke billboards, animated liquid surface |

### 08 - Project
`08_project/`

Template for the final project.
