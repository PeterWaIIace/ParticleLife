set(CPM_DOWNLOAD_VERSION 0.36.0)

if(CPM_PATH) # CPM_PATH may be provided on command-line
  set(CPM_DOWNLOAD_LOCATION "${CPM_PATH}/CPM.cmake")
elseif(DEFINED ENV{CPM_PATH}) # CPM_PATH may be provided from script environment
  file(TO_CMAKE_PATH "$ENV{CPM_PATH}/CPM.cmake" CPM_DOWNLOAD_LOCATION)
elseif(CPM_SOURCE_CACHE) 
  set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
elseif(DEFINED ENV{CPM_SOURCE_CACHE})
  set(CPM_DOWNLOAD_LOCATION "$ENV{CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
else()
  set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
endif()

# Expand relative path. This is important if the provided path contains a tilde (~)
get_filename_component(CPM_DOWNLOAD_LOCATION ${CPM_DOWNLOAD_LOCATION} ABSOLUTE)
if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
  message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
  file(DOWNLOAD
       https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
       ${CPM_DOWNLOAD_LOCATION}
  )
endif()

include(${CPM_DOWNLOAD_LOCATION})

if ( NOT COMMAND CPMAddPackage )
    message(FATAL "[CPM download] failed or corrupted... removing. Please re-run Cmake-configure")
    file(REMOVE ${CPM_DOWNLOAD_LOCATION})
endif()