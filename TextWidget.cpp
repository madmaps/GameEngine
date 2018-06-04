#include "TextWidget.h"

TextWidget::TextWidget():Widgets()
{
}

TextWidget::TextWidget(std::string inString):Widgets()
{
    outputString = inString;
}

TextWidget::~TextWidget()
{
}

void TextWidget::addRenderer(TextRenderer* inRenderer)
{
    renderer = inRenderer;
}


void TextWidget::draw()
{
}

void TextWidget::update(double inTimeStamp)
{
}

void TextWidget::setup()
{
}

void TextWidget::setText(std::string inText)
{
    outputString = inText;
}

