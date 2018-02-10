#include "StandardMesh.h"

StandardMesh::StandardMesh()
{
	
}

StandardMesh::~StandardMesh()
{
	for(unsigned int i = 0; i < renderer.size(); i++)
	{
		if(updateLocationRotation.at(i))
		{
			delete[] locationMatrix.at(i);
			delete[] rotationMatrix.at(i);
		}
	}
}

void StandardMesh::draw()
{
	for(unsigned int i = 0; i < renderer.size(); i++)
	{
		renderer.at(i)->updateModelMatrix(modelMatrix);
		renderer.at(i)->updateProjectionMatrix(projectionMatrix);
		renderer.at(i)->updateViewMatrix(viewMatrix);
		renderer.at(i)->updateCameraLocation(cameraLocation);
		renderer.at(i)->draw();
	}
}

void StandardMesh::addRenderer(RenderTarget* inRenderer)
{
	renderer.push_back(inRenderer);
	updateLocationRotation.push_back(0);
	locationMatrix.push_back(0);
	rotationMatrix.push_back(0);
}

void StandardMesh::addRenderer(RenderTarget* inRenderer, const float* inLocationMatrix, const float* inRotationMatrix)
{
	renderer.push_back(inRenderer);
	updateLocationRotation.push_back(1);
	float* tempLocationMatrix = new float[16];
	for(unsigned int i = 0; i < 16; i++)
	{
		tempLocationMatrix[i] = inLocationMatrix[i];
	}
	locationMatrix.push_back(tempLocationMatrix);
	float* tempRotationMatrix = new float[16];
	for(unsigned int i = 0; i < 16; i++)
	{
		tempRotationMatrix[i] = inRotationMatrix[i];
	}
	rotationMatrix.push_back(tempRotationMatrix);
}




