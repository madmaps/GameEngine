#ifndef __STANDARDOBJECT__
#define __STANDARDOBJECT__
#include <vector>
#include "Component.h"
#include "ObjectSocket.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

class StandardObject
{
	public:
		StandardObject();
		~StandardObject();
		virtual void update() = 0;
		virtual void draw() = 0;
		virtual void setup() = 0;
		void addComponent(Component* inComponent);
		void addSockets(ObjectSocket* inSocket);
		glm::vec3 getPosition()const;
		glm::quat getRotation()const;
		void setPosition(const glm::vec3& inPosition);
		void setRotation(const glm::quat& inRotation);
	protected:
		glm::vec3 position;
		glm::vec3 scale;
		glm::quat rotation;
		std::vector<Component*> components;
		std::vector<ObjectSocket*> sockets;
	private:
		
};
#endif
