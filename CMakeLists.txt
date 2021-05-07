cmake_minimum_required(VERSION 3.10)
project(code)
add_compile_options(-std=c++11 -O2)
add_executable(code backend/main.cpp)
