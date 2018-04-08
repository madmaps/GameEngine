#ifndef __LOADHELPER__
#define __LOADHELPER__
#include "BumpMapGLRenderer.h"
#include "SkyBoxGLRenderer.h"
#include "assimp/scene.h"

int loadNormalMesh(const aiScene* inScene, BumpMapGLRenderer& inRenderer,const int inObjectIndex, const char* inDefuse, const char* inNormal, const char* inSpecular, const char* inAmbient);

int loadSkyBoxMesh(SkyBoxGLRenderer& inRenderer, const char* inMeshFile, const char* inUp, const char* inDown, const char* inLeft, const char* inRight, const char* inFront, const char* inBack);

GLuint loadShaders(const char* inVertexShaderFile, const char* inFragmentShaderFile);

#endif
