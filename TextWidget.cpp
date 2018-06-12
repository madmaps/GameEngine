#include "TextWidget.h"

TextWidget::TextWidget():Widgets()
{
    textColor = glm::vec3(0.0f, 0.0f, 1.0f);
    modelMatrix = glm::mat4(1.0f);
    textSize = glm::vec2(0.007f, 0.01f);
    textAlignment = 0;
}

TextWidget::TextWidget(std::string inString):Widgets()
{
    outputString = inString;
    textColor = glm::vec3(1.0f, 1.0f, 1.0f);
    modelMatrix = glm::mat4(1.0f);
    textSize = glm::vec2(0.01f, 0.013f);
}

TextWidget::~TextWidget()
{
}

void TextWidget::addRenderer(TextRenderer* inRenderer)
{
    renderer = inRenderer;
    renderer->updateColor((float*)glm::value_ptr(textColor));
    renderer->updateModelMatrix((float*)glm::value_ptr(modelMatrix));
    renderer->updateSize((float*)glm::value_ptr(textSize));
    renderer->updateTextureCoordinates((float*)glm::value_ptr(topLeft), (float*)glm::value_ptr(bottomRight));
}


void TextWidget::draw()
{
    int currentChar = 0;
    glm::mat4 translationMatrix = glm::mat4(1.0f);
    translationMatrix[3][0] = textSize.x;
    double offset = 0;
    switch(textAlignment)
    {
        case 1:
            offset = -(1.6666 * textSize.x * outputString.length()) / 2;
            break;
        case 2:
            offset = -(1.6666 * textSize.x * outputString.length());
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
        int textPositionX = currentChar % 12;
        int textPositionY = 7 - floor((double)currentChar / 12);
        
        translationMatrix[3][0] = offset + (1.666 * textSize.x * i);
        modelMatrix = translationMatrix;
        topLeft = glm::vec2(textPositionX * 0.0833333f, textPositionY *  0.125);
        bottomRight = glm::vec2((textPositionX + 1) * 0.0833333f, (textPositionY + 1) * 0.125f);

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

void TextWidget::setTextSize(const glm::vec2 inTextSize)
{
    textSize = inTextSize;
}


void TextWidget::setTextColor(const glm::vec3 inTextColor)
{
    textColor = inTextColor;
}

void TextWidget::setTextAlignment(const unsigned int inTextAlignment)
{
    textAlignment = inTextAlignment;
}

