cmake_minimum_required(VERSION 3.21)
project(MandelbrotSet)

set(CMAKE_CXX_STANDARD 17)

add_executable(MandelbrotSet main.cpp 
    src/App.cpp src/App.hpp 
    src/MandelbrotSet.cpp src/MandelbrotSet.hpp
    src/ThemeManager.hpp src/ThemeManager.cpp 
    src/LocationManager.hpp src/LocationManager.cpp
    src/HintText.hpp src/HintText.cpp
    src/Input.cpp src/Input.hpp)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(MandelbrotSet PRIVATE -static
    sfml-main sfml-system sfml-network sfml-graphics sfml-window
    imgui::imgui ImGui-SFML::ImGui-SFML
    nlohmann_json nlohmann_json::nlohmann_json)