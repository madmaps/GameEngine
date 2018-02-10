#ifndef __BUMPMAPGLRENDERER__
#define __BUMPMAPGLRENDERER__
#define GL_GLEXT_PROTOTYPES
#include "RenderTarget.h"
#include <vector>
#include <GL/gl.h>
#include <GL/glx.h>

class BumpMapGLRenderer : public RenderTarget
{
	public:
		BumpMapGLRenderer();
		~BumpMapGLRenderer();
		
		virtual void draw();
		
		void addShader(GLuint inShader);
		
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
		unsigned char vaoStatus;
		unsigned int numberOfVertices;
		
	private:
		void generateVao();
		void addPoints(const unsigned int inType, GLfloat* inPoints, const unsigned int inPointCount);
		void addTexture(const unsigned int inType, const unsigned int inWidth, const unsigned int inHeight, unsigned char* inData);
};
#endif
