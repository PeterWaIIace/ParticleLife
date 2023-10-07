cmake_minimum_required(VERSION 3.0)

project(myproject)
include(FetchContent)
FetchContent_Declare(raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master
)
FetchContent_MakeAvailable(raylib)

OPTION(DEBUG "For enabling debug output" OFF) # Enabled by default

IF(DEBUG)
    ADD_DEFINITIONS(-DDEBUG)
ENDIF(DEBUG)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/particleLib)

add_executable(myproject
    main.cpp)

target_link_libraries(myproject PRIVATE 
    raylib
    particleLib
)