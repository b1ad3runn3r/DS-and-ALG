cmake_minimum_required(VERSION 3.2)
project("Lab 4.a" C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-g")

add_library(tree SHARED src/tree.c)
add_library(utils SHARED src/utils.c)

add_library(dialog SHARED src/dialog.c)
target_link_libraries(dialog utils)

add_executable(lab4a src/main.c)
target_link_libraries(lab4a tree dialog)
