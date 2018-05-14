#ifndef __SOUNDDEVICE__
#define __SOUNDDEVICE__
#include "Component.h"

class SoundDevice : public Component
{
	public:
		SoundDevice();
		~SoundDevice();
        virtual int playSound(unsigned int inSoundIndex) = 0;
        virtual int playSoundLoop(unsigned int inSoundIndex) = 0;
        virtual bool isSoundDonePlaying() = 0;
        virtual void stopPlayingSound(unsigned int inSoundIndex) = 0;
        virtual void setGain(unsigned int inSoundIndex) = 0;
	protected:
	private:
};
#endif
