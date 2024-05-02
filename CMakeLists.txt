cmake_minimum_required(VERSION 3.5)

project(LimboKeys
	VERSION 0.1.0
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} cmake/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package(ImGui REQUIRED)

add_subdirectory(extern/glm)
include_directories(extern/glm)

include_directories(extern/stb)

set(GLEW_INCLUDE_DIRS extern/glew/include)
set(GLEW_LIBRARIES extern/glew/lib)

include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARIES})

add_subdirectory(extern/tracy)
include_directories(extern/tracy/public/tracy)

add_subdirectory(extern/json)
include_directories(extern/json/include)

add_subdirectory(runtime)