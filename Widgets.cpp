#include "Widgets.h"

Widgets::Widgets()
{
    isActive = false;
    isVisible = false;
}
    
Widgets::Widgets(const int inLocationX, const int inLocationY, const int inSizeX, const int inSizeY)
{
    location.x = inLocationX;
    location.y = inLocationY;
    size.x = inLocationX;
    size.y = inLocationY;
    isActive = false;
    isVisible = false;
}

Widgets::~Widgets()
{
}

void Widgets::setScreenDim(int* inScreenWidth, int* inScreenHeight)
{
    screenWidth = inScreenWidth;
    screenHeight = inScreenHeight;
}

void Widgets::setActive(const bool inActive)
{
    isActive = inActive;
}

void Widgets::setVisible(const bool inVisible)
{
    isVisible = inVisible;
}

void Widgets::setLocation(const glm::vec2 inLocation)
{
    location = inLocation;
}

void Widgets::setSize(const glm::vec2 inSize)
{
    size.x = inSize.x/(float)(*screenWidth);
    size.y = inSize.y/(float)(*screenHeight);
}

bool Widgets::getActive()const
{
    return isActive;
}

bool Widgets::getVisible()const
{
    return isVisible;
}
        

