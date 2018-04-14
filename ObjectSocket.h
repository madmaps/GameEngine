#ifndef __OBJECTSOCKET__
#define __OBJECTSOCKET__
#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "StandardObject.h"

class ObjectSocket
{
public:
	ObjectSocket();
	ObjectSocket(glm::vec3 inPosition, glm::quat inRotation);
	~ObjectSocket();
	virtual void setPosition(const glm::vec3& inPosition);
	virtual void setRotation(const glm::quat& inRotation);
	virtual glm::vec3 getPosition()const;
	virtual glm::quat getRotation()const;
	void setChildObject(StandardObject* inChildObject);
	StandardObject* getChildObject()const;
protected:
	glm::vec3 position;
	glm::quat rotation;
	StandardObject* childObject;
private:
};
#endif
