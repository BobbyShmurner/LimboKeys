cmake_minimum_required(VERSION 3.5)

project(ImGui
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

add_subdirectory(extern/glfw)
include_directories(extern/glfw/include)

set(GLEW_INCLUDE_DIRS extern/glew/include)
set(GLEW_LIBRARIES extern/glew/lib)

include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARIES})

set(IMGUI_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui)

add_library(ImGui STATIC
	${IMGUI_SOURCE_DIR}/imgui.cpp
	${IMGUI_SOURCE_DIR}/imgui_demo.cpp
	${IMGUI_SOURCE_DIR}/imgui_draw.cpp
	${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
	${IMGUI_SOURCE_DIR}/imgui_tables.cpp
	${IMGUI_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
	${IMGUI_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

include_directories(${IMGUI_SOURCE_DIR})

target_link_libraries(ImGui PUBLIC
	${OPENGL_LIBRARIES}
	glfw
	Release/x64/glew32s
)

set(IMGUI_LIBRARIES ImGui;glfw)