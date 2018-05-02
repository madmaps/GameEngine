#include "ModelMesh.h"

ModelMesh::ModelMesh()
{
	modelMatrix = new float[16];
}

ModelMesh::~ModelMesh()
{
	delete[] modelMatrix;
}

void ModelMesh::updateModelMatrix(float* inModelMatrix)
{
	for(unsigned int i = 0; i < 16; i++)
	{
		modelMatrix[i] = inModelMatrix[i];
	}
}

float* ModelMesh::getModelMatrix()const
{
	return modelMatrix;
}

