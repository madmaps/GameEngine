#ifndef __WIDGETWINDOW__
#define __WIDGETWINDOW__
#include "Widgets.h"
#include "glm/vec2.hpp"

class WidgetWindow : public Widgets
{
	public:
        WidgetWindow(const int inLocationX, const int inLocationY, const int inSizeX, const int inSizeY);
        ~WidgetWindow();
		virtual void setup();
		virtual void update(double TimeLapse);
	protected:
	private:
};
#endif
		
