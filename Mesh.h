#ifndef __MESH__
#define __MESH__
#include "Component.h"
#include "RenderTarget.h"

class Mesh public Component
{
	public:
		virtual void draw() = 0;
		virtual void setup() = 0;
	protected:
		RenderTarget* renderer;
	private:
};
#endif
