cmake_minimum_required(VERSION 3.2)
project("Lab 3" C)

set(CMAKE_C_STANDARD 99)
add_library(libtable SHARED src/include/table.c)

add_executable(lab3 src/main.c)
target_link_libraries(lab3 libtable)