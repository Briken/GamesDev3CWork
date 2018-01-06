#pragma once
#include <string>
#include <GL\glew.h>
#include "Transform.h"
#include "Camera.h"

class Shader
{
public:
	Shader(const std::string& filename);

	void BindShader(); //Set gpu to use our shaders
	void Update(const Transform& transform, const Camera& camera);

	std::string Shader::LoadShader(const std::string& fileName);
	void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint Shader::CreateShader(const std::string& text, unsigned int type);

    ~Shader();
	GLuint GetShaderProgram();

protected:
private:
	static const unsigned int NUM_SHADERS = 2; // number of shaders

	enum
	{
		TRANSFORM_U,

		NUM_UNIFORMS
	};

	GLuint m_Program; // Track the shader program
	GLuint m_Shaders[NUM_SHADERS]; //array of shaders
	GLuint m_Uniforms[NUM_UNIFORMS]; //no of uniform variables
};
