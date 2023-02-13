CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

project("lab1" C)

set(CMAKE_C_STANDARD 99)


set(SRC src)
set(INC src/include)

FILE(GLOB INC_SRC ${INC}/*.c)

add_executable(lab1 ${SRC}/main.c ${INC_SRC})