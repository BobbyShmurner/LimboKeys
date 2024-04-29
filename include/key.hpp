#pragma once

#include "colors.hpp"

#define GLEW_STATIC

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/gtc/constants.hpp"

class Key {
private:
	Color m_Col;
	GLFWwindow* m_Window;
	void* m_WindowHandle = nullptr;

	unsigned int m_ShaderProgram;
	unsigned int m_ElementBuffer;
	unsigned int m_VertexArray;

	unsigned int m_KeyTexture;
	unsigned int m_OverlayTexture;

	void init(unsigned int width, unsigned int height);

	const constexpr double getCircleAlpha() {
		switch (m_Col) {
			case Color::GREEN:
				return 0.0;
			case Color::BLUE:
				return glm::quarter_pi<double>();
			case Color::PINK:
				return glm::half_pi<double>();
			case Color::RED:
				return glm::quarter_pi<double>() * 3.0;
			case Color::LIME:
				return glm::pi<double>();
			case Color::AQUA:
				return glm::pi<double>() + glm::quarter_pi<double>();
			case Color::PURPLE:
				return glm::three_over_two_pi<double>();
			case Color::YELLOW:
				return glm::pi<double>() * 1.75;
			default:
				return 0.0;
		}
	}
public:
	Key(Color col, unsigned int width, unsigned int height);
	~Key();

	void setPos(float x, float y);
	void setPosAbs(int x, int y);

	void setVisibility(bool visible);

	void positonForCircle(double t, float speed = 0.25, float amplitude = 0.6);

	void render();
};