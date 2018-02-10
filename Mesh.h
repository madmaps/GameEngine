#ifndef __MESH__
#define __MESH__
#include <vector>
#include "Component.h"
#include "RenderTarget.h"

class Mesh : public Component
{
public:
	Mesh();
	~Mesh();
	virtual void draw() = 0;
	virtual void setup() = 0;
	virtual void updateModelMatrix(const float* inModelMatrix);
	virtual void updateProjectionMatrix(const float* inProjectionMatrix);
	virtual void updateViewMatrix(const float* inViewMatrix);
	virtual void updateCameraLocation(const float* inCameraLocation);
protected:
	std::vector<RenderTarget*> renderer;
	float* modelMatrix;
	float* projectionMatrix;
	float* viewMatrix;
	float* cameraLocation;
private:
};
#endif
