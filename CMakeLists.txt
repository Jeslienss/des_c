cmake_minimum_required(VERSION 3.12)
project(DES C)

set(CMAKE_C_STANDARD 99)

add_executable(DES main.c des.c des.h)