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
		OpenALSound(unsigned int inNumberOfBuffers=8, unsigned int inNumberOfVoices = 8);
		~OpenALSound();
        
        virtual void setup();
		virtual void update(double TimeLapse);
        
        virtual unsigned int playSound(unsigned int inSoundIndex);
        virtual unsigned int playSoundLoop(unsigned int inSoundIndex);
        virtual bool isSoundDonePlaying(unsigned int inSoundIndex);
        virtual void stopPlayingSound(unsigned int inSoundIndex);
        virtual void setGain(unsigned int inSoundIndex, const float inGain);
        
        virtual void setPosition(const glm::vec3& inPosition);
        virtual void setVelocity(const glm::vec3& inVelocity);
        virtual void setListenerPosition(const glm::vec3& inPosition);
        virtual void setListenerVelocity(const glm::vec3& inVelocity);
        virtual void setListenerOrientation(const glm::vec3& inForwardVector,const glm::vec3& inUpVector);
        
        void setRolloffRate(const float inRolloffRate);
        void addBuffer(ALuint inSoundBuffer,unsigned int inIndex);
	protected:
        std::vector<ALuint> soundBuffers;
        std::list<ALuint> sources;
        std::list<ALuint> usedSources;
        unsigned int numberOfVoices;
        float rolloffRate;
	private:
};
#endif
