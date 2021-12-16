cmake_minimum_required(VERSION 3.18)

project("QtQuickPlotScene" VERSION 1.0 LANGUAGES C CXX)

find_package(Qt6 REQUIRED COMPONENTS Core)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

add_compile_definitions(QT_DISABLE_DEPRECATED_BEFORE=0x050F00)

set(DEPLOY_PATH "deploy" CACHE STRING "Deploy folder name")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${DEPLOY_PATH}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${DEPLOY_PATH}")

add_subdirectory("QtQuickPlotScene")
add_subdirectory("examples")
add_subdirectory("tests")

set(QML_IMPORT_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} CACHE STRING "" FORCE)
