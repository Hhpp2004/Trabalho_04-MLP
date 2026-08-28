cmake_minimum_required(VERSION 4.3)
project(Trabalho_04 C)

set(CMAKE_C_STANDARD 11)


add_executable(Trabalho_04 main.c
        Multi_Percepton.h
        Multi_Percepton.c
)

target_link_libraries(Trabalho_04 m)
