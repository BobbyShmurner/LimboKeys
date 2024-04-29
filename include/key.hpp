#pragma once

#include "colors.hpp"

#define GLEW_STATIC

#include "GL/glew.h"
#include "GLFW/glfw3.h"

class Key {
private:
	Color m_Col;
	GLFWwindow* m_Window;

	unsigned int m_ShaderProgram;
	unsigned int m_VertexArray;
	unsigned int m_ColLocation;

	unsigned int m_Texture;
	unsigned int m_ElementBuffer;

	void init(unsigned int width, unsigned int height);
public:
	Key(Color col, unsigned int width, unsigned int height);
	~Key();

	void setPos(float x, float y);
	void setPosAbs(int x, int y);

	void render();
};