cmake_minimum_required(VERSION 3.2)
project("Lab 3" C)

set(CMAKE_C_STANDARD 99)

add_library(libtable SHARED src/include/table.c)
add_library(libutils SHARED src/include/utils.c)
add_library(libdialog SHARED src/include/dialog.c)
target_link_libraries(libdialog libutils)

add_executable(lab3 src/main.c)
target_link_libraries(lab3 libtable libutils libdialog)
