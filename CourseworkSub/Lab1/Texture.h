#pragma once
#include <string>
#include <GL\glew.h>

class Texture
{
public://basic texture class to load, set active and bind a texture to the pipeline for drawing
	Texture(const std::string& fileName);
	Texture();
	void SetTexture(const std::string& fileName);
	void BindTexture(unsigned int unit); // bind upto 32 textures

	~Texture();

protected:
private:

	GLuint m_TextureHandler;
};

