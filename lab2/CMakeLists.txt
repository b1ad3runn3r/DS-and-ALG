cmake_minimum_required(VERSION 3.24)

project("lab2" C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-gdwarf-4")

add_library(queue SHARED src/include/queue.c)
add_library(airport SHARED src/include/airport.c)

add_executable(lab2 src/main.c)
target_link_libraries(lab2 queue airport)
