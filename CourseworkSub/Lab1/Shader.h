#pragma once
#include <string>
#include <GL\glew.h>
#include "Transform.h"
#include "Camera.h"

class Shader//basic shader class used to set, bind, update, and render from the shader
{
public:
	Shader(const std::string& filename);
	Shader();
	void SetShader(const std::string& filename);

	void BindShader(); //Set gpu to use our shaders
	void Update(const Transform* transform, const Camera& camera);

	void SetDirection(Camera camera);//used to create a light director for a toon shader

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
		LIGHTDIR_U,

		NUM_UNIFORMS
	};

	GLuint m_Program; // Track the shader program
	GLuint m_Shaders[NUM_SHADERS]; //array of shaders
	GLuint m_Uniforms[NUM_UNIFORMS]; //no of uniform variables
};
