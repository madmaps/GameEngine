#include "OpenALSound.h"


OpenALSound::OpenALSound(unsigned int inNumberOfVoices)
{
    numberOfVoices = inNumberOfVoices;
    sources = new ALuint[inNumberOfVoices];
    for(unsigned int i = 0; i < inNumberOfVoices; i++)
    {
        alGenSources((ALuint)1, &sources[i]);
    }
    for(unsigned int i = 0; i < 16; i++)
    {
        soundBuffers.push_back(0);
    }
}

OpenALSound::~OpenALSound()
{
    delete[] sources;
}

void OpenALSound::setup()
{
}

void OpenALSound::update(double TimeLapse)
{
}

void OpenALSound::playSound(unsigned int inSoundIndex)
{
}

void OpenALSound::playSoundLoop(unsigned int inSoundIndex)
{
}

bool OpenALSound::isSoundDonePlaying()
{
    return true;
}

void OpenALSound::stopPlayingSound(unsigned int inSoundIndex)
{
}

void OpenALSound::setGain(unsigned int inSoundIndex)
{
}

void OpenALSound::setPosition(const glm::vec3& inPosition)
{
}

void OpenALSound::setVelocity(const glm::vec3& inVelocity)
{
}

void OpenALSound::setListenerPosition(const glm::vec3& inPosition)
{
}

void OpenALSound::setListenerVelocity(const glm::vec3& inVelocity)
{
}

void OpenALSound::setListenerOrientation(const glm::vec3& inForwardVector,const glm::vec3& inUpVector)
{
}

void OpenALSound::addBuffer(ALuint inSoundBuffer,unsigned int inIndex)
{
}


/*protected:
std::vector<ALuint> soundBuffers;
ALuint* sources;
*         glm::vec3 position;
        glm::vec3 velocity;
        static glm::vec3 listenerPosition;
        static glm::vec3 listenerOrientationForward;
        static glm::vec3 listenerOrientationUp;
*/
