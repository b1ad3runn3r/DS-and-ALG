CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

project("lab1" C)

set(CMAKE_C_STANDARD 99)


set(SRC src)

FILE(GLOB INC_SRC ${SRC}/*.c)

add_executable(lab1 ${INC_SRC})
