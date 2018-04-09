#ifndef __SKYBOX__
#define __SKYBOX__
#include "StandardObject.h"

class SkyBox : public StandardObject
{
public:
	void draw();
	void update(double timeLapse);
	void setup();
protected:
private:
};

#endif
