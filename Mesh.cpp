#include "Mesh.h"

Mesh::Mesh(StandardObject* inParent) : parent(inParent)
{
}

Mesh::~Mesh()
{

}


void Mesh::addRenderer(RenderTarget* inRenderer)
{
	renderer.push_back(inRenderer);
}


