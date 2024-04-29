#include "key.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glm/trigonometric.hpp"

Key::Key(Color col, unsigned int width, unsigned int height) {
	m_Col = col;

	init(width, height);
}

Key::~Key() {
	glfwDestroyWindow(m_Window);
}

void Key::positonForCircle(double t, double speed, double amplitude) {
	double alpha = getCircleAlpha();

	double x = -amplitude * glm::cos(alpha + t * speed);
	double y = -amplitude * glm::sin(alpha + t * speed);

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
	"uniform sampler2D outTexture;\n"
	"uniform vec4 col;\n"
	"void main()\n"
	"{\n"
    "	FragColor = texture(outTexture, TexCoord) * col;\n"
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

	glGenTextures(1, &m_Texture); 
	glBindTexture(GL_TEXTURE_2D, m_Texture); 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int img_width, img_height, nrChannels;
	stbi_set_flip_vertically_on_load(true);  
	unsigned char *data = stbi_load("resources/img/key.png", &img_width, &img_height, &nrChannels, 0); 

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

	} else {
		printf("Failed to load texture!\n");
	}

	stbi_image_free(data);

	glUseProgram(m_ShaderProgram);
	m_ColLocation = glGetUniformLocation(m_ShaderProgram, "col");

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Key::render() {
	int width, height;

	glfwGetFramebufferSize(m_Window, &width, &height);
	glfwMakeContextCurrent(m_Window);

	glUseProgram(m_ShaderProgram);
	glViewport(0, 0, width, height);

	switch (m_Col) {
		case Color::GREEN:
			glUniform4f(m_ColLocation, 0.404f, 1.0f, 0.592f, 1.0f); // Green #67FF97
			break;
		case Color::YELLOW:
			glUniform4f(m_ColLocation, 1.0f, 0.827f, 0.412f, 1.0f); // Yellow #FFD369
			break;
		case Color::BLUE:
			glUniform4f(m_ColLocation, 0.275f, 0.435f, 1.0f, 1.0f); // Blue #466FFF
			break;
		case Color::PURPLE:
			glUniform4f(m_ColLocation, 0.588f, 0.275f, 1.0f, 1.0f); // Purple #9646FF
			break;
		case Color::PINK:
			glUniform4f(m_ColLocation, 1.0f, 0.373f, 0.906f, 1.0f); // Pink #FF5FE7
			break;
		case Color::AQUA:
			glUniform4f(m_ColLocation, 0.518f, 1.0f, 0.988f, 1.0f); // Aqua #84FFFC
			break;
		case Color::LIME:
			glUniform4f(m_ColLocation, 0.69f, 1.00f, 0.365f, 1.0f); // Lime #B0FF5D
			break;
		case Color::RED:
			glUniform4f(m_ColLocation, 1.0f, 0.259f, 0.259f, 1.0f); // Red #FF4242
			break;
		default:
			glUniform4f(m_ColLocation, 1.0f, 0.0f, 0.051f, 1.0f); // Unknown #FF000D
			break;
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
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

	xAbs = (int)((x + 1) * 0.5f * screen_width);
	yAbs = (int)((y + 1) * 0.5f * screen_height);

	xAbs -= width / 2;
	yAbs -= height / 2;

	return setPosAbs(xAbs, yAbs);
}