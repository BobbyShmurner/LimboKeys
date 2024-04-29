cmake_minimum_required(VERSION 3.5)

project(LimboKeys
	VERSION 0.1.0
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

add_subdirectory(lib/glfw)
include_directories(lib/glfw/include)

add_subdirectory(lib/glm)
include_directories(lib/glm)

set(GLEW_INCLUDE_DIRS lib/glew/include)
set(GLEW_LIBRARIES lib/glew/lib)

include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARIES})

include_directories(include)
include_directories(include)

add_executable(LimboKeys
	src/key.cpp
	src/main.cpp
	src/shader.cpp
)

target_link_libraries(LimboKeys ${OPENGL_LIBRARIES} Release/x64/glew32s glfw)