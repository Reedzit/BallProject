cmake_minimum_required(VERSION 3.5)

project(BallProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
add_definitions(-DQT_NO_INT128)

find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)
find_package(glm)

add_executable(BallProject
    main.cpp
    mainwindow.ui
    mainwindow.cpp
    mainwindow.hpp

)

target_link_libraries(BallProject PRIVATE Qt6::Widgets Qt6::OpenGL Qt::OpenGLWidgets glm::glm)
target_compile_features(BallProject PRIVATE cxx_std_17)
