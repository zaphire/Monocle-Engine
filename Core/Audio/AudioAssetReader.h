//
//  AudioAssetReader.h
//
//  Created by Josh Whelchel on 4/27/11.
//

#pragma once

#include "AudioAsset.h"
#include "AudioCrypt.h"

namespace Monocle {
    
    /*
     * AudioAssetReader is designed to include a file/memory pointer for reading from audio assets either on disk or in memory
     */
    class AudioAssetReader
    {
    public:
        
        AudioAssetReader( AudioAsset *asset, std::string key = "" );
        ~AudioAssetReader();
        
        size_t      SpaceUntilEOF();
        size_t      Read( void *dst, size_t size );
        void        Seek( size_t offset, int whence );
        size_t      Tell();                             // Gets the position we're at (dataRead)
        
        long        GetSize();
        
    private:
        
        // When Reading from Buffer
        char*		dataPtr;			// Pointer to the data in memory
        long		dataSize;			// Sizeo fo the data
        
        long		dataRead;			// How much data we have read so far
        
        // When Streaming
        FILE        *file;
        
        AudioAsset *asset;
        AudioCryptKey	*cryptKey;
        
    };
    
}