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

void StandardObject::setPosition(const glm::vec3& inPosition)
{
	float* tempPosition = (float*)glm::value_ptr(position);
	float* tempInPosition = (float*)glm::value_ptr(inPosition);
	for(unsigned int i = 0; i < 3; i++)
	{
		tempPosition[i] = tempInPosition[i];
	}
}

void StandardObject::setRotation(const glm::quat& inRotation)
{
	float* tempRotation = (float*)glm::value_ptr(rotation);
	float* tempInRotation = (float*)glm::value_ptr(inRotation);
	for(unsigned int i = 0; i < 4; i++)
	{
		tempRotation[i] = tempInRotation[i];
	}
}

