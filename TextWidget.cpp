#include "TextWidget.h"

TextWidget::TextWidget():Widgets()
{
    textColor = glm::vec3(0.0f, 0.0f, 1.0f);
    modelMatrix = glm::mat4(1.0f);
    textAlignment = 0;
}

TextWidget::TextWidget(std::string inString):Widgets()
{
    outputString = inString;
    textColor = glm::vec3(1.0f, 1.0f, 1.0f);
    modelMatrix = glm::mat4(1.0f);
}

TextWidget::~TextWidget()
{
}

void TextWidget::addRenderer(TextRenderer* inRenderer)
{
    renderer = inRenderer;
    renderer->updateColor((float*)glm::value_ptr(textColor));
    renderer->updateModelMatrix((float*)glm::value_ptr(modelMatrix));
    renderer->updateSize((float*)glm::value_ptr(size));
    renderer->updateTextureCoordinates((float*)glm::value_ptr(topLeft), (float*)glm::value_ptr(bottomRight));
}


void TextWidget::draw()
{
    int currentChar = 0;
    glm::mat4 translationMatrix = glm::mat4(1.0f);
    rotation = glm::angleAxis(glm::radians(0.0f),glm::vec3(0.0f,0.0f,1.0f));
	rotation *= glm::angleAxis(glm::radians(rotationAngle),glm::vec3(0.0f, 0.0f, 1.0f));
    float xOffset = (location.x / (float)(*screenWidth)) * 2 - 1;
    float yOffset = (location.y / (float)(*screenHeight)) * 2 - 1;
    double offset = 0;
    switch(textAlignment)
    {
        case 1:
            offset = -(1.6666 * size.x * outputString.length()) / 2;
            break;
        case 2:
            offset = -(1.6666 * size.x * outputString.length());
            break;
        default:
            offset = 0;
    };
    for(unsigned int i = 0; i < outputString.length();i++)
    {
        currentChar = (int)outputString[i] - 32;
        if(currentChar < 0 || currentChar > 95)
        {
            currentChar = 0;
        }
        int textPositionX = currentChar % (int)letterBoxCount.x;
        int textPositionY = (letterBoxCount.y - 1) - floor((double)currentChar / letterBoxCount.x);
        
        translationMatrix[3][0] = xOffset + offset + (1.666 * size.x * i);
        translationMatrix[3][1] = yOffset;
        glm::mat4 rotationMatrix = glm::toMat4(rotation);
        modelMatrix = rotationMatrix * translationMatrix;
        topLeft = glm::vec2(textPositionX * (1 / letterBoxCount.x), textPositionY *  (1 / letterBoxCount.y));
        bottomRight = glm::vec2((textPositionX + 1) * (1 / letterBoxCount.x), (textPositionY + 1) * (1 / letterBoxCount.y));

        renderer->draw();
    }
    
}

void TextWidget::update(double inTimeStamp)
{
}

void TextWidget::setup()
{
}

void TextWidget::animate(double)
{
}

void TextWidget::setText(std::string inText)
{
    outputString = inText;
}


void TextWidget::setTextColor(const glm::vec3 inTextColor)
{
    textColor = inTextColor;
}

void TextWidget::setTextAlignment(const unsigned int inTextAlignment)
{
    textAlignment = inTextAlignment;
}

void TextWidget::setTextureSize(const glm::vec2 inTextureSize)
{
    textureSize = inTextureSize;
}

void TextWidget::setLetterBoxCount(const glm::vec2 inLetterBoxCount)
{
    letterBoxCount = inLetterBoxCount;
}

