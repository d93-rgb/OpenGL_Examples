cmake_minimum_required (VERSION 3.9)
cmake_policy(SET CMP0048 NEW)
project(Rendering-Triangle VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)

set(ALL_PROJECT_LIBRARIES
    OpenGL::GL
	OpenGL::GLU
	glfw)	# <- target name, contains absolute path to library

SET(PROJECT_SOURCES "src/main.cpp")

add_executable(Rendering-Triangle ${OPTIONS} ${PROJECT_SOURCES} ${GL3W_SOURCES})

target_link_libraries(Rendering-Triangle ${ALL_PROJECT_LIBRARIES})

