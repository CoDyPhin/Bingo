cmake_minimum_required (VERSION 3.16)

project ("BingoProject")

#set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 20)

add_library(BingoProject SHARED BingoProject/BingoProject.cpp)

target_include_directories(BingoProject PUBLIC ${PROJECT_SOURCE_DIR}/BingoProject)