#ifndef __STANDARDMESH__
#define __STANDARDMESH__
#include <vector>
#include "ModelMesh.h"
#include "RenderTarget.h"

class StandardMesh : public ModelMesh
{
public:
	StandardMesh();
	~StandardMesh();
	virtual void draw();
	virtual void setup();
	virtual void update();
	virtual void addRenderer(RenderTarget* inRenderer);
	virtual void addRenderer(RenderTarget* inRenderer, const float* inLocationMatrix, const float* inRotationMatrix);
protected:
	std::vector<bool> updateLocationRotation;
	std::vector<float*> locationMatrix;
	std::vector<float*> rotationMatrix;
	
private:
};
#endif

