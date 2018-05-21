#ifndef __WIDGETS__
#define __WIDGETS__
#include <vector>
#include "Component.h"
#include "glm/vec2.hpp"

class Widgets : public Component
{
	public:
        Widgets(const int inLocationX, const int inLocationY, const int inSizeX, const int inSizeY);
        ~Widgets();
	protected:
        glm::vec2 location;
        glm::vec2 size;
        std::vector<Widgets*> widgets;
	private:
};
#endif
		
