#ifndef __SKYBOXGLRENDERER__
#define __SKYBOXGLRENDERER__
#define GL_GLEXT_PROTOTYPES
#include "RenderTarget.h"
#include <GL/gl.h>
#include <GL/glx.h>

class SkyBoxGLRenderer : public RenderTarget
{
	public:
		SkyBoxGLRenderer();
		~SkyBoxGLRenderer();
		
		virtual void draw();
		
		void addShader(GLuint inShader);
		
		void updateProjectionMatrix(const float* inProjectionMatrix);
		void updateCameraRotation(const float* inCameraRotation);
		
		void setVertices(GLfloat* inVertices,const unsigned int inNumberOfVertices);
		
		void setBoxTextureUp(const unsigned int inWidth, const unsigned int inHeight, unsigned char* inData);
		void setBoxTextureDown(const unsigned int inWidth, const unsigned int inHeight, unsigned char* inData);
		void setBoxTextureLeft(const unsigned int inWidth, const unsigned int inHeight, unsigned char* inData);
		void setBoxTextureRight(const unsigned int inWidth, const unsigned int inHeight, unsigned char* inData);
		void setBoxTextureFront(const unsigned int inWidth, const unsigned int inHeight, unsigned char* inData);
		void setBoxTextureBack(const unsigned int inWidth, const unsigned int inHeight, unsigned char* inData);
		
			
	protected:
		GLuint* textures;
		GLuint* vbos;
		GLuint vao;
		GLuint shader;
		GLuint* uniformLocations;
		float* projectionMatrix;
		float* cameraRotation;
		unsigned char vaoStatus;
		unsigned char textureStatus;
		unsigned int numberOfVertices;
	private:
		void generateVao();
		void formatTextures();
};
#endif	
		
