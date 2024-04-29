cmake_minimum_required(VERSION 3.5)

project(LimboKeys
	VERSION 0.1.0
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} cmake/)

message("CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package(ImGui REQUIRED)

# add_subdirectory(extern/glfw)
# include_directories(extern/glfw/include)

add_subdirectory(extern/glm)
include_directories(extern/glm)

include_directories(extern/stb)

set(GLEW_INCLUDE_DIRS extern/glew/include)
set(GLEW_LIBRARIES extern/glew/lib)

include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARIES})

include_directories(include)

add_executable(LimboKeys
	src/key.cpp
	src/main.cpp
	src/shader.cpp
)

target_link_libraries(LimboKeys Release/x64/glew32s ${OPENGL_LIBRARIES} ${IMGUI_LIBRARIES})