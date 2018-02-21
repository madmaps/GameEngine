#include "StandardMesh.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "BumpMapGLRenderer.h"
#include <iostream>


StandardMesh::StandardMesh(StandardObject* inParent) : Mesh(inParent)
{
	modelMatrix = new float[16];
}

StandardMesh::~StandardMesh()
{
	delete[] modelMatrix;
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
	glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), parent->getPosition());
	glm::mat4 rotationMatrix = glm::toMat4(parent->getRotation());
	glm::mat4 matrix = translateMatrix * rotationMatrix;
	updateModelMatrix((float*)glm::value_ptr(matrix));
	for(unsigned int i = 0; i < renderer.size(); i++)
	{
		renderer.at(i)->draw();
	}
}

void StandardMesh::setup()
{
}

void StandardMesh::update()
{
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

void StandardMesh::updateModelMatrix(float* inModelMatrix)
{
	for(unsigned int i = 0; i < 16; i++)
	{
		modelMatrix[i] = inModelMatrix[i];
	}
}

float* StandardMesh::getModelMatrix()const
{
	return modelMatrix;
}






