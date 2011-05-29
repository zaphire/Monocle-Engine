//
//  WaveDecoder.cpp
//
//  Created by Josh Whelchel on 4/28/11.
//

#include "WaveDecoder.h"

namespace Monocle
{
    /***
     THIS CODE is responsible for registering this decoder with the file extension.
     See more info in Audio::Init.
     ***/
    AudioDecoder *makeWaveFunc( AudioAsset *asset )
    {
        return new WaveDecoder(asset);
    }
    
    static short WAVE_FORMAT_PCM = 0x0001;
    
    void WaveDecoder::WaveOpen()
    {
        char csID[10];
        int fsize;
        int wfxsize;
        int datasize;
        WAVEFORMATEX *wfx = &format;
        
        csID[4] = 0;
        reader->Read(csID,4);
        if (std::string (csID) != "RIFF"){
            return;
        }
        
        reader->Read ((char*)&fsize, 4);
        reader->Read (csID, 4);
        if (std::string (csID) != "WAVE"){
            std::string s (" is not a valid WAVE form-type.");
            throw s;
        }
        
        reader->Read (csID, 4);
        if (std::string (csID) != "fmt "){
            std::string s (" does not have a valid wave-form chunk ID.");
            throw s;
        }
        
        reader->Read ((char*)&wfxsize, 4);
        reader->Read ((char*)wfx, wfxsize);
        if (wfx->wFormatTag != WAVE_FORMAT_PCM){
            std::string s (" is not of type PCM wave format.");
            throw s;
        }
        
        reader->Read (csID, 4);
        if (std::string (csID) != "data"){
            std::string s (" does not have a valid data chunk ID.");
            throw s;
        }
        
        reader->Read ((char*)&datasize, 4);
        
        // Should be at data...
    }
    
    WaveDecoder::WaveDecoder( AudioAsset *asset )
    {
        this->audAsset = asset;
        reader = new AudioAssetReader(audAsset,audAsset->GetDecodeString());
        
        WaveOpen();
        
        Init(format.nSamplesPerSec,format.wBitsPerSample,format.nChannels);
    }
    
    unsigned long WaveDecoder::Render( unsigned long size, void *buf )
    {
        size_t ret = reader->Read(buf, size);
        if (!ret)
            outOfData = true;
        
        return ret;
    }
    
    WaveDecoder::~WaveDecoder()
    {
        delete reader;
    }
}




