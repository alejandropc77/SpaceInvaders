# Configuration & Compilation Details

Follow these steps to configure and compile the project.

## 1. Compile the Raylib Library using MinGW64
   - Navigate to the Raylib source directory:
     ```bash
     cd ./depends/raylib/src/
     ```
   - Compile the library:
     ```bash
     make
     ```
   - Copy the compiled library file to the `./lib` directory:
     ```bash
     cp libraylib.a ../../lib/
     ```
   - Copy the header file to the `./inc` directory:
     ```bash
     cp raylib.h ../../inc/
     ```

## 2. Ensure No Existing Build Directory
   - If the `./build` directory exists, clean it up:
     ```bash
     git clean -fdX
     ```

## 3. Configure the Project with CMake and MinGW64
   - Make sure MinGW64 is included in your environment `PATH`.
   - Configure the project:
     ```bash
     cmake -S . -B build -G "MinGW Makefiles"
     ```
     - `-S .` : Specifies the source directory (current).
     - `-B build` : Specifies the build directory (`build/`).
     - `-G "MinGW Makefiles"` : Uses the MinGW Makefiles generator.
   - Build the project:
     ```bash
     cmake --build build -v
     ```

## 4. Run the Executable
   - The executable can be found in the `/build/bin/` folder.
