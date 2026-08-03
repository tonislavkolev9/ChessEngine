cmake_minimum_required(VERSION 3.20)

project(ChessEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "src/*.cpp")
add_executable(ChessEngine ${SOURCES})

target_include_directories(ChessEngine PRIVATE include)