//
//  AudioAsset.cpp
//  MonocleTest
//
//  Created by Josh Whelchel on 4/19/11.
//

#include "Audio.h"
#include "AudioAsset.h"
#include "../MonocleToolkit.h"
#include "../Debug.h"

#include <stdio.h>
#include <stdlib.h> // required for free and malloc (GCC)

namespace Monocle
{
    AudioAsset::AudioAsset() : Asset(ASSET_AUDIO)
    {
        size = 0;
        streamFromDisk = false;
        dataBuffer = 0;
        decodeString = "";
    }
	
	/// Alec: Removed destructor.
	/// These objects get deleted when you call RemoveReference() (if the references run out)
    
    void AudioAsset::Load(const std::string &filename, bool streamFromDisk)
	{
        this->filename = filename;
        
        this->streamFromDisk = streamFromDisk;
        
        FILE *f;
        f = fopen(this->filename.c_str(),"rb");
        
        if (!f){
            Debug::Log("Asset " + filename + " was not found and cannot be loaded."); // ERROR
            return;
        }
        
        fseek(f,0,SEEK_END);
        size = ftell(f);
        fseek(f,0,SEEK_SET);
        
        if (!streamFromDisk){
            // Load to memory!
            dataBuffer = malloc(size);
            fread(dataBuffer,size,1,f);
            
            Debug::Log("Commited " + StringOf(size) + " bytes to memory for " + GetName());
        }
        
        fclose(f);
	}
    
	void AudioAsset::Reload()
	{
		Unload();
		Load(this->filename,streamFromDisk);
	}
    
	void AudioAsset::Unload()
	{
		Debug::Log("Freeing audio memory for: " + this->filename);
        
        if (dataBuffer)
		{
            free(dataBuffer);
			dataBuffer = NULL;
		}
	}
    
    bool AudioAsset::IsStreaming()
    {
        return streamFromDisk;
    }
    
    void *AudioAsset::GetDataBuffer()
    {
        return dataBuffer;
    }
    
    long AudioAsset::GetDataSize()
    {
        return size;
    }
    
    void AudioAsset::SetDecodeString(std::string decodeString)
    {
        this->decodeString = decodeString;
    }
    
    std::string AudioAsset::GetDecodeString()
    {
        return this->decodeString;
    }
    
    void AudioAsset::Play( int loops, float volume, float pan, float pitch )
    {
        Audio::PlaySound(this,loops,volume,pan,pitch);
    }
}
