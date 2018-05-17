#include "OpenALSound.h"
#include <iostream>


OpenALSound::OpenALSound(unsigned int inNumberOfBuffers, unsigned int inNumberOfVoices)
{
    rolloffRate = 1.0f;
    numberOfVoices = inNumberOfVoices;
    ALuint tempSource;
    for(unsigned int i = 0; i < inNumberOfVoices; i++)
    {
        alGenSources((ALuint)1, &tempSource);
        sources.push_back(tempSource);
    }
    for(unsigned int i = 0; i < inNumberOfBuffers; i++)
    {
        soundBuffers.push_back(0);
    }
}

OpenALSound::~OpenALSound()
{
    for(std::list<ALuint>::iterator completeSource = usedSources.begin(); completeSource != usedSources.end(); completeSource++)
    {
        alDeleteSources(1, &(*completeSource));
    }
    for(std::list<ALuint>::iterator unusedSources = sources.begin(); unusedSources != sources.end(); unusedSources++)
    {
        alDeleteSources(1, &(*unusedSources));
    }
}

void OpenALSound::setup()
{
}

void OpenALSound::update(double TimeLapse)
{
    for(std::list<ALuint>::iterator completeSource = usedSources.begin(); completeSource != usedSources.end(); completeSource++)
    {
        alSource3f(*completeSource, AL_POSITION, position.x, position.y, position.z);
        alSource3f(*completeSource, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
    }
    if(updateListener)
    {
        alListener3f(AL_POSITION, listenerPosition.x, listenerPosition.y, listenerPosition.z);
        alListener3f(AL_VELOCITY, listenerVelocity.x, listenerVelocity.y, listenerVelocity.z);
        float temp[6];
        temp[0] = listenerOrientationForward.x;
        temp[1] = listenerOrientationForward.y;
        temp[2] = listenerOrientationForward.z;
        temp[3] = listenerOrientationUp.x;
        temp[4] = listenerOrientationUp.y;
        temp[5] = listenerOrientationUp.z;
        alListenerfv(AL_ORIENTATION, temp);
    }

}

unsigned int OpenALSound::playSound(unsigned int inSoundIndex)
{
    ALuint tempSource = 0;
    ALint sourceState = 0;
    for(std::list<ALuint>::iterator completeSource = usedSources.begin(); completeSource != usedSources.end(); completeSource++)
    {
        alGetSourcei(*completeSource,AL_SOURCE_STATE,&sourceState);
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
        alSourcef(tempSource, AL_ROLLOFF_FACTOR, rolloffRate);
        alSourcei(tempSource, AL_LOOPING, AL_FALSE);
        alSourcei(tempSource, AL_BUFFER, soundBuffers.at(inSoundIndex));
        alSourcePlay(tempSource);
    }
    return tempSource;
}

unsigned int OpenALSound::playSoundLoop(unsigned int inSoundIndex)
{
    ALuint tempSource = 0;
    ALint sourceState = 0;
    for(std::list<ALuint>::iterator completeSource = usedSources.begin(); completeSource != usedSources.end(); completeSource++)
    {
        alGetSourcei(*completeSource,AL_SOURCE_STATE,&sourceState);
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
        alSourcei(tempSource, AL_LOOPING, AL_TRUE);
        alSourcei(tempSource, AL_BUFFER, soundBuffers.at(inSoundIndex));
        alSourcePlay(tempSource);
    }
    return tempSource;
}

bool OpenALSound::isSoundDonePlaying(unsigned int inSoundIndex)
{
    bool returnValue = true;
    ALint sourceState = 0;
    std::list<ALuint>::iterator iter;
    for(std::list<ALuint>::iterator completeSource = usedSources.begin(); completeSource != usedSources.end(); completeSource++)
    {
        if(inSoundIndex == *completeSource)
        {
            alGetSourcei(*completeSource,AL_SOURCE_STATE,&sourceState);
            if(sourceState == AL_PLAYING)
            {
                returnValue = false;
            }
        }
    }
    return returnValue;
}

void OpenALSound::stopPlayingSound(unsigned int inSoundIndex)
{
    alSourceStop(inSoundIndex);
}

void OpenALSound::setGain(unsigned int inSoundIndex, const float inGain)
{
    alSourcef(inSoundIndex, AL_GAIN, inGain);
}

void OpenALSound::setPosition(const glm::vec3& inPosition)
{
    position = inPosition;
}

void OpenALSound::setVelocity(const glm::vec3& inVelocity)
{
    velocity = inVelocity;
}

void OpenALSound::setListenerPosition(const glm::vec3& inPosition)
{
    OpenALSound::listenerPosition = inPosition;
}

void OpenALSound::setListenerVelocity(const glm::vec3& inVelocity)
{
    OpenALSound::listenerVelocity = inVelocity;
}

void OpenALSound::setListenerOrientation(const glm::vec3& inForwardVector,const glm::vec3& inUpVector)
{
    OpenALSound::listenerOrientationForward = inForwardVector;
    OpenALSound::listenerOrientationUp = inUpVector;
}

void OpenALSound::addBuffer(ALuint inSoundBuffer,unsigned int inIndex)
{
    soundBuffers.at(inIndex) = inSoundBuffer;
}

void OpenALSound::setRolloffRate(const float inRolloffRate)
{
    rolloffRate = inRolloffRate;
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
