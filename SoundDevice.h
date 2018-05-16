#ifndef __SOUNDDEVICE__
#define __SOUNDDEVICE__
#include "Component.h"

class SoundDevice : public Component
{
	public:
		SoundDevice();
		~SoundDevice();
        virtual unsigned int playSound(unsigned int inSoundIndex) = 0;
        virtual unsigned int playSoundLoop(unsigned int inSoundIndex) = 0;
        virtual bool isSoundDonePlaying(unsigned int inSoundIndex) = 0;
        virtual void stopPlayingSound(unsigned int inSoundIndex) = 0;
        virtual void setGain(unsigned int inSoundIndex,const float inGain) = 0;
	protected:
	private:
};
#endif
