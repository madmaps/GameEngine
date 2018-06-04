#ifndef __WIDGETS__
#define __WIDGETS__
#include <vector>
#include "StandardObject.h"
#include "glm/vec2.hpp"

class Widgets : public StandardObject
{
	public:
        Widgets();
        Widgets(const int inLocationX, const int inLocationY, const int inSizeX, const int inSizeY);
        ~Widgets();
        
        virtual void animate(const double inTime) = 0;
        
        void setActive(const bool inActive);
        void setVisible(const bool inVisible);
        
        bool getActive()const;
        bool getVisible()const;
        
	protected:
        glm::vec2 location;
        glm::vec2 size;
        std::vector<Widgets*> widgets;
        bool isActive;
        bool isVisible;
        
	private:
};
#endif
		
