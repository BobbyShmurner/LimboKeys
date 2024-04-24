#include <iostream>
#include "GLFW/glfw3.h"

void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error: %s\n", description);
}

int main() {
	printf("Loading GLFW...\n");

	if (!glfwInit()) {
		printf("Failed to initialize GLFW!\n");
		return -1;
	}

	glfwSetErrorCallback(glfw_error_callback);

	printf("GLFW loaded successfully\n");

	GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
	if (!window) {
		printf("Failed to Create GLFW Window!\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}