#include "shader.hpp"

Shader::Shader(std::string path) {
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
        printf("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
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
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
	};

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
	};

    ID = glCreateProgram();

    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);

    glLinkProgram(ID);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool(const char* name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name), (int)value);
}

void Shader::setInt(const char* name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::setFloat(const char* name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name), value);
}

void Shader::setVec4(const char* name, glm::vec4 value) const {
    setVec4(name, value.x, value.y, value.z, value.w);
}

void Shader::setVec4(const char* name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(ID, name), x, y, z, w);
}
