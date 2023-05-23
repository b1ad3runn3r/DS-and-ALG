cmake_minimum_required(VERSION 3.2)
project("Lab 5" C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-g")

add_library(list SHARED src/list.c)
add_library(queue SHARED src/queue.c)
add_library(priority_queue SHARED src/priority_queue.c)

add_library(graph SHARED src/graph.c)
target_link_libraries(graph list queue priority_queue)

add_library(utils SHARED src/utils.c)

add_library(dialog SHARED src/dialog.c)
target_link_libraries(dialog graph utils)

add_executable(lab5 src/main.c)
target_link_libraries(lab5 dialog)
