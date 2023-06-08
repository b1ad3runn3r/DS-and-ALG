cmake_minimum_required(VERSION 3.24)

project("lab2" C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-D Q_LIST")

add_library(queue SHARED src/queue.c)
add_library(airport SHARED src/airport.c)

add_executable(lab2 src/main.c)
target_link_libraries(lab2 queue airport)
