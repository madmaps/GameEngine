#ifndef __SKYBOXMESH__
#define __SKYBOXMESH__
#include <vector>
#include "Mesh.h"
#include "RenderTarget.h"
#include "StandardObject.h"

class SkyBoxMesh : public Mesh
{
public:
	SkyBoxMesh();
	~SkyBoxMesh();
	virtual void draw();
	virtual void setup();
	virtual void update(double timeLapse);
protected:

	
private:
};
#endif

