#include "StandardObject.h"

StandardObject::StandardObject()
{
	
}
StandardObject::~StandardObject()
{
	
}


void StandardObject::addComponent(Component* inComponent)
{
	components.push_back(inComponent);
}

glm::vec3 StandardObject::getPosition()const
{
	return position;
}

glm::quat StandardObject::getRotation()const
{
	return rotation;
}
