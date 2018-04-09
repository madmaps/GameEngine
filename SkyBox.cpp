#include "SkyBox.h"
#include "Mesh.h"

void SkyBox::draw()
{
	Mesh* temp = (Mesh*)components.at(0);
	temp->draw();
}

void SkyBox::update(double timeLapse)
{
}

void SkyBox::setup()
{
	
}
