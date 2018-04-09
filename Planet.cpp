#include "Planet.h"
#include "StandardMesh.h"

Planet::Planet()
{
	rotationSpeed = 15.0f;
}

Planet::~Planet()
{
}

void Planet::draw()
{
	ModelMesh* temp = (ModelMesh*)components.at(0);
	glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotationMatrix = glm::toMat4(rotation);
	glm::mat4 matrix = translateMatrix * rotationMatrix;
	temp->updateModelMatrix((float*)glm::value_ptr(matrix));
	temp->draw();
}

void Planet::update(double timeLapse)
{
	glm::quat newMoonRotationQuat = glm::angleAxis(glm::radians(rotationSpeed*(float)timeLapse), glm::vec3(0.0f, -1.0f, 0.0f));
	rotation *= newMoonRotationQuat;
}

void Planet::setup()
{
	
}
