cmake_minimum_required(VERSION 3.14)
project(particleLib)

find_package(OpenMP)
# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(particleLib INTERFACE)
target_include_directories(particleLib INTERFACE .)

if(OpenMP_CXX_FOUND)
    target_link_libraries(particleLib INTERFACE OpenMP::OpenMP_CXX)
endif()
