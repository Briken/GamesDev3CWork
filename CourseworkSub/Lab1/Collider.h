#pragma once
#include <glm/glm.hpp>
class Collider
{
public:
	Collider(); //constructor
	~Collider();//destructor

	float colSize;//radius of the collider


	glm::vec3 position;//collider current position
};

