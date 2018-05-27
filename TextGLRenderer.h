#ifndef __TEXTGLRENDERER__
#define __TEXTGLRENDERER__
#define GL_GLEXT_PROTOTYPES
#include "RenderTarget.h"
#include <GL/gl.h>
#include <GL/glx.h>


class TextGLRenderer : public RenderTarget
{
	public:
		TextGLRenderer();
		~TextGLRenderer();
		
		virtual void draw();
		
		void addShader(GLuint inShader);
		
		void updateModelMatrix(float* inModelMatrix);
        void updateTextureCoordinates(float* inTopLeft,float* inBottomRight);
        void updateSize(float* inSize);
        void updateColor(float* inColor);
		
        void setTextTexture(const unsigned int inWidth, const unsigned int inHeight, unsigned char* inData);
		void setTextTexture(const GLuint inTextTexture);

		GLuint getTextTexure()const;
		
	protected:
		GLuint* textures;
		GLuint* vbos;
		GLuint vao;
		GLuint shader;
		GLuint* uniformLocations;
		float* modelMatrix;
        float* topLeft;
        float* bottomRight;
        float* textSize;
        float* color;
        float* points;
		unsigned int numberOfVertices;
		
	private:
		void generateVao();
		void addPoints(const unsigned int inType, GLfloat* inPoints, const unsigned int inPointCount);
		void addTexture(const unsigned int inType, const unsigned int inWidth, const unsigned int inHeight, unsigned char* inData);
};
#endif
