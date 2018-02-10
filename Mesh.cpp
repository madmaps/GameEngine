#include "Mesh.h"

Mesh::Mesh()
{
	modelMatrix = new float[16];
	projectionMatrix = new float[16];
	viewMatrix = new float[16];
	cameraLocation = new float[3];
}

Mesh::~Mesh()
{
	delete[] modelMatrix;
	delete[] projectionMatrix;
	delete[] viewMatrix;
	delete[] cameraLocation;
}

void Mesh::updateModelMatrix(const float* inModelMatrix)
{
	for(unsigned int i = 0; i < 16; i++)
	{
		modelMatrix[i] = inModelMatrix[i];
	}
}

void Mesh::updateProjectionMatrix(const float* inProjectionMatrix)
{
	for(unsigned int i = 0; i < 16; i++)
	{
		projectionMatrix[i] = inProjectionMatrix[i];
	}
}

void Mesh::updateViewMatrix(const float* inViewMatrix)
{
	for(unsigned int i = 0; i < 16; i++)
	{
		viewMatrix[i] = inViewMatrix[i];
	}
}

void Mesh::updateCameraLocation(const float* inCameraLocation)
{
	for(unsigned int i = 0; i < 3; i++)
	{
		cameraLocation[i] = inCameraLocation[i];
	}
}
