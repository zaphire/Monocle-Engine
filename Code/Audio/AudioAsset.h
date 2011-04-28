#pragma once

#include "../Asset.h"
#include "AudioDecoder.h"

namespace Monocle
{
	class AudioAsset : public Asset
	{
	public:
        AudioAsset();
        ~AudioAsset();
        
		void Load( const std::string &filename, bool streamFromDisk );
		void Reload();
		void Unload();
        
        bool IsStreaming();
        
        void *GetDataBuffer();
        long GetDataSize();
        
        void SetDecodeString( std::string decodeString );
        std::string GetDecodeString();
        
    private:
        
        long size;
        bool streamFromDisk;
        void *dataBuffer;

        std::string decodeString;
    
	};
}