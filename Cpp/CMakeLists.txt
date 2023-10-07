cmake_minimum_required(VERSION 3.20)

# Organise CMake scripts into a folder
list(APPEND CMAKE_MODULE_PATH 
    "${CMAKE_CURRENT_LIST_DIR}/CMake/")

# Embed versioning from Git-Tag
find_package(Version.cmake)

# Enumerate supported languages for target
# @see `CmakeDetermine*` in https://github.com/Kitware/CMake/tree/master/Modules
include(CheckLanguage)

enable_language( C CXX )

project(particleLife)

OPTION(DEBUG "For enabling debug output" OFF) # Enabled by default
OPTION(RECORD "For enabling record" OFF) # Enabled by default

IF(RECORD)
    ADD_DEFINITIONS(-DRECORD)
ENDIF(RECORD)

IF(DEBUG)
    ADD_DEFINITIONS(-DDEBUG)
ENDIF(DEBUG)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/particleLib)

find_package(raylib REQUIRED)
find_package(fmt REQUIRED)

add_executable(particleLife
    main.cpp)

target_link_libraries(particleLife PRIVATE 
    raylib
    fmt::fmt-header-only
    particleLib
)