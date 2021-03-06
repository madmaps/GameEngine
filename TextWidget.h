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
        virtual void animate(double);
        
        void setText(std::string inText);
        
        void setTextColor(const glm::vec3 inTextColor);
        void setTextAlignment(const unsigned int inTextAlignment);
        void setTextureSize(const glm::vec2 inTextureSize);
        void setLetterBoxCount(const glm::vec2 inLetterBoxCount);
        
	protected:
        TextRenderer* renderer;
        std::string outputString;
        glm::mat4 modelMatrix;
        glm::vec2 topLeft;
        glm::vec2 bottomRight;
        glm::vec3 textColor;
        glm::vec2 textureSize;
        glm::vec2 letterBoxCount;
        unsigned int textAlignment;
	private:
};
#endif

