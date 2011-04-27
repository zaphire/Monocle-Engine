#pragma once

#include <string>
#include <map>

#include "ChannelStream.h"
#include "AudioDeck.h"
#include "AudioAsset.h"
#include "AudioDecoder.h"
#include "AudioVis.h"

namespace Monocle
{
	class Audio
	{
	public:
		Audio();
        ~Audio();
        
		void Init();
        void Update();
        
		static void PlaySFX(const std::string &sfx);
        
        static void PauseAll();
        static void ResumeAll();
        
        static AudioDeck *NewDeck(AudioDecodeData *decodeData);
        static AudioDeck *LastDeck();
        
    private:
        
        static Audio *instance;
        
        std::map<std::string, AudioDecoder *> decoderMap;
        
        AudioDeck *firstDeck;
		
		std::map<std::string, void*> sfxMap;
        std::map<std::string, void*> musMap;
	};
}