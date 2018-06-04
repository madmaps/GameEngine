#ifndef __TEXTRENDERER__
#define __TEXTRENDERER__
#include "RenderTarget.h"

class TextRenderer : public RenderTarget
{
	public:
		TextRenderer();
		~TextRenderer();
			
		void updateModelMatrix(float* inModelMatrix);
        void updateTextureCoordinates(float* inTopLeft,float* inBottomRight);
        void updateSize(float* inSize);
        void updateColor(float* inColor);
		
		
	protected:
		float* modelMatrix;
        float* topLeft;
        float* bottomRight;
        float* textSize;
        float* color;
		
	private:
};
#endif
