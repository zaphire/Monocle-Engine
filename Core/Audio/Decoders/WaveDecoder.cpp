//
//  WaveDecoder.cpp
//
//  Created by Josh Whelchel on 4/28/11.
//

#include "WaveDecoder.h"

namespace Monocle
{
    typedef struct { 
        short  wFormatTag; 
        short  nChannels; 
        int nSamplesPerSec; 
        int nAvgBytesPerSec; 
        short  nBlockAlign; 
        short  wBitsPerSample; 
        short  cbSize; 
    } WAVEFORMATEX; 
    
    class WaveDecoderData {
    public:
        WAVEFORMATEX format;
        AudioAssetReader *reader;
    };
    
    static short WAVE_FORMAT_PCM = 0x0001;
    
    void WaveDecoder::WaveOpen(WaveDecoderData *data)
    {
        char csID[10];
        int fsize;
        int wfxsize;
        int datasize;
        WAVEFORMATEX *wfx = &data->format;
        AudioAssetReader *reader = data->reader;
        
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
    
    AudioDecodeData *WaveDecoder::RequestData( AudioAsset *asset )
    {
        WaveDecoderData *data = new WaveDecoderData;
        AudioDecodeData *dd = new AudioDecodeData(44100, 16, 2, this, (void*)data, asset);
        
        AudioAssetReader *reader = new AudioAssetReader(dd->audAsset,dd->audAsset->GetDecodeString());
        data->reader = reader;
        
        WaveOpen(data);
        dd->samplerate = data->format.nSamplesPerSec;
        dd->bit = data->format.wBitsPerSample;
        dd->ch = data->format.nChannels;
        
        return dd;
    }
    
    unsigned long WaveDecoder::Render( unsigned long size, void *buf, AudioDecodeData &dd )
    {
        WaveDecoderData *wdd = (WaveDecoderData*)dd.decoderData;
        AudioAssetReader *reader = wdd->reader;
        
        size_t ret = reader->Read(buf, size);
        if (!ret)
            dd.outOfData = true;
        
        return ret;
    }
    
    void WaveDecoder::FreeDecoderData( AudioDecodeData &dd )
    {
        WaveDecoderData *wdd = (WaveDecoderData*)dd.decoderData;
        delete wdd->reader;
        delete wdd;
    }
}