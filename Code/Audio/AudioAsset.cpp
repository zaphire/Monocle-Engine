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
    void AudioAsset::Load(const std::string &filename)
	{
        this->filename = filename;
	}
    
	void AudioAsset::Reload()
	{
		Unload();
		Load(filename);
	}
    
	void AudioAsset::Unload()
	{
		Debug::Log("Freeing audio memory for: " + this->filename);
	}
}