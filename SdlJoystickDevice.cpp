#include "SdlJoystickDevice.h"

SdlJoystickDevice::SdlJoystickDevice()
{
	for(unsigned int i = 0; i < 5; i++)
	{
		axisValues.push_back(0.0f);
	}
}
SdlJoystickDevice::~SdlJoystickDevice()
{
}

void SdlJoystickDevice::update(double inTimeStamp)
{

}

void SdlJoystickDevice::setup()
{}

void SdlJoystickDevice::poll()
{
	switch((*event).type)
	{
		case SDL_JOYAXISMOTION:
			axisValues.at((*event).jaxis.axis) = ((float)(*event).jaxis.value/32767.0f);
			break;
		case SDL_JOYBUTTONDOWN:
			buttons |= 0x01 << (*event).jbutton.button;
			break;
		default:
			break;
	}
}

void SdlJoystickDevice::getEvent(SDL_Event& sdlEvent)
{
	event = &sdlEvent;
}

void SdlJoystickDevice::clearButtons()
{
	buttons = 0;
}

