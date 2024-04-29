#include <iostream>

#include "key.hpp"
#include "colors.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/trigonometric.hpp"

const unsigned int DEFAULT_WINDOW_WIDTH = (unsigned int)(130 * 1.5);
const unsigned int DEFAULT_WINDOW_HEIGHT = (unsigned int)(86 * 1.5);

void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error: %s\n", description);
}

int main() {
	printf("Loading GLFW... ");

	if (!glfwInit()) {
		printf("FAILED!!!\nFailed to initialize GLFW!\n");
		return -1;
	}

	glfwSetErrorCallback(glfw_error_callback);

	printf("Success!\n");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "LIMBO", NULL, NULL);
	if (!window) {
		printf("Failed to Create GLFW Window!\n");
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

	glfwMakeContextCurrent(window);
	glViewport(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "Failed to init GLEW: %s\n", glewGetErrorString(err));
		glfwTerminate();
		return -1;
	}

	glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
		fprintf(stderr, "[OpenGL]: %s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "[ERROR]" : ""), type, severity, message);
	}, NULL);

	printf("Creating Keys... ");

	Key* keys[8] = {
		new Key(Color::GREEN, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT),
		new Key(Color::YELLOW, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT),
		new Key(Color::BLUE, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT),
		new Key(Color::PURPLE, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT),
		new Key(Color::PINK, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT),
		new Key(Color::AQUA, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT),
		new Key(Color::LIME, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT),
		new Key(Color::RED, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT),
	};

	printf("Complete!\n");

	while (!glfwWindowShouldClose(window)) {
		for (int i = 0; i < 8; i++) {
			keys[i]->render();
			keys[i]->setPos(0.8f * (i / 3.5f - 1.0f), 0.0f);
		}

		glfwPollEvents();
	}

	for (int i = 0; i < 8; i++) {
		delete keys[i];
	}
	delete[] keys;

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}