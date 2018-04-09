#include "SkyBoxMesh.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "SkyBoxGLRenderer.h"


SkyBoxMesh::SkyBoxMesh()
{
}

SkyBoxMesh::~SkyBoxMesh()
{
}

void SkyBoxMesh::draw()
{
	for(unsigned int i = 0; i < renderer.size(); i++)
	{
		renderer.at(i)->draw();
	}
}

void SkyBoxMesh::setup()
{
}

void SkyBoxMesh::update(double timeLapse)
{
}



