#ifndef __STANDARDMESH__
#define __STANDARDMESH__
#include <vector>
#include "Mesh.h"
#include "RenderTarget.h"

class StandardMesh : public Mesh
{
public:
	StandardMesh(StandardObject* inParent);
	~StandardMesh();
	virtual void draw();
	virtual void setup();
	virtual void update();
	virtual void updateModelMatrix(float* inModelMatrix);
	virtual float* getModelMatrix()const;
	virtual void addRenderer(RenderTarget* inRenderer);
	virtual void addRenderer(RenderTarget* inRenderer, const float* inLocationMatrix, const float* inRotationMatrix);
protected:
	float* modelMatrix;
	std::vector<bool> updateLocationRotation;
	std::vector<float*> locationMatrix;
	std::vector<float*> rotationMatrix;
	
private:
};
#endif

