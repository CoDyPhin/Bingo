cmake_minimum_required (VERSION 3.16)

project ("BingoProject")

#set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-rdynamic")

add_library(BingoProject SHARED "src/Bingo.cpp" "src/Card.cpp" "src/utils.cpp")

target_include_directories(BingoProject PUBLIC ${PROJECT_SOURCE_DIR}/src)