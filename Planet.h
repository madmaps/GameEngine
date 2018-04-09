#ifndef __PLANET__
#define __PLANET__
#include "StandardObject.h"

class Planet : public StandardObject
{
public:
	Planet();
	~Planet();
	void draw();
	void update(double timeLapse);
	void setup();
protected:
private:
	float rotationSpeed;
};

#endif
