#pragma once
#include <SDL/SDL.h>
#include <GL\glew.h>
#include<glm/glm.hpp>
class Light
{
public:
	~Light();

	Light();
	
	void initLight(glm::vec3 position, glm::vec4 ambientColour, glm::vec4 diffuseColour, glm::vec4 specularColour);
	glm::vec3 GetPosition();
	glm::vec4 GetAmbientColour();
	glm::vec4 GetDiffuseColour();
	glm::vec4 GetSpecularColour();

private:
	glm::vec3 m_Position;
	glm::vec4 m_AmbientColour;
	glm::vec4 m_DiffuseColour;
	glm::vec4 m_SpecularColour;

	void EnableLight();
};

