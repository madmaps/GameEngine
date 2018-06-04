#ifndef __TEXTGLRENDERER__
#define __TEXTGLRENDERER__
#define GL_GLEXT_PROTOTYPES
#include "TextRenderer.h"
#include <GL/gl.h>
#include <GL/glx.h>


class TextGLRenderer : public TextRenderer
{
	public:
		TextGLRenderer();
		~TextGLRenderer();
		
		virtual void draw();
		
		void addShader(GLuint inShader);
		
        void setTextTexture(const unsigned int inWidth, const unsigned int inHeight, unsigned char* inData);
		void setTextTexture(const GLuint inTextTexture);

		GLuint getTextTexure()const;
		
	protected:
		GLuint* textures;
		GLuint* vbos;
		GLuint vao;
		GLuint shader;
		GLuint* uniformLocations;
        float* points;
		unsigned int numberOfVertices;
		
	private:
		void generateVao();
		void addPoints(const unsigned int inType, GLfloat* inPoints, const unsigned int inPointCount);
};
#endif
