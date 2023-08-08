cmake_minimum_required(VERSION 3.0)

project(myproject)
include(FetchContent)
FetchContent_Declare(raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master
)
FetchContent_MakeAvailable(raylib)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/particleLab)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/particleLib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)

add_executable(myproject
    main.cpp)

target_include_directories(myproject
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/particleLab
)

message(STATUS "path: ${CMAKE_CURRENT_SOURCE_DIR}/particleLab")
target_link_libraries(myproject PRIVATE raylib)