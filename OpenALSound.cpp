#include "OpenALSound.h"


OpenALSound::OpenALSound(unsigned int inNumberOfVoices)
{
    numberOfVoices = inNumberOfVoices;
    ALuint tempSource;
    for(unsigned int i = 0; i < inNumberOfVoices; i++)
    {
        alGenSources((ALuint)1, &tempSource);
        sources.push_back(tempSource);
    }
    for(unsigned int i = 0; i < 16; i++)
    {
        soundBuffers.push_back(0);
    }
}

OpenALSound::~OpenALSound()
{
}

void OpenALSound::setup()
{
}

void OpenALSound::update(double TimeLapse)
{
}

int OpenALSound::playSound(unsigned int inSoundIndex)
{
    ALuint tempSource = 0;
    ALuint sourceState = 0;
    std::list<ALuint>::iterator iter;
    for(std::list<ALuint>::iterator completeSource = usedSources.begin(); completeSource != usedSources.end(); completeSource++)
    {
        alSourcei(*completeSource,AL_SOURCE_STATE,sourceState);
        if(sourceState == AL_STOPPED)
        {
            sources.push_back(*completeSource);
            completeSource = usedSources.erase(completeSource);
        }
    }
    if(sources.size() > 0)
    {
        tempSource = sources.back();
        usedSources.push_back(tempSource);
        sources.pop_back();
        alSourcef(tempSource, AL_PITCH, 1);
        alSourcef(tempSource, AL_GAIN, 1);
        alSource3f(tempSource, AL_POSITION, position.x, position.y, position.z);
        alSource3f(tempSource, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
        alSourcei(tempSource, AL_LOOPING, AL_FALSE);
        alSourcei(tempSource, AL_BUFFER, soundBuffers.at(inSoundIndex));
        alSourcePlay(tempSource);
    }
    return tempSource;
}

int OpenALSound::playSoundLoop(unsigned int inSoundIndex)
{
    return 1;
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
