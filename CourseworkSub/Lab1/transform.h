#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Camera.h"
#include "Collider.h"

struct Transform//holds all transform data for the model, position rotation scale, also holds a collider for collision detection
{
public:
	bool isActive = true;
	Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f))
	{
		this->m_Position = pos;
		this->m_Rotation = rot;
		this->m_Scale = scale;
	}

	inline glm::mat4 GetModel() const
	{
		glm::mat4 posMat = glm::translate(m_Position);
		glm::mat4 scaleMat = glm::scale(m_Scale);
		glm::mat4 rotX = glm::rotate(m_Rotation.x, glm::vec3(1.0, 0.0, 0.0));
		glm::mat4 rotY = glm::rotate(m_Rotation.y, glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 rotZ = glm::rotate(m_Rotation.z, glm::vec3(0.0, 0.0, 1.0));
		glm::mat4 rotMat = rotX * rotY * rotZ;

		return posMat * rotMat * scaleMat;
	}
	void ControllerRotate(float angle)
	{
		static const glm::vec3 const_UP(0.0f, 1.0f, 0.0f);

		glm::mat4 rotation = glm::rotate(angle, const_UP);

		m_Forward = glm::vec3(glm::normalize(rotation * glm::vec4(m_Forward, 0.0)));
		m_Up = glm::vec3(glm::normalize(rotation * glm::vec4(m_Up, 0.0)));
	}

	inline glm::vec3* GetPosition() { return &m_Position; } //getters
	inline glm::vec3* GetRot() { return &m_Rotation; }
	inline glm::vec3* GetScale() { return &m_Scale; }

	inline void SetPos(glm::vec3& pos) { this->m_Position = pos; collider.position = pos; } // setters
	inline void SetRot(glm::vec3& rot) { this->m_Rotation = rot; }
	inline void SetScale(glm::vec3& scale) { this->m_Scale = scale; }
	

	Collider collider;
protected:
private:
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;
	glm::vec3 m_Forward;
	glm::vec3 m_Up;
};


