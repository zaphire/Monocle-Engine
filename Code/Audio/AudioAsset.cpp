//
//  AudioAsset.cpp
//  MonocleTest
//
//  Created by Josh Whelchel on 4/19/11.
//

#include "AudioAsset.h"
#include "../Debug.h"

namespace Monocle
{
    void AudioAsset::Load(const std::string &filename, AudioDecoder &decoder)
	{
		this->decoder = &decoder;    
        this->filename = filename;
	}
    
	void AudioAsset::Reload()
	{
		Unload();
		Load(filename, *this->decoder);
	}
    
	void AudioAsset::Unload()
	{
		Debug::Log("Freeing audio memory for: " + this->filename);
	}
}