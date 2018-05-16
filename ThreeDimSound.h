#ifndef __THREEDIMSOUND__
#define __THREEDIMSOUND__
#include "SoundDevice.h"
#include "glm/vec3.hpp"


class ThreeDimSound : public SoundDevice
{
	public:
		ThreeDimSound();
		~ThreeDimSound();
        virtual void setPosition(const glm::vec3& inPosition) = 0;
        virtual void setVelocity(const glm::vec3& inVelocity) = 0;
        virtual void setListenerPosition(const glm::vec3& inPosition) = 0;
        virtual void setListenerVelocity(const glm::vec3& inVelocity) = 0;
        virtual void setListenerOrientation(const glm::vec3& inForwardVector,const glm::vec3& inUpVector) = 0;
        virtual void setUpdateListener(const bool inUpdateListener);
	protected:
        glm::vec3 position;
        glm::vec3 velocity;
        static glm::vec3 listenerPosition;
        static glm::vec3 listenerVelocity;
        static glm::vec3 listenerOrientationForward;
        static glm::vec3 listenerOrientationUp;
        bool updateListener;
	private:
};
#endif
