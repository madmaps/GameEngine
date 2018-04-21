#ifndef __SDLJOYSTICKDEVICE__
#define __SDLJOYSTICKDEVICE__
#include <cstdint>
#include <SDL2/SDL.h>
#include "JoystickDevice.h"

class SdlJoystickDevice : public JoystickDevice
{
public:
	SdlJoystickDevice();
	~SdlJoystickDevice();
	
	virtual void update(double inTimeStamp);
	virtual void setup();
	virtual void poll();
	virtual void getEvent(SDL_Event& sdlEvent);
	void clearButtons();
protected:
	SDL_Event* event;
private:
};
#endif

