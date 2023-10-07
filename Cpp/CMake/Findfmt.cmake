cmake_minimum_required(VERSION 3.18) #FetchContent

find_package(fmt CONFIG QUIET) # Search for installed build

if (NOT fmt_FOUND)

	include(CPM)

	CPMAddPackage(
		NAME fmt
		GIT_TAG 9.1.0
		GITHUB_REPOSITORY fmtlib/fmt
	)
endif()