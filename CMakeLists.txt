cmake_minimum_required(VERSION 3.20.0)

PROJECT(simple_tokenize)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_compile_options(-std=c++17)
add_compile_options(-O3)
add_compile_options(-Wall)

add_executable(simple_tokenize simple_tokenize.cpp)
