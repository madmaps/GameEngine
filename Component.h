#ifndef __COMPONENT__
#define __COMPONENT__

class Component
{
	public:
		virtual void setup() = 0;
		virtual void update(double TimeLapse) = 0;
	protected:
	private:
};
#endif
		
