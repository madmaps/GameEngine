#ifndef __MODELMESH__
#define __MODELMESH__
#include "Mesh.h"

class ModelMesh : public Mesh
{
public:
	ModelMesh();
	~ModelMesh();
	virtual void draw()=0;
	virtual void setup()=0;
	virtual void update()=0;
	virtual void updateModelMatrix(float* inModelMatrix);
	virtual float* getModelMatrix()const;
protected:
	float* modelMatrix;
private:
};
#endif

