#ifndef __TESTRENDERER__
#define __TESTRENDERER__
#define GL_GLEXT_PROTOTYPES
#include "RenderTarget.h"
#include <vector>
#include <GL/gl.h>
#include <GL/glx.h>

class TestRenderer : public RenderTarget
{
	public:
		TestRenderer();
		~TestRenderer();
		
		virtual void draw();
		
		void generateVao();
		
		void addShader(GLuint inShader);
		
		void updateProjectionMatrix(const float* inProjectionMatrix);
		void updateViewMatrix(const float* inViewMatrix);	
		void updateModelMatrix(const float* inModelMatrix);
		void updateCameraLocation(const float* inCameraLocation);
		
		void setVertices(GLfloat* inVertices, const unsigned int numberOfPoints);
		void setNormals(GLfloat* inNormals, const unsigned int numberOfPoints);
		void setTextureCoordinates(GLfloat* inTextureCoordinates, const unsigned int numberOfPoints);
		void setTangents(GLfloat* inTangents, const unsigned int numberOfTangents);
		
		void setDiffuseTexture(const unsigned int inWidth,const unsigned int inHeight, unsigned char* inData);
		void setNormalTexture(const unsigned int inWidth,const unsigned int inHeight, unsigned char* inData);
		void setSpecularTexture(const unsigned int inWidth,const unsigned int inHeight, unsigned char* inData);
		void setOcclusionTexture(const unsigned int inWidth,const unsigned int inHeight, unsigned char* inData);
		
	protected:
		GLuint* textures;
		GLuint* vbos;
		GLuint vao;
		GLuint shader;
		GLuint* uniformLocations;
		unsigned int numberOfVertices;
		
	private:
		void addPoints(const unsigned int inType, GLfloat* inPoints, const unsigned int inPointCount);
		void addTexture(const unsigned int inType, const unsigned int inWidth, const unsigned int inHeight, unsigned char* inData);
};
#endif
