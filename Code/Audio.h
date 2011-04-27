#pragma once

#include <string>
#include <map>

#include "Audio/ChannelStream.h"
#include "Audio/AudioAsset.h"
#include "Audio/AudioDecoder.h"

namespace Monocle
{
	class Audio
	{
	public:
		Audio();
		void Init();
        
        void Update();

		void PlaySFX(const std::string &sfx);
		
		std::map<std::string, void*> sfxMap;
        std::map<std::string, void*> musMap;
	};
}