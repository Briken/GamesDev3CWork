#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct Camera
{
public:
	Camera()
	{
	}

	//Initializes the camera and sets the values for variables required
	void InitializeCamera(const glm::vec3& pos, float fov, float aspect, float nearClip, float farClip)
	{
		this->m_Position = pos;

		this->m_Forward = glm::vec3(0.0f, 0.0f, 1.0f);
		this->m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->m_Projection = glm::perspective(fov, aspect, nearClip, farClip);

	}

	//Returns the cameras view matrix
	inline glm::mat4 GetViewProjection() const
	{
		return m_Projection * glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
	}

	//Moves the camera forward and backward dependant on value passed
	void MoveForwardBack(float amt)
	{
		m_Position += m_Forward * amt;
	}

	//Strafes the camera left or right dependant on value passed
	void MoveLeftRight(float amt)
	{
		m_Position += glm::cross(m_Up, m_Forward) * amt;
	}

	//Rotates the camera forward and backward
	void Pitch(float angle)
	{
		glm::vec3 axis = glm::normalize(glm::cross(m_Up, m_Forward));

		m_Forward = glm::vec3(glm::normalize(glm::rotate(angle, axis) * glm::vec4(m_Forward, 0.0)));
		m_Up = glm::normalize(glm::cross(m_Forward, axis));
	}

	//Rotates the camera about the Y axis
	void RotateY(float angle)
	{
		static const glm::vec3 const_UP(0.0f, 1.0f, 0.0f);

		glm::mat4 rotation = glm::rotate(angle, const_UP);

		m_Forward = glm::vec3(glm::normalize(rotation * glm::vec4(m_Forward, 0.0)));
		m_Up = glm::vec3(glm::normalize(rotation * glm::vec4(m_Up, 0.0)));
	}
	glm::vec3 GetPos()
	{
		return this->m_Position;
	}
	glm::vec3 GetForward()
	{
		return this->m_Forward;
	}

	bool isActive = true;
protected:
private:
	glm::mat4 m_Projection; //projection matrix
	glm::vec3 m_Position;	//position vector
	glm::vec3 m_Forward;	//forward vector
	glm::vec3 m_Up;			//relative up vector

};


