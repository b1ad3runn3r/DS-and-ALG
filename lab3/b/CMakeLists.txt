cmake_minimum_required(VERSION 3.2)
project("Lab 3" C)

set(CMAKE_C_STANDARD 99)

add_library(libtable SHARED src/table.c)
add_library(libutils SHARED src/utils.c)
add_library(libdialog SHARED src/dialog.c)
add_library(libfiles SHARED src/files.c)
target_link_libraries(libdialog libutils)
target_link_libraries(libtable libfiles)

add_executable(lab3 src/main.c)
target_link_libraries(lab3 libtable libutils libdialog)
