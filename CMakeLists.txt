cmake_minimum_required(VERSION 2.31)
project(riptide-simulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# string(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wcast-qual -Wformat=2 -Wundef -Werror=float-equal -Wshadow -Wcast-align -Wunused -Wnull-dereference -Wdouble-promotion -Wimplicit-fallthrough -Wextra-semi -Woverloaded-virtual -Wnon-virtual-dtor -Wold-style-cast")
string(APPEND CMAKE_CXX_FLAGS "-fsanitize=address,undefined -fno-sanitize-recover=all")

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

add_executable(simulate main.cpp action.cpp construction.cpp detector.cpp generator.cpp physics.cpp run.cpp)
target_link_libraries(simulate ${Geant4_LIBRARIES})
