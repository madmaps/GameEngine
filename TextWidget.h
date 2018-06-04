#ifndef __TEXTWIDGET__
#define __TEXTWIDGET__
#include <string>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"
#include "Widgets.h"
#include "TextRenderer.h"

class TextWidget : public Widgets
{
	public:
		TextWidget();
        TextWidget(std::string inString);
		~TextWidget();
        
        void addRenderer(TextRenderer* inRenderer);
        
		virtual void draw();
		virtual void update(double inTimeStamp);
        virtual void setup();
        
        void setText(std::string inText);
        
	protected:
        TextRenderer* renderer;
        std::string outputString;
        glm::mat4 modelMatrix;
        glm::vec2 topLeft;
        glm::vec2 bottomRight;
        glm::vec3 textColor;
        float aspectRatio;
        float textHeight;
        int screenWidth;
        int screenHeight;
	private:
};
#endif

/*
 *     glm::mat4 ident = glm::mat4(1.0f);
    glm::vec2 topLeft = glm::vec2(0.0833333f, 0.0f);
    glm::vec2 bottomRight = glm::vec2(0.1666666f, 0.125f);
    
    float* textSize = new float[2];
    textSize[0] = 0.25f; 
    textSize[1] = 0.5f;
    
    glm::vec3 textColor = glm::vec3(1.0f, 0.0f, 1.0f);
*/
