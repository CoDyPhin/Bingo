# CMakeList.txt : CMake project for Binary, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.16)

project ("Binary")

# Add source to this project's executable.
add_executable (Binary "Binary.cpp" "Binary.h")

add_library(BingoProject SHARED IMPORTED)

# Provide library link location

set_property(TARGET BingoProject PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libBingoProject.so")

target_link_libraries(Binary dl)
target_link_libraries(Binary BingoProject)