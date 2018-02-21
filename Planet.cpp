#include "Planet.h"
#include "StandardMesh.h"

void Planet::draw()
{
	Mesh* temp = (Mesh*)components.at(0);
	temp-> draw();
}

void Planet::update()
{
	glm::quat newMoonRotationQuat = glm::angleAxis(0.001f, glm::vec3(0.0f, -1.0f, 0.0f));
	rotation *= newMoonRotationQuat;
}

void Planet::setup()
{
	
}
