#include "ObjectSocket.h"

ObjectSocket::ObjectSocket()
{
}

ObjectSocket::ObjectSocket(glm::vec3 inPosition, glm::quat inRotation)
{
	position = inPosition;
	rotation = inRotation;
}

ObjectSocket::~ObjectSocket()
{
}

void ObjectSocket::setPosition(const glm::vec3& inPosition)
{
	position = inPosition;
}

void ObjectSocket::setRotation(const glm::quat& inRotation)
{
	rotation = inRotation;
}

glm::vec3 ObjectSocket::getPosition()const
{
	return position;
}

glm::quat ObjectSocket::getRotation()const
{
	return rotation;
}
