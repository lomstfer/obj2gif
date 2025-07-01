cmake_minimum_required(VERSION 3.10.0)
project(fluffycube VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB SOURCES "*.cpp" "*.c")
add_executable(fluffycube ${SOURCES})

