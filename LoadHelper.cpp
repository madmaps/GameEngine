#include "LoadHelper.h"
#include <iostream>
#include <fstream>
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"
#include "assimp/cimport.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "BmpLoader.h"

int loadNormalMesh(const aiScene* inScene, BumpMapGLRenderer& inRenderer,const int inObjectIndex, const char* inDefuse, const char* inNormal, const char* inSpecular, const char* inAmbient)
{
	
	GLfloat* vertPoints = NULL;
	GLfloat* normalPoints = NULL;
	GLfloat* texturePoints = NULL;
	GLfloat* tangentPoints = NULL;
	unsigned int pointCount = 0;



	const aiMesh* mesh = inScene->mMeshes[inObjectIndex];
	pointCount = mesh->mNumVertices;
	if(mesh->HasPositions())
	{
		//std::cout << "Has positions" << std::endl;
		vertPoints = new GLfloat[pointCount * 3];
		for(unsigned int i = 0;i < pointCount;i++)
		{
			const aiVector3D* vp = &(mesh->mVertices[i]);
			vertPoints[i * 3 + 0] = (GLfloat)vp->x;
			vertPoints[i * 3 + 1] = (GLfloat)vp->y;
			vertPoints[i * 3 + 2] = (GLfloat)vp->z;
		}
	}
	if(mesh->HasNormals())
	{
		//std::cout << "Has normals" << std::endl;
		normalPoints = new GLfloat[pointCount * 3];
		for(unsigned int i = 0; i < pointCount; i++)
		{
			const aiVector3D* vn = &(mesh->mNormals[i]);
			normalPoints[i * 3 + 0] = (GLfloat)vn->x;
			normalPoints[i * 3 + 1] = (GLfloat)vn->y;
			normalPoints[i * 3 + 2] = (GLfloat)vn->z;
		}
	}

	if(mesh->HasTextureCoords(0))
	{
		//std::cout << "Has texture coordinates" << std::endl;
		texturePoints = new GLfloat[pointCount * 2];
		for(unsigned int i = 0; i < pointCount; i++)
		{
			const aiVector3D* vt = &(mesh->mTextureCoords[0][i]);
			texturePoints[i * 2 + 0] = (GLfloat)vt->x;
			texturePoints[i * 2 + 1] = (GLfloat)vt->y;
		}
	}

	if(mesh->HasTangentsAndBitangents())
	{
		//std::cout << "Has tangents" << std::endl;
		tangentPoints = new GLfloat[pointCount * 4];
		for(unsigned int i = 0; i < pointCount; i++)
		{
			const aiVector3D* vt = &(mesh->mTangents[i]);
			const aiVector3D* vb = &(mesh->mBitangents[i]);
			const aiVector3D* vn = &(mesh->mNormals[i]);
			
			glm::vec3 tangent = glm::vec3(vt->x,vt->y,vt->z);
			glm::vec3 biTangent = glm::vec3(vb->x,vb->y,vb->z);
			glm::vec3 normalVec = glm::vec3(vn->x,vn->y,vn->z);
			
			glm::vec3 tempTangent = glm::normalize(tangent - normalVec * glm::dot(normalVec,tangent));
			float det = (glm::dot(glm::cross(normalVec,tangent),biTangent));
			if(det < 0.0f)
			{
				det = -1.0f;
			}
			else
			{
				det = 1.0f;
			}
			tangentPoints[i * 4 + 0] = tempTangent.x;
			tangentPoints[i * 4 + 1] = tempTangent.y;
			tangentPoints[i * 4 + 2] = tempTangent.z;
			tangentPoints[i * 4 + 3] = det;
		}
	}
	
	//aiReleaseImport(scene);
	
	bmpLoader defuseFile;
	bmpLoader normalFile;
	bmpLoader specularFile;
	bmpLoader ambientFile;
    defuseFile.loadFile(inDefuse);
    normalFile.loadFile(inNormal);
    specularFile.loadFile(inSpecular);
    ambientFile.loadFile(inAmbient);
    
    inRenderer.setVertices(vertPoints, pointCount);
    inRenderer.setNormals(normalPoints, pointCount);
    inRenderer.setTextureCoordinates(texturePoints, pointCount);
    inRenderer.setTangents(tangentPoints, pointCount);
    inRenderer.setDiffuseTexture(defuseFile.getWidth(), defuseFile.getHeigth(), defuseFile.getData());
    inRenderer.setNormalTexture(normalFile.getWidth(), normalFile.getHeigth(), normalFile.getData());
    inRenderer.setSpecularTexture(specularFile.getWidth(), specularFile.getHeigth(), specularFile.getData());
    inRenderer.setOcclusionTexture(ambientFile.getWidth(), ambientFile.getHeigth(), ambientFile.getData());
	return 1;
}

int loadSkyBoxMesh(SkyBoxGLRenderer& inRenderer, const char* inMeshFile, const char* inUp, const char* inDown, const char* inLeft, const char* inRight, const char* inFront, const char* inBack)
{
	Assimp::Importer importer;
	
	const aiScene* scene = importer.ReadFile("meshes/box.dae",aiProcess_Triangulate);
	if(!scene)
	{
		std::cout << "BAD!";
	}
	GLfloat* vertPoints = NULL;
	const aiMesh* mesh = scene->mMeshes[0];
	unsigned int pointCount = mesh->mNumVertices;
	if(mesh->HasPositions())
	{
		std::cout << "Has positions" << std::endl;
		vertPoints = new GLfloat[pointCount * 3];
		for(unsigned int i = 0; i < pointCount; i++)
		{
			const aiVector3D* vp = &(mesh->mVertices[i]);
			vertPoints[i * 3 + 0] = (GLfloat)vp->x;
			vertPoints[i * 3 + 1] = (GLfloat)vp->y;
			vertPoints[i * 3 + 2] = (GLfloat)vp->z;
		}
	}
    
	bmpLoader upSkyBox;
	bmpLoader downSkyBox;
	bmpLoader leftSkyBox;
	bmpLoader rightSkyBox;
	bmpLoader frontSkyBox;
	bmpLoader backSkyBox;
	upSkyBox.loadFile(inUp);
	downSkyBox.loadFile(inDown);
	leftSkyBox.loadFile(inLeft);
	rightSkyBox.loadFile(inRight);
	frontSkyBox.loadFile(inFront);
	backSkyBox.loadFile(inBack);
	
	inRenderer.setVertices(vertPoints,pointCount);
	inRenderer.setBoxTextureUp(upSkyBox.getWidth(),upSkyBox.getHeigth(),upSkyBox.getData());
	inRenderer.setBoxTextureDown(downSkyBox.getWidth(),downSkyBox.getHeigth(),downSkyBox.getData());
	inRenderer.setBoxTextureLeft(leftSkyBox.getWidth(),leftSkyBox.getHeigth(),leftSkyBox.getData());
	inRenderer.setBoxTextureRight(rightSkyBox.getWidth(),rightSkyBox.getHeigth(),rightSkyBox.getData());
	inRenderer.setBoxTextureFront(frontSkyBox.getWidth(),frontSkyBox.getHeigth(),frontSkyBox.getData());
	inRenderer.setBoxTextureBack(backSkyBox.getWidth(),backSkyBox.getHeigth(),backSkyBox.getData());

	return 1;
}

GLuint loadShaders(const char* inVertexShaderFile, const char* inFragmentShaderFile)
{
	std::ifstream vertexFile, fragmentFile;
	int vertexLength, fragmentLength;
	
	vertexFile.open(inVertexShaderFile);
	vertexFile.seekg(0,std::ios::end);
	vertexLength = vertexFile.tellg();
	vertexFile.seekg(0,std::ios::beg);
	char* vertexData = new char[vertexLength];
	vertexFile.read(vertexData, vertexLength);
	vertexFile.close();

	fragmentFile.open(inFragmentShaderFile);
	fragmentFile.seekg(0,std::ios::end);
	fragmentLength = fragmentFile.tellg();
	fragmentFile.seekg(0,std::ios::beg);
	char* fragmentData = new char[fragmentLength];
	fragmentFile.read(fragmentData, fragmentLength);
	fragmentFile.close();
	
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexData, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentData, NULL);
    glCompileShader(fs);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,fs);
    glAttachShader(shaderProgram,vs);
    glLinkProgram(shaderProgram);
	
	return shaderProgram;
}
