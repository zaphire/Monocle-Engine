#pragma once

#include "../Asset.h"
#include "AudioDecoder.h"

namespace Monocle
{
	class AudioAsset : public Asset
	{
	public:
        AudioAsset();
        
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

        /**
         Plays an audio file. The deck it plays on will be freed automatically when the sound is complete.
         
         @param loops Number of times to loop
         @param volume Volume, between 0.0 and 1.0
         @param pan Panning, between -1.0(L) and 1.0(R), 0.0 for center
         @param pitch Pitch, 0.5 - 2.0
         
         @see Audio::PlaySound
         */
        void Play( int loops = 1, float volume = 1.0, float pan = 0.0, float pitch = 1.0 );
        
    private:
        
        long size;
        bool streamFromDisk;
        void *dataBuffer;

        std::string decodeString;
    
	};
}