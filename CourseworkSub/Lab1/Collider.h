#pragma once
#include <glm/glm.hpp>
class Collider
{
public:
	Collider();
	~Collider();

	float colSize;
	bool collidingX;
	bool collidingY;
	bool collidingZ;

	glm::vec3 position;
};

