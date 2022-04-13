cmake_minimum_required(VERSION 3.21)
project(MandelbrotSet)

set(CMAKE_CXX_STANDARD 17)

add_executable(MandelbrotSet main.cpp src/App.cpp src/App.hpp src/MandelbrotSet.cpp src/MandelbrotSet.hpp src/Input.cpp src/Input.hpp)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(MandelbrotSet PRIVATE sfml-system sfml-network sfml-graphics sfml-window)