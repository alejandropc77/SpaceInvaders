cmake_minimum_required(VERSION 3.5)
project(SpaceInvaders LANGUAGES CXX)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(inc)

# Check for Raylib header
if (NOT EXISTS "${CMAKE_SOURCE_DIR}/inc/raylib.h")
    message(FATAL_ERROR "Error: raylib.h not found in inc directory")
endif()

# Project executables
file(GLOB SRC_FILES src/*.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")

# OS-specific configurations
if (WIN32)
    # Windows-specific settings using MinGW

    # Check for Raylib library
    if (NOT EXISTS "${CMAKE_SOURCE_DIR}/lib/libraylib.a")
        message(FATAL_ERROR "Error: libraylib.a not found in lib directory")
    endif()

    # Add Raylib as an imported library
    add_library(raylib STATIC IMPORTED)
    set_target_properties(raylib PROPERTIES
        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/libraylib.a"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/inc"
    )

    # Link libraries
    target_link_libraries(${PROJECT_NAME} PRIVATE raylib
        -static-libgcc -static-libstdc++  # Statically link GCC and stdc++ libraries
        opengl32
        gdi32
        winmm
    )
elseif (UNIX)
    # Unix-like OS (Linux, macOS)

    # Check for Raylib library
    if (NOT EXISTS "${CMAKE_SOURCE_DIR}/lib/libraylib.a")
        message(FATAL_ERROR "Error: libraylib.a not found in lib directory")
    endif()

    # Add Raylib as an imported library
    add_library(raylib STATIC IMPORTED)
    set_target_properties(raylib PROPERTIES
        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/libraylib.a"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/inc"
    )

    # Link libraries
    if (APPLE)
        # macOS-specific libraries
        target_link_libraries(${PROJECT_NAME} raylib
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
        )
    else()
        # Linux-specific libraries
        target_link_libraries(${PROJECT_NAME} raylib
            m  # Math library
            pthread
            dl  # Dynamic loading library
            rt  # Real-time extensions
            X11  # X Window System
        )
    endif()
else()
    message(FATAL_ERROR "Unsupported operating system")
endif()
