cmake_minimum_required(VERSION 3.9)
project(PUT_PR)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenMP)


include_directories(.)

add_executable(PUT_PR
        RWtest.cpp
        Time.cpp
        Time.h)

if (OpenMP_CXX_FOUND)
    target_link_libraries(PUT_PR PUBLIC OpenMP::OpenMP_CXX)
endif ()