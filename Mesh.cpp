#include "Mesh.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}


void Mesh::addRenderer(RenderTarget* inRenderer)
{
	renderer.push_back(inRenderer);
}


