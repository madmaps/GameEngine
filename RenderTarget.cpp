#include "RenderTarget.h"

RenderTarget::RenderTarget()
{

}

RenderTarget::~RenderTarget()
{

}

void RenderTarget::updateProjectionMatrix(float* inProjectionMatrix)
{
	projectionMatrix = inProjectionMatrix;
}

void RenderTarget::updateViewMatrix(float* inViewMatrix)
{
	viewMatrix = inViewMatrix;
}
	
void RenderTarget::updateModelMatrix(float* inModelMatrix)
{
	modelMatrix = inModelMatrix;
}

void RenderTarget::updateCameraLocation(float* inCameraLocation)
{
	cameraLocation = inCameraLocation;
}



