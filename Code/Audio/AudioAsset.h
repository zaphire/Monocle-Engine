#pragma once

#include "Asset.h"
#include "AudioDecoder.h"

namespace Monocle
{
	class AudioAsset : public Asset
	{
	public:
        AudioAsset(): Asset(ASSET_AUDIO) { this->decoder = 0; }
		void Load(const std::string &filename, AudioDecoder &decoder);
		void Reload();
		void Unload();
        
    private:
        
        AudioDecoder *decoder;
	};
}