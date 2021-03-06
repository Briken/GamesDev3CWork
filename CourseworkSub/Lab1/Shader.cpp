#include "Shader.h"
#include <iostream>
#include <fstream>

Shader::Shader(const std::string& filename)
{
	m_Program = glCreateProgram(); // create shader program (openGL saves as ref number)
	m_Shaders[0] = CreateShader(LoadShader(filename+".vert"), GL_VERTEX_SHADER); // create vertex shader
	m_Shaders[1] = CreateShader(LoadShader(filename+".frag"), GL_FRAGMENT_SHADER); // create fragment shader

	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glAttachShader(m_Program, m_Shaders[i]); //add all our shaders to the shader program "m_Shaders" 
	}

	glBindAttribLocation(m_Program, 0, "position"); // associate attribute variable with our shader m_Program attribute (in this case attribute vec3 position;)
	glBindAttribLocation(m_Program, 1, "texCoord"); 
	glBindAttribLocation(m_Program, 2, "normal");

	glLinkProgram(m_Program); //create executables that will run on the GPU shaders
	CheckShaderError(m_Program, GL_LINK_STATUS, true, "Error: Shader program linking failed"); // cheack for error

	glValidateProgram(m_Program); //check the entire program is valid
	CheckShaderError(m_Program, GL_VALIDATE_STATUS, true, "Error: Shader program not valid");

	m_Uniforms[TRANSFORM_U] = glGetUniformLocation(m_Program, "transform"); 
	m_Uniforms[LIGHTDIR_U] = glGetUniformLocation(m_Program, "lightDir");
}

Shader::Shader()
{
}




Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glDetachShader(m_Program, m_Shaders[i]); //detach shader from program
		glDeleteShader(m_Shaders[i]); //delete the sahders
	}
	glDeleteProgram(m_Program); // delete the program
}

GLuint Shader::GetShaderProgram()
{
	return m_Program;
}

void Shader::BindShader()
{
	glUseProgram(m_Program); //installs the m_Program object specified by program as part of rendering state
}

void Shader::Update(const Transform* transform, const Camera& camera)
{
	glm::mat4 mvp = camera.GetViewProjection() * transform->GetModel();
	glUniformMatrix4fv(m_Uniforms[TRANSFORM_U], 1, GLU_FALSE, &mvp[0][0]);
}
void Shader::SetDirection(Camera camera)
{
	glm::vec3 direction = camera.GetForward();
	glUniform3fv(m_Uniforms[LIGHTDIR_U], 1, &direction[0]);
}

GLuint Shader::CreateShader(const std::string& text, unsigned int type)
{
	GLuint shader = glCreateShader(type); //create shader based on specified type

	if (shader == 0) //if == 0 shader no created
		std::cerr << "Error type creation failed " << type << std::endl;

	const GLchar* stringSource[1]; //convert strings into list of c-strings
	stringSource[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	glShaderSource(shader, 1, stringSource, lengths); //send source code to opengl
	glCompileShader(shader); //get open gl to compile shader code

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!"); //check for compile error

	return shader;
}

std::string Shader::LoadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

