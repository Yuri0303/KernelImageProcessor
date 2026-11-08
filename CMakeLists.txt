cmake_minimum_required(VERSION 4.4)
project(KernelImageProcessor)

set(CMAKE_CXX_STANDARD 20)

add_executable(KernelImageProcessor main.cpp
        Image.cpp
        Image.h
        PnmImage.h
        Kernel.cpp
        Kernel.h)
