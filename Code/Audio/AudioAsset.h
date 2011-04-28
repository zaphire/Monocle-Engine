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
        
        /**
            Prepares the asset for decoding.
         
            @param streamFromDisk true if the asset should be streamed from disk or false if the asset should be loaded into memory.
         */
		void Load( const std::string &filename, bool streamFromDisk );
		void Reload();
		void Unload();
        
        /**
            @return true if the asset is to be streamed from disk
         */
        bool IsStreaming();
        
        /**
            If the asset isn't streaming from disk, this will return the pointer to the data buffer.
            
            @see GetDataSize()
            @return Pointer to the data.
         */
        void *GetDataBuffer();
        
        /**
            This returns the size in bytes of the asset, regardless of whether or not it is streaming from disk.
         
            @return the size in bytes of the asset, regardless of whether or not it is streaming from disk.
         */
        long GetDataSize();
        
        /**
            Sets the decode string for use by AudioCrypt. If the asset is not encoded, send ""
         */
        void SetDecodeString( std::string decodeString );
        
        std::string GetDecodeString();
        
    private:
        
        long size;
        bool streamFromDisk;
        void *dataBuffer;

        std::string decodeString;
    
	};
}