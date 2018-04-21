#ifndef __JOYSTICKDEVICE__
#define __JOYSTICKDEVICE__
#include <cstdint>
#include "InputDevice.h"
#include <vector>

class JoystickDevice : public InputDevice
{
public:
	JoystickDevice();
	~JoystickDevice();
	
	virtual void poll() = 0;
	virtual uint64_t getButtonsDown()const;
	virtual float getAxis(unsigned int inAxisIndex)const;
protected:
	uint64_t buttons;
	std::vector<float> axisValues;
private:
};
#endif
