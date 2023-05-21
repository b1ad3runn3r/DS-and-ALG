cmake_minimum_required(VERSION 3.2)
project("Lab 4.b" C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-g")

add_library(llrb_tree SHARED src/llrb_tree.c)
add_library(utils SHARED src/utils.c)
add_library(dialog SHARED src/dialog.c)

target_link_libraries(dialog llrb_tree utils)

add_executable(lab4b src/main.c)
target_link_libraries(lab4b dialog)
