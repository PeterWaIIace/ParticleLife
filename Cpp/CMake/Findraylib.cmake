cmake_minimum_required(VERSION 3.18) #FetchContent

find_package(raylib CONFIG QUIET) # Search for installed build

if (NOT raylib_FOUND)

	include(CPM)

	CPMAddPackage(
		NAME raylib
		GIT_TAG master
        GIT_REPOSITORY https://github.com/raysan5/raylib.git

	)
endif()