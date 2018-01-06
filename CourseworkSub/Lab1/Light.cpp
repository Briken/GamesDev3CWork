#include "Light.h"

Light::~Light()
{
}

Light::Light()
{
}

void Light::initLight(glm::vec3 position, glm::vec4 ambientColour, glm::vec4 diffuseColour, glm::vec4 specularColour)
{
	m_Position = position;
	m_AmbientColour = ambientColour;
	m_DiffuseColour = diffuseColour;
	m_SpecularColour = specularColour;

	EnableLight();
}

glm::vec3 Light::GetPosition()
{
	return m_Position;
}

glm::vec4 Light::GetAmbientColour()
{
	return m_AmbientColour;
}

glm::vec4 Light::GetDiffuseColour()
{
	return m_DiffuseColour;
}

glm::vec4 Light::GetSpecularColour()
{
	return m_SpecularColour;
}

void Light::EnableLight()
{
	glEnable(GL_LIGHTING); // enable lighting calculations
	glEnable(GL_LIGHT0);

	GLfloat globalAmbient[] = { m_AmbientColour.x, m_AmbientColour.y, m_AmbientColour.z, m_AmbientColour.w };//{ 0.1f, 0.0f, 0.0f, 1.0f };
	GLfloat diffuseLight[] = { m_DiffuseColour.x, m_DiffuseColour.y, m_DiffuseColour.z, m_DiffuseColour.w };//{ 0.8f, 0.8f, 0.8, 1.0f };
	GLfloat specular[] = { m_SpecularColour.x, m_SpecularColour.y, m_SpecularColour.z, m_SpecularColour.w };//{ 1.0,1.0,1.0,1.0 };
	GLfloat position[] = { m_Position.x, m_Position.y, m_Position.z, 1.0f };//{ -1.5f, 1.5f, -4.0f, 1.0f, };

	glLightfv(GL_LIGHT0, GL_AMBIENT, globalAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
}
