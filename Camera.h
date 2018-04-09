#ifndef __CAMERA__
#define __CAMERA__
#include "StandardObject.h"

class Camera : public StandardObject
{
public:
	Camera(const float& inNear, const float& inFar, const float& inFieldOfView, const unsigned int& inWidth, const unsigned int& inHeight);
	~Camera();
	virtual void update(double timeLapse);
	virtual void draw();
	virtual void setup();
	void updateWidthHeight(const unsigned int& inWidth, const unsigned int& inHeight);
	void updateFieldOfView(const float& inFieldOfView);
	void updateNearFar(const float& inNear,const float& inFar);
	
	float* getViewMatrix()const;
	float* getProjectionMatrix()const;
	float* getLocationMatrix()const;
	float* getRotationMatrix()const;
	
	
protected:
	float* projectionMatrix;
	float* viewMatrix;
	float* locationVector;
	float* rotationMatrix;
	float near;
	float far;
	float fieldOfView;
	unsigned int width;
	unsigned int height;
private:
	void setupProjectionMatrix();
	void updateViewMatrix();
	void updateLocation();
	void updateRotation();
};
#endif
