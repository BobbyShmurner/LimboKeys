cmake_minimum_required(VERSION 3.0)

project(LimboKeys
	VERSION 0.1.0
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )

add_subdirectory(lib/glfw)

add_executable(LimboKeys
	src/main.cpp
)

target_link_libraries(LimboKeys ${OPENGL_LIBRARIES} glfw)