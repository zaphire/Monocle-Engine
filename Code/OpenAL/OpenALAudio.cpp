#ifdef MONOCLE_OPENAL

#include "../Audio.h"
#include "../Debug.h"

#include "FmodOpenALBridge.h"

namespace Monocle
{
	Audio::Audio()
	{
	}

	void Audio::Init()
	{
		Debug::Log("Audio::Init...");
        
        ChannelStream::init();

		Debug::Log("...Done");
	}

	void Audio::PlaySFX(const std::string &sfx)
	{
		//if (Cast *cast = (Cast*)sfxMap[sfx])
		{
		}
	}
}

#endif