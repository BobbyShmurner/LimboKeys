#include "key.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glm/common.hpp"
#include "glm/trigonometric.hpp"
#include "glm/ext/vector_float4.hpp"

#ifdef WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#include <windows.h>
#endif

Key::Key(Color col, unsigned int width, unsigned int height) {
	m_Col = col;

	init(width, height);
}

Key::~Key() {
	glfwDestroyWindow(m_Window);
}

void Key::positonForCircle(double t, float speed, float amplitude) {
	double alpha = getCircleAlpha();

	float x = -amplitude * (float)glm::cos(alpha + t * speed);
	float y = -amplitude * (float)glm::sin(alpha + t * speed);

	setPos(x, y);
}

void Key::init(unsigned int width, unsigned int height) {
	static const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
	"out vec2 TexCoord;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "	TexCoord = aTexCoord;\n"
    "}\0";

	static const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec2 TexCoord;\n"
	"uniform sampler2D keyTexture;\n"
	"uniform sampler2D overlayTexture;\n"
	"uniform vec4 col;\n"
	"uniform vec4 overlayCol;\n"
	"void main()\n"
	"{\n"
	"	vec4 base = texture(keyTexture, TexCoord) * col;\n"
	"	vec4 overlay = texture(overlayTexture, TexCoord) * overlayCol;\n"
	"	FragColor = vec4(mix(base.xyz, overlay.xyz, overlay.a), base.a);\n"
	"}\0";

	static float vertices[] = {
		-1.0f, -1.0f, 0.0f, 	0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f, 	1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 	1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 	0.0f, 1.0f,
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
		fprintf(stderr, "[OpenGL]: %s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "[ERROR]" : ""), type, severity, message);
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

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	m_ShaderProgram = glCreateProgram();

	glAttachShader(m_ShaderProgram, vertexShader);
	glAttachShader(m_ShaderProgram, fragmentShader);

	glLinkProgram(m_ShaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Key

	glGenTextures(1, &m_KeyTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_KeyTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int img_width, img_height, nrChannels;
	stbi_set_flip_vertically_on_load(true);  
	unsigned char *key_img_data = stbi_load("resources/img/key.png", &img_width, &img_height, &nrChannels, 0); 

	if (key_img_data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, key_img_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		printf("Failed to load texture!\n");
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
		printf("Failed to load texture!\n");
	}

	stbi_image_free(key_col_img_data);

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Key::render() {
	int width, height;

	glfwGetFramebufferSize(m_Window, &width, &height);
	glfwMakeContextCurrent(m_Window);

	#ifdef WIN32
	{
		if (m_WindowHandle) {
			SetWindowPos((HWND)m_WindowHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
	}
	#endif

	glUseProgram(m_ShaderProgram);
	glViewport(0, 0, width, height);

	unsigned int colLoc = glGetUniformLocation(m_ShaderProgram, "col");
	unsigned int overlayColLoc = glGetUniformLocation(m_ShaderProgram, "overlayCol");

	glm::vec4 col;
	glm::vec4* overlayCol = &col;

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
		default:
			col = glm::vec4(1.0f, 0.0f, 0.051f, 1.0f); // Unknown #FF000D
			overlayCol = new glm::vec4(1.0f, 0.945f, 0.102f, 1.0f); // Unknown #FFF11A
			break;
	}

	glUniform4f(colLoc, col.r, col.g, col.b, col.a);
	glUniform4f(overlayColLoc, overlayCol->r, overlayCol->g, overlayCol->b, overlayCol->a);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_KeyTexture);
	glUniform1i(glGetUniformLocation(m_ShaderProgram, "keyTexture"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_OverlayTexture);
	glUniform1i(glGetUniformLocation(m_ShaderProgram, "overlayTexture"), 1);

	glBindVertexArray(m_VertexArray);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(m_Window);
}

void Key::setVisibility(bool visible) {
	if (visible) {
		glfwShowWindow(m_Window);
	} else {
		glfwHideWindow(m_Window);
	}
}

void Key::setPosAbs(int x, int y) {
	glfwSetWindowPos(m_Window, x, y);
}

void Key::setPos(float x, float y) {
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