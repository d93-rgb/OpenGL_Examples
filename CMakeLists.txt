cmake_minimum_required (VERSION 3.9)
cmake_policy(SET CMP0048 NEW)
#project(OpenGL_Examples VERSION 1.0)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "*** Setting build type to 'Release' ***")
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
elseif (NOT CMAKE_BUILD_TYPE)
	message(STATUS "*** Setting build type to 'Release' ***")
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
else()
	message(STATUS "*** ${CMAKE_BUILD_TYPE} build ***")
endif()

add_subdirectory(OpenGL_Examples)
