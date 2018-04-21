#ifndef __INPUTDEVICE__
#define __INPUTDEVICE__
#include "Component.h"

class InputDevice : public Component
{
public:
	InputDevice();
	~InputDevice();
	virtual void poll() = 0;
protected:
private:
};
#endif
