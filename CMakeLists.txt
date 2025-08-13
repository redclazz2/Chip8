cmake_minimum_required(VERSION 3.10.0)
project(chip8 VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_COMPILER "/usr/bin/g++" CACHE STRING "" FORCE)

#Tells cmake to find SDL3 in the system
find_package(SDL3 REQUIRED CONFIG)

include_directories(
    ${SDL3_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
)

#Adds the source files that the executable uses
add_executable(chip8
    src/main.cpp
    src/chip8/chip8.cpp
    src/display/display.cpp
    src/input/input.cpp
)

#Links the library to the target
target_link_libraries(chip8 SDL3::SDL3)
#Inclues to the target
target_include_directories(chip8 PRIVATE ${SDL3_INCLUDE_DIRS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

#When this file is set up, we create a build folder
#and inside we run: cdmake {target to this file}
#that builds native files.

#Since I'm on linux, the file generated will be a Makefile,
#a makefile that can run with make, and it'll save the build
#to the system.
