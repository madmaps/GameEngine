#include "JoystickDevice.h"

JoystickDevice::JoystickDevice()
{
}

JoystickDevice::~JoystickDevice()
{
}

uint64_t JoystickDevice::getButtonsDown()const
{
	return buttons;
}

float JoystickDevice::getAxis(unsigned int inAxisIndex)const
{
	float returnValue = 0;
	if(inAxisIndex < axisValues.size())
	{	
		returnValue = axisValues.at(inAxisIndex);
	}
	return returnValue;
}
