#include "key.hpp"
#include "state.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glm/common.hpp"
#include "glm/trigonometric.hpp"
#include "glm/ext/vector_float4.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/compatibility.hpp"

#ifdef WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#include <windows.h>
#include <winuser.h>
#endif

#include "tracy/Tracy.hpp"

Key::Key(Color col, unsigned int width, unsigned int height) {
	m_Col = col;

	init(width, height);
}

Key::~Key() {
	glfwDestroyWindow(m_Window);
}

void Key::positonForCircle(double t, float speedX, float speedY, float amplitudeX, float amplitudeY, float freqX, float freqY) {
	ZoneScoped;
	double alpha = getCircleAlpha();

	float x = -amplitudeX * (float)glm::cos((alpha + t * speedX) * freqX);
	float y = -amplitudeY * (float)glm::sin((alpha + t * speedY) * freqY);

	setPos(x, y);
}

void Key::init(unsigned int width, unsigned int height) {
	ZoneScoped;

	int img_width, img_height, nrChannels;
	stbi_set_flip_vertically_on_load(true);  
	unsigned char *key_img_data = stbi_load("resources/img/key.png", &img_width, &img_height, &nrChannels, 0);

	float keyAspect = (float)img_height / (float)img_width;

	static float vertices[] = {
		-(float)width, -(float)width * keyAspect, 0.0f, 0.0f, 0.0f,
		 (float)width, -(float)width * keyAspect, 0.0f, 1.0f, 0.0f,
		 (float)width,  (float)width * keyAspect, 0.0f, 1.0f, 1.0f,
		-(float)width,  (float)width * keyAspect, 0.0f, 0.0f, 1.0f,
	};

	static unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0 
	};

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	m_Window = glfwCreateWindow(width, height, "LIMBO", NULL, NULL);
	glfwSetWindowUserPointer(m_Window, this);

	#ifdef WIN32
	{
		m_WindowHandle = glfwGetWin32Window(m_Window);
		if (m_WindowHandle) {
			ShowWindow((HWND)m_WindowHandle, SW_HIDE);
			SetWindowLongPtr((HWND)m_WindowHandle, GWL_EXSTYLE, WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_OVERLAPPEDWINDOW);
		}
	}
	#endif

	glfwMakeContextCurrent(m_Window);

	glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
		fprintf(stderr, "[OpenGL]:%s %s\n", (type == GL_DEBUG_TYPE_ERROR ? " [ERROR]" : ""), message);
	}, NULL);

	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);

	unsigned int vertexBuffer;

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_ElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// uv attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	// Shader
	m_Shader = new Shader("resources/shaders/key");

	// Key
	glGenTextures(1, &m_KeyTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_KeyTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

	if (key_img_data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, key_img_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		State::exit("Failed to load texture!\n");
	}

	stbi_image_free(key_img_data);

	// Key Col
	glGenTextures(1, &m_OverlayTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_OverlayTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);  
	unsigned char *key_col_img_data = stbi_load("resources/img/keyOverlay.png", &img_width, &img_height, &nrChannels, 0); 

	if (key_col_img_data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, key_col_img_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		State::exit("Failed to load texture!\n");
	}

	stbi_image_free(key_col_img_data);

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Key::render() {
	ZoneScoped;
	int width, height;

	glfwGetFramebufferSize(m_Window, &width, &height);

	{
		ZoneScopedN("glfwMakeContextCurrent");
		glfwMakeContextCurrent(m_Window);
	}

	// Very Slow, also seems unecessary
	// This was orginally added to ensure the windows were always on top
	// But they seem to be on top already
	
	// #ifdef WIN32
	// {
	// 	if (m_WindowHandle) {
	// 		ZoneScopedN("SetWindowPos");
	// 		SetWindowPos((HWND)m_WindowHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	// 	}
	// }
	// #endif

	m_Shader->use();

	glViewport(0, 0, width, height);

	glm::vec4 col;
	glm::vec4 overlayCol;

	glm::vec4 unknownCol = glm::vec4(1.0f, 0.0f, 0.051f, 1.0f); // Unknown Red #FF000D
	glm::vec4 unknownOverlayCol = glm::vec4(1.0f, 0.945f, 0.102f, 1.0f); // Unknown Yellow #FFF11A

	switch (m_Col) {
		case Color::GREEN:
			col = glm::vec4(0.404f, 1.0f, 0.592f, 1.0f); // Green #67FF97
			break;
		case Color::YELLOW:
			col = glm::vec4(1.0f, 0.827f, 0.412f, 1.0f); // Yellow #FFD369
			break;
		case Color::BLUE:
			col = glm::vec4(0.275f, 0.435f, 1.0f, 1.0f); // Blue #466FFF
			break;
		case Color::PURPLE:
			col = glm::vec4(0.588f, 0.275f, 1.0f, 1.0f); // Purple #9646FF
			break;
		case Color::PINK:
			col = glm::vec4(1.0f, 0.373f, 0.906f, 1.0f); // Pink #FF5FE7
			break;
		case Color::AQUA:
			col = glm::vec4(0.518f, 1.0f, 0.988f, 1.0f); // Aqua #84FFFC
			break;
		case Color::LIME:
			col = glm::vec4(0.69f, 1.00f, 0.365f, 1.0f); // Lime #B0FF5D
			break;
		case Color::RED:
			col = glm::vec4(1.0f, 0.259f, 0.259f, 1.0f); // Red #FF4242
			break;
	}

	col = glm::lerp(unknownCol, col, revealedAmount);
	overlayCol = glm::lerp(unknownOverlayCol, col, revealedAmount);

	m_Shader->setVec4("col", col);
	m_Shader->setVec4("overlayCol", overlayCol);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_KeyTexture);
	m_Shader->setInt("keyTexture", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_OverlayTexture);
	m_Shader->setInt("overlayTexture", 1);

	m_Model = glm::ortho(-(float)width, (float)width, -(float)height, (float)height, -1.0f, 1.0f);
	m_Model = glm::rotate(m_Model, float(glm::radians(State::instance()->rotation)), glm::vec3(0.0f, 0.0f, 1.0f));
	m_Shader->setMat4("model", m_Model);

	glBindVertexArray(m_VertexArray);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer);

	{
		ZoneScopedN("glDrawElements");
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	{
		ZoneScopedN("glfwSwapBuffers");
		glfwSwapBuffers(m_Window);
	}
}

void Key::setVisibility(bool visible) {
	ZoneScoped;
	if (visible) {
		glfwShowWindow(m_Window);
	} else {
		glfwHideWindow(m_Window);
	}
}

void Key::setDecoration(bool show) {
	ZoneScoped;

	int width, height;
	glfwGetWindowSize(m_Window, &width, &height);

	glfwSetWindowAttrib(m_Window, GLFW_DECORATED, show);
	glfwSetWindowSize(m_Window, width + 4 , height + 4);
}

void Key::setPosAbs(int x, int y) {
	ZoneScoped;
	#ifdef WIN32
		if (m_WindowHandle) {
			SetWindowPos((HWND)m_WindowHandle, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER | SWP_NOSENDCHANGING | SWP_DEFERERASE);
		} else {
			glfwSetWindowPos(m_Window, x, y);
		}
	#else
		glfwSetWindowPos(m_Window, x, y);
	#endif
}

void Key::setPos(float x, float y) {
	ZoneScoped;
	int screen_width, screen_height;
	int width, height;
	int xAbs, yAbs;

	auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	screen_width = mode->width;
	screen_height = mode->height;

	glfwGetWindowSize(m_Window, &width, &height);

	xAbs = (int)(glm::round((x + 1) * 0.5f * screen_width - width * 0.5f));
	yAbs = (int)(glm::round((y + 1) * 0.5f * screen_height - height * 0.5f));

	return setPosAbs(xAbs, yAbs);
}