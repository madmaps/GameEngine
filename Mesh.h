#ifndef __MESH__
#define __MESH__
#include <vector>
#include "Component.h"
#include "RenderTarget.h"
#include "StandardObject.h"

class Mesh : public Component
{
public:
	Mesh();
	~Mesh();
	virtual void draw() = 0;
	virtual void addRenderer(RenderTarget* inRenderer);
protected:
	std::vector<RenderTarget*> renderer;
private:
};
#endif
