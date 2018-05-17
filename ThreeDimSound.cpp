#include "ThreeDimSound.h"


glm::vec3 ThreeDimSound::listenerPosition = glm::vec3(0.0f, 0.0f, 0.0f);

glm::vec3 ThreeDimSound::listenerVelocity = glm::vec3(0.0f, 0.0f, 0.0f);

glm::vec3 ThreeDimSound::listenerOrientationForward = glm::vec3(0.0f, 0.0f, -1.0f);

glm::vec3 ThreeDimSound::listenerOrientationUp = glm::vec3(0.0f, 1.0f, 0.0f);


ThreeDimSound::ThreeDimSound()
{}

ThreeDimSound::~ThreeDimSound()
{}

void ThreeDimSound::setUpdateListener(const bool inUpdateListener)
{
    updateListener = inUpdateListener;
}
