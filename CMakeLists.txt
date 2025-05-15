cmake_minimum_required(VERSION 3.30)

project(whisperly LANGUAGES CXX)

set(CMAKE_CXX_STANDART 20)
set(CMAKE_CXX_STANDART_REQUIRED ON)

# Add subdirectories
add_subdirectory(src/client)
add_subdirectory(src/server)
