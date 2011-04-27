#pragma once

#include "../Asset.h"
#include "AudioDecoder.h"

namespace Monocle
{
	class AudioAsset : public Asset
	{
	public:
        AudioAsset(): Asset(ASSET_AUDIO) {  }
		void Load( const std::string &filename );
		void Reload();
		void Unload();
        
    private:
    
	};
}