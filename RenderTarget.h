#ifndef __RENDERTARGET__
#define __RENDERTARGET__

class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();
	virtual void draw() = 0;
	virtual void updateProjectionMatrix(float* inProjectionMatrix);
	virtual void updateViewMatrix(float* inViewMatrix);	
	virtual void updateModelMatrix(float* inModelMatrix);
	virtual void updateCameraLocation(float* inCameraLocation);

protected:
	float* projectionMatrix;
	float* viewMatrix;
	float* modelMatrix;
	float* cameraLocation;

	private:
};
#endif
