cmake_minimum_required(VERSION 3.26)
project(StructToBase64)

set(CMAKE_CXX_STANDARD 20)

add_executable(StructToBase64 main.cpp
        structs.h
        config.h
        base64.cpp
        base64.h)
