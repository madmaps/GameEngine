#include "Camera.h"
#include <math.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

Camera::Camera(const float& inNear, const float& inFar, const float& inFieldOfView, const unsigned int& inWidth, const unsigned int& inHeight)
{
	projectionMatrix = new float[16];
	for(unsigned int i = 0; i < 16; i++)
	{
		projectionMatrix[i] = 0;
	}
	viewMatrix = new float[16];
	for(unsigned int i = 0; i < 16; i++)
	{
		viewMatrix[i] = 0;
	}
	locationVector = new float[3];
	for(unsigned int i = 0; i < 3; i++)
	{
		locationVector[i] = 0;
	}
	rotationMatrix = new float[16];
	for(unsigned int i = 0; i < 16; i++)
	{
		rotationMatrix[i] = 0;
	}
	near = inNear;
	far = inFar;
	fieldOfView = inFieldOfView;
	width = inWidth;
	height= inHeight;
	setupProjectionMatrix();
	updateViewMatrix();
}

Camera::~Camera()
{
	delete[] projectionMatrix;
	delete[] viewMatrix;
	delete[] locationVector;
	delete[] rotationMatrix;
}

void Camera::draw()
{
}

void Camera::setup()
{
}

void Camera::update()
{
	updateViewMatrix();
	updateLocation();
	updateRotation();
}


void Camera::updateWidthHeight(const unsigned int& inWidth, const unsigned int& inHeight)
{
	width = inWidth;
	height = inHeight;
	setupProjectionMatrix();
}

void Camera::updateFieldOfView(const float& inFieldOfView)
{
	fieldOfView = inFieldOfView;
	setupProjectionMatrix();
}

void Camera::updateNearFar(const float& inNear,const float& inFar)
{
	near = inNear;
	far = inFar;
	setupProjectionMatrix();
}

float* Camera::getViewMatrix()const
{
	return viewMatrix;
}

float* Camera::getProjectionMatrix()const
{
	return projectionMatrix;
}

float* Camera::getLocationMatrix()const
{
	return locationVector;
}

float* Camera::getRotationMatrix()const
{
	return rotationMatrix;
}

void Camera::updateViewMatrix()
{
	glm::mat4 T = glm::translate(glm::mat4(1.0f),-position);
	glm::mat4 R = glm::toMat4(rotation);
	glm::mat4 view_mat = R * T;
	float* tempViewMatrix = (float*)glm::value_ptr(view_mat);
	for(unsigned int i = 0; i < 16; i++)
	{
		viewMatrix[i] = tempViewMatrix[i];
	}	
}	

void Camera::updateLocation()
{
	float* tempLocation = (float*)glm::value_ptr(position);
	for(unsigned int i = 0; i < 3; i++)
	{
		locationVector[i] = tempLocation[i];
	}
}

void Camera::updateRotation()
{
	glm::mat4 R = glm::toMat4(rotation);
	float* tempRotation = (float*)glm::value_ptr(R);
	for(unsigned int i = 0; i < 16; i++)
	{
		rotationMatrix[i] = tempRotation[i];
	}
}
	
void Camera::setupProjectionMatrix()
{
	float aspect = (float)width/(float)height;
	float range = tan(((fieldOfView * 2 * M_PI) / 360) * 0.5f) * near;
	float Sx = (2.0f * near) / (range * aspect + range * aspect);
	float Sy = near / range;
	float Sz = -(far + near) / (far - near);
	float Pz = -(2.0f * far * near) / (far - near);
	projectionMatrix[0] = Sx;
	projectionMatrix[5] = Sy;
	projectionMatrix[10] = Sz;
	projectionMatrix[11] = -1.0f;
	projectionMatrix[14] = Pz;
}


