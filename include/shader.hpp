#pragma once

#include <string>
#include <fstream>
#include <sstream>

#define GLEW_STATIC
#include "GL/glew.h"

#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext/vector_float4.hpp"
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	unsigned int ID;

	Shader(std::string path);
	void use();

	void setBool(const char* name, bool value) const;
	void setInt(const char* name, int value) const;
	void setFloat(const char* name, float value) const;

	void setVec4(const char* name, glm::vec4) const;
	void setVec4(const char* name, float x, float y, float z, float w) const;

	void setMat4(const char* name, glm::mat4 value) const;
};