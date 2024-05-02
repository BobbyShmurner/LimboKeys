#pragma once

#include "colors.hpp"
#include "shader.hpp"

#define GLEW_STATIC

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/gtc/constants.hpp"

class KeyManager;

class Key {
friend class KeyManager;
private:
	Color m_Col;
	GLFWwindow* m_Window;
	void* m_WindowHandle = nullptr;

	Shader* m_Shader;
	unsigned int m_ElementBuffer;
	unsigned int m_VertexArray;

	unsigned int m_KeyTexture;
	unsigned int m_OverlayTexture;

	float m_Width;
	float m_Height;
	glm::vec2 m_Pos = glm::vec2(0.0f, 0.0f);

	unsigned int m_WindowWidth = 0;
	unsigned int m_WindowHeight = 0;

	glm::mat4 m_Model = glm::mat4(1.0f);

	KeyManager* m_Manager = nullptr;

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

	void m_UpdatePos();
	void m_SetPosAbs(int x, int y);

	~Key();
public:
	float revealedAmount = 0.0f;
	float rotation = 0.0f;

	Key(KeyManager* manager, Color col, float width);

	void initWindow();

	inline void setPos(float x, float y) { setPos(glm::vec2(x, y)); }
	void setPos(glm::vec2 pos);
	void setVisibility(bool visible);
	void setDecoration(bool show);

	void getWindowMaxSize(unsigned int& width, unsigned int& height);

	void positonForCircle(double t, float speedX = 0.25f, float speedY = 0.25f, float amplitudeX = 0.6f, float amplitudeY = 0.6f, float freqX = 1.0f, float freqY = 1.0f);
	void clearBoarderFix();

	void render();
};