#ifdef MONOCLE_AUDIO_OGG

//
//  OggDecoder.cpp
//  MonocleTest
//
//  Created by Josh Whelchel on 4/19/11.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "OggDecoder.h"
#include "../../Debug.h"
#include "../AudioAsset.h"

#include "../AudioAssetReader.h"

#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"

namespace Monocle {
    
    /***
        THIS CODE is responsible for registering this decoder with the file extension.
        See more info in Audio::Init.
     ***/
    AudioDecoder *makeOggFunc( AudioAsset *asset )
    {
        return new OggDecoder(asset);
    }
    
    // Struct that contains the pointer to our file in memory
    /*struct SOggFile
    {
        AudioAssetReader *reader;
    };*/
    
    typedef struct OggDecoderData_s {
        
        int sec;
        OggVorbis_File vf;
        
    } OggDecoderData;
    
    inline OggDecoderData *OGGDATA( OggDecoder *d )
    {
        return (OggDecoderData*)d->oggData;
    }
    
    //---------------------------------------------------------------------------------
    // Function	: VorbisRead
    // Purpose	: Callback for the Vorbis read function
    // Info		: 
    //---------------------------------------------------------------------------------
    size_t VorbisRead(void *ptr			/* ptr to the data that the vorbis files need*/, 
                      size_t byteSize	/* how big a byte is*/, 
                      size_t sizeToRead /* How much we can read*/, 
                      void *datasource	/* this is a pointer to the data we passed into ov_open_callbacks (our SOggFile struct*/)
    {
        AudioAssetReader*	vorbisData;			// Our vorbis data, for the typecast
        
        // Get the data in the right format
        vorbisData = (AudioAssetReader*)datasource;

        return vorbisData->Read(ptr,sizeToRead*byteSize);
    }
    
    //---------------------------------------------------------------------------------
    // Function	: VorbisSeek
    // Purpose	: Callback for the Vorbis seek function
    // Info		: 
    //---------------------------------------------------------------------------------
    int VorbisSeek(void *datasource		/*this is a pointer to the data we passed into ov_open_callbacks (our SOggFile struct*/, 
                   ogg_int64_t offset	/*offset from the point we wish to seek to*/, 
                   int whence			/*where we want to seek to*/)
    {
        AudioAssetReader*	vorbisData;		// The data we passed in (for the typecast)
        
        // Get the data in the right format
        vorbisData = (AudioAssetReader*)datasource;
        vorbisData->Seek( offset, whence );
        
        return 0;
    }
    
    //---------------------------------------------------------------------------------
    // Function	: VorbisClose
    // Purpose	: Callback for the Vorbis close function
    // Info		: 
    //---------------------------------------------------------------------------------
    int VorbisClose(void *datasource /*this is a pointer to the data we passed into ov_open_callbacks (our SOggFile struct*/)
    {
        // This file is called when we call ov_close.  If we wanted, we could free our memory here, but
        // in this case, we will free the memory in the main body of the program, so dont do anything
        AudioAssetReader*	vorbisData;		// The data we passed in (for the typecast)
        
        // Get the data in the right format
        vorbisData = (AudioAssetReader*)datasource;
        
        delete vorbisData;
        return 1;
    }
    
    //---------------------------------------------------------------------------------
    // Function	: VorbisTell
    // Purpose	: Classback for the Vorbis tell function
    // Info		: 
    //---------------------------------------------------------------------------------
    long VorbisTell(void *datasource /*this is a pointer to the data we passed into ov_open_callbacks (our SOggFile struct*/)
    {
        AudioAssetReader*	vorbisData;		// The data we passed in (for the typecast)
        
        // Get the data in the right format
        vorbisData = (AudioAssetReader*)datasource;
        
        // We just want to tell the vorbis libs how much we have read so far
        return vorbisData->Tell();
    }
    /************************************************************************************************************************
     End of Vorbis callback functions
     ************************************************************************************************************************/
    
//    int WINAPI e_seek( onu_deck_cable_s *cable, long pos )
    static bool oggSeek( OggDecoder *dd, long pos )
    {
        dd->seekOffset = pos;
        if (!ov_seekable(&(OGGDATA(dd)->vf))) return false;
        
        return true;
    }
    
    ov_callbacks vorbisCallbacks;
    
    bool isstrnum( char *str )
    {
        if (str[0] == 0) return true;
        if (str[0] == ' ' || isalnum(str[0])) return isstrnum(str+1);
        return false;
    }
    
    unsigned long OggDecoder::Render( unsigned long size, void *buf )
    {
        OggDecoderData *data = OGGDATA(this);
        
        if (!data){
            outOfData = true;
            return 0;
        }
        
        int ret = 1;
        unsigned long pos=0;
        
        if (seekOffset != -1)
        {
            long seek = (long)(((float)seekOffset/1000.0f) * (float)samplerate);
            ov_pcm_seek(&data->vf, seek);
            seekOffset = -1;
            outOfData = false;
        }
        
        while(ret && pos<size)
        {
            ret = ov_read(&data->vf, (char*)buf+pos, size-pos, 0, 2, 1, &data->sec);
            pos += ret;
        }
        
        // reached the end?
        if (!ret && (loopsRemaining!=0))
        {
            // we are looping so restart from the beginning
            ret = 1;
            ov_pcm_seek(&data->vf, 0);  // Loop back position, TODO: If we want to loop to a specific point, here is the code for that!
            while(ret && pos<size)
            {
                ret = ov_read(&data->vf, (char*)buf+pos, size-pos, 0, 2, 1, &data->sec);
                pos += ret;
            }
            
            // Decrease from Loops Remaining (don't touch the -1 infinite loop stuff)
            if (loopsRemaining > 0)
                loopsRemaining--;
        }
        else if (!ret && (loopsRemaining==0)){
            outOfData = true;
        }
        
        return pos;
    }
    
    OggDecoder::OggDecoder( AudioAsset *asset )
    {
        Init(44100,16,2);
        this->audAsset = asset;
        
        oggData = new OggDecoderData();
        OggDecoderData *data = OGGDATA(this);
        
        vorbisCallbacks.read_func = VorbisRead;
        vorbisCallbacks.close_func = VorbisClose;
        vorbisCallbacks.seek_func = VorbisSeek;
        vorbisCallbacks.tell_func = VorbisTell;
        
        AudioAssetReader *reader = new AudioAssetReader(audAsset,audAsset->GetDecodeString());
        
        if (ov_open_callbacks( reader, &data->vf, NULL, 0, vorbisCallbacks )<0){
            delete reader;
            delete (OggDecoderData*)oggData;
            oggData = NULL;
            reader = NULL;
        
            return;
        }
        
        vorbis_info *vi=ov_info(&data->vf,-1);
        
        if (!vi)
        {
            delete reader;
            delete (OggDecoderData*)oggData;
            oggData = NULL;
            reader = NULL;
            
            return;
        }
        
        samplerate = (int)vi->rate;
        ch = vi->channels;
        
        total = (long)(((float)ov_pcm_total(&data->vf,-1)/(float)samplerate) * 1000.0f); 
    }
    
    OggDecoder::~OggDecoder()
    {
        OggDecoderData *data = OGGDATA(this);
        ov_clear(&data->vf);
        delete data;
    }
    
}

#endif