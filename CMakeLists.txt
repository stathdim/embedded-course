cmake_minimum_required(VERSION 3.14)
project(embeded_systems C)

set(CMAKE_C_STANDARD 99)

add_executable(embeded_systems main_with.c)
add_executable(embeded_systems_without main_without.c)