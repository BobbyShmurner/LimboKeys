project(LimboKeys-Runtime
	VERSION 0.1.0
	LANGUAGES CXX
)

add_executable(${PROJECT_NAME}
	src/key.cpp
	src/main.cpp
	src/shader.cpp
	src/key_manager.cpp
)

add_custom_target(copy ALL)
add_custom_command(TARGET copy PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)

set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "LimboKeys"
)

target_include_directories(${PROJECT_NAME} PRIVATE
	include
)

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${IMGUI_LIBRARIES} Release/x64/glew32s Tracy::TracyClient nlohmann_json::nlohmann_json -static-libgcc -static-libstdc++)