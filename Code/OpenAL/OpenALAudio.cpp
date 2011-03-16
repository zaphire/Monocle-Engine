#ifdef MONOCLE_OPENAL

#include "../Audio.h"
#include "../Debug.h"

namespace Monocle
{
	Audio::Audio()
	{
	}

	void Audio::Init()
	{
		Debug::Log("Audio::Init...");

		Debug::Log("...Done");
	}
}

#endif