#include "TextRenderer.h"

TextRenderer::TextRenderer()
{
}

TextRenderer::~TextRenderer()
{
}

void TextRenderer::updateModelMatrix(float* inModelMatrix)
{
    modelMatrix = inModelMatrix;
}

void TextRenderer::updateTextureCoordinates(float* inTopLeft,float* inBottomRight)
{
    topLeft = inTopLeft;
    bottomRight = inBottomRight;
}

void TextRenderer::updateSize(float* inSize)
{
    textSize = inSize;
}

void TextRenderer::updateColor(float* inColor)
{
    color = inColor;
}

