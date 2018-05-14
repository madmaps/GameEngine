#ifndef __OPENALSOUND__
#define __OPENALSOUND__
#include <vector>
#include <list>
#include <AL/al.h>
#include <AL/alc.h>
#include "ThreeDimSound.h"
#include "glm/vec3.hpp"

class OpenALSound : public ThreeDimSound
{
	public:
		OpenALSound(unsigned int inNumberOfVoices = 8);
		~OpenALSound();
        
        virtual void setup();
		virtual void update(double TimeLapse);
        
        virtual int playSound(unsigned int inSoundIndex);
        virtual int playSoundLoop(unsigned int inSoundIndex);
        virtual bool isSoundDonePlaying();
        virtual void stopPlayingSound(unsigned int inSoundIndex);
        virtual void setGain(unsigned int inSoundIndex);
        
        virtual void setPosition(const glm::vec3& inPosition);
        virtual void setVelocity(const glm::vec3& inVelocity);
        virtual void setListenerPosition(const glm::vec3& inPosition);
        virtual void setListenerVelocity(const glm::vec3& inVelocity);
        virtual void setListenerOrientation(const glm::vec3& inForwardVector,const glm::vec3& inUpVector);
        
        void addBuffer(ALuint inSoundBuffer,unsigned int inIndex);
	protected:
        std::vector<ALuint> soundBuffers;
        std::list<ALuint> sources;
        std::list<ALuint> usedSources;
        glm::vec3 position;
        glm::vec3 velocity;
        unsigned int numberOfVoices;
	private:
};
#endif
