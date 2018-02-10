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


