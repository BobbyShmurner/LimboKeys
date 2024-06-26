#include "shader.hpp"
#include "state.hpp"

#include "tracy/Tracy.hpp"
#include <format>

Shader::Shader(std::string path) {
    ZoneScoped;
	std::string vertexPath = path + "/vert.glsl";
	std::string fragmentPath = path + "/frag.glsl";

	std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();		

        vShaderFile.close();
        fShaderFile.close();

        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();		
    } catch (std::ifstream::failure e) {
        State::exit("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];
	
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		State::exit(std::format("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}\n", infoLog).c_str());
	};

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        State::exit(std::format("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{}\n", infoLog).c_str());
	};

    ID = glCreateProgram();

    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);

    glLinkProgram(ID);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() {
    ZoneScoped;
    glUseProgram(ID);
}

void Shader::setBool(const char* name, bool value) const {
    ZoneScoped;
    glUniform1i(glGetUniformLocation(ID, name), (int)value);
}

void Shader::setInt(const char* name, int value) const {
    ZoneScoped;
    glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::setFloat(const char* name, float value) const {
    ZoneScoped;
    glUniform1f(glGetUniformLocation(ID, name), value);
}

void Shader::setVec4(const char* name, glm::vec4 value) const {
    ZoneScoped;
    setVec4(name, value.x, value.y, value.z, value.w);
}

void Shader::setVec4(const char* name, float x, float y, float z, float w) const {
    ZoneScoped;
    glUniform4f(glGetUniformLocation(ID, name), x, y, z, w);
}

void Shader::setMat4(const char* name, glm::mat4 value) const {
    ZoneScoped;
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
}