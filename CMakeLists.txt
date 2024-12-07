cmake_minimum_required(VERSION 3.10)
project(untitled2)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# SDL2 and SDL_ttf paths
set(SDL2_DIR "C:/msys64/mingw64/lib/cmake/SDL2")
set(SDL2_TTF_DIR "C:/msys64/mingw64/lib/cmake/SDL2_ttf")

# Add SDL2_ttf directory to CMake search path
list(APPEND CMAKE_PREFIX_PATH ${SDL2_TTF_DIR})

# Find SDL2 and SDL_ttf packages
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Define the executable
add_executable(untitled2
        main.cpp
        Code_lib/ExpressionTree.cpp
        Code_lib/ExpressionTree.h
        Code_lib/MyStack.cpp
        Code_lib/MyStack.h
        Code_lib/MyVector.cpp
        Code_lib/MyVector.h

)

# Link SDL2, SDL_ttf, and FreeType libraries
target_link_libraries(untitled2 PRIVATE SDL2::SDL2 SDL2_ttf::SDL2_ttf)

# Define SDL_MAIN_HANDLED before including SDL2 headers
target_compile_definitions(untitled2 PRIVATE SDL_MAIN_HANDLED)

# Include SDL2, SDL_ttf, and FreeType directories
target_include_directories(untitled2 PRIVATE
        C:/msys64/mingw64/include/SDL2
        C:/msys64/mingw64/include/freetype2
)

# Ensure runtime DLLs are available (runtime paths)
set_target_properties(untitled2 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Copy necessary DLLs to the build directory
add_custom_command(TARGET untitled2 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/msys64/mingw64/bin/SDL2.dll"
        "C:/msys64/mingw64/bin/SDL2_ttf.dll"
        "C:/msys64/mingw64/bin/libfreetype-6.dll"
        $<TARGET_FILE_DIR:untitled2>
)
