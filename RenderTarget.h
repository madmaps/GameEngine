#ifndef __RENDERTARGET__
#define __RENDERTARGET__

class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();
	virtual void draw() = 0;


protected:

private:
};
#endif
