cmake_minimum_required(VERSION 3.2)
project("Lab 3" C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-g")

add_executable(lab3 src/main.c src/table.c src/utils.c src/dialog.c)

