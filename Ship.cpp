#include "Ship.h"
#include "StandardMesh.h"

void Ship::draw()
{
	ModelMesh* temp = (ModelMesh*)components.at(0);
	glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rotationMatrix = glm::toMat4(rotation);
	glm::mat4 matrix = translateMatrix * rotationMatrix;
	temp->updateModelMatrix((float*)glm::value_ptr(matrix));
	temp->draw();
}

void Ship::update()
{
	rotation *= glm::angleAxis(-0.01f, glm::vec3(0.0f, -1.0f, 0.0f));
}

void Ship::setup()
{
	
}
