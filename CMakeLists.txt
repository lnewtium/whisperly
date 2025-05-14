cmake_minimum_required(VERSION 3.0)

project(whisperly LANGUAGES CXX)

set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_STANDART_REQUIRED ON)

# Add executable
add_executable(whisperly src/main.cpp)
