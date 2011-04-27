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
#include "../AudioCrypt.h"
#include "../AudioAsset.h"

#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"

namespace Monocle {
    
    AudioCryptKey g_mKey("hellogirl");
    
    // Struct that contains the pointer to our file in memory
    struct SOggFile
    {
        char*		dataPtr;			// Pointer to the data in memoru
        long		dataSize;			// Sizeo fo the data
        
        long		dataRead;			// How much data we have read so far
        AudioCryptKey	*key;
        
    };
    
    typedef struct OggDecoderData_s {
        
        int sec;
        long seek;
        OggVorbis_File vf;
        
    } OggDecoderData;
    
    inline OggDecoderData *OGGDATA( AudioDecodeData *d )
    {
        return (OggDecoderData*)d->decoderData;
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
        size_t				spaceToEOF;			// How much more we can read till we hit the EOF marker
        size_t				actualSizeToRead;	// How much data we are actually going to read from memory
        SOggFile*			vorbisData;			// Our vorbis data, for the typecast
        
        // Get the data in the right format
        vorbisData = (SOggFile*)datasource;
        
        // Calculate how much we need to read.  This can be sizeToRead*byteSize or less depending on how near the EOF marker we are
        spaceToEOF = vorbisData->dataSize - vorbisData->dataRead;
        if ((sizeToRead*byteSize) < spaceToEOF)
            actualSizeToRead = (sizeToRead*byteSize);
        else
            actualSizeToRead = spaceToEOF;	
        
        // A simple copy of the data from memory to the datastruct that the vorbis libs will use
        if (actualSizeToRead)
        {
            // Copy the data from the start of the file PLUS how much we have already read in
            memcpy(ptr, (char*)vorbisData->dataPtr + vorbisData->dataRead, actualSizeToRead);
            
#ifdef DECODE_ONDEMAND
            // DECODE
            if (vorbisData->key)
            {
                // This part will be REALLY cool... o_O
                unsigned long offset;
                //			unsigned long size;
                //			unsigned long pos;
                
                offset = (vorbisData->dataRead) % 5;
                
                /*for (pos=0;pos<(unsigned long)actualSizeToRead;pos+=5)
                 {
                 if (actualSizeToRead-pos <= 5) 
                 size = actualSizeToRead-pos;
                 else 
                 size = 5;
                 
                 vorbisData->key->DecodeSlice5b((unsigned char*)vorbisData->dataPtr + vorbisData->dataRead + pos, (unsigned char*)ptr+pos, size, offset, spaceToEOF-pos);
                 }*/
                
                vorbisData->key->DecodeSlice5b((unsigned char*)vorbisData->dataPtr + vorbisData->dataRead, (unsigned char*)ptr, actualSizeToRead, offset, spaceToEOF);
            }
#endif
            
            // Increase by how much we have read by
            vorbisData->dataRead += (actualSizeToRead);
        }
        
        // Return how much we read (in the same way fread would)
        return actualSizeToRead;
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
        size_t				spaceToEOF;		// How much more we can read till we hit the EOF marker
        ogg_int64_t			actualOffset;	// How much we can actually offset it by
        SOggFile*			vorbisData;		// The data we passed in (for the typecast)
        
        // Get the data in the right format
        vorbisData = (SOggFile*)datasource;
        
        // Goto where we wish to seek to
        switch (whence)
        {
            case SEEK_SET: // Seek to the start of the data file
                // Make sure we are not going to the end of the file
                if (vorbisData->dataSize >= offset)
                    actualOffset = offset;
                else
                    actualOffset = vorbisData->dataSize;
                // Set where we now are
                vorbisData->dataRead = (int)actualOffset;
                break;
            case SEEK_CUR: // Seek from where we are
                // Make sure we dont go past the end
                spaceToEOF = vorbisData->dataSize - vorbisData->dataRead;
                if (offset < spaceToEOF)
                    actualOffset = (offset);
                else
                    actualOffset = spaceToEOF;	
                // Seek from our currrent location
                vorbisData->dataRead += actualOffset;
                break;
            case SEEK_END: // Seek from the end of the file
                vorbisData->dataRead = vorbisData->dataSize+1;
                break;
            default:
                printf("*** ERROR *** Unknown seek command in VorbisSeek\n");
                break;
        };
        
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
        SOggFile*	vorbisData;
        
        // Get the data in the right format
        vorbisData = (SOggFile*)datasource;
        free(vorbisData->dataPtr);
        free(datasource);
        return 1;
    }
    
    //---------------------------------------------------------------------------------
    // Function	: VorbisTell
    // Purpose	: Classback for the Vorbis tell function
    // Info		: 
    //---------------------------------------------------------------------------------
    long VorbisTell(void *datasource /*this is a pointer to the data we passed into ov_open_callbacks (our SOggFile struct*/)
    {
        SOggFile*	vorbisData;
        
        // Get the data in the right format
        vorbisData = (SOggFile*)datasource;
        
        // We just want to tell the vorbis libs how much we have read so far
        return vorbisData->dataRead;
    }
    /************************************************************************************************************************
     End of Vorbis callback functions
     ************************************************************************************************************************/
    
//    int WINAPI e_seek( onu_deck_cable_s *cable, long pos )
    static bool oggSeek( AudioDecodeData *dd, long pos )
    {
        OGGDATA(dd)->seek = (long)(((float)pos/1000.0f) * (float)dd->samplerate);
        if (!ov_seekable(&(OGGDATA(dd)->vf))) return false;
        
        return true;
    }
    
    static unsigned long oggRender( unsigned long size, void *buf, AudioDecodeData *dd )
    {
        OggDecoderData *data = OGGDATA(dd);
        
        int ret = 1;
        unsigned long pos=0;
        
        if (data->seek != -1)
        {
            ov_pcm_seek(&data->vf, data->seek);
            data->seek = -1;
            dd->outOfData = false;
        }
        
        while(ret && pos<size)
        {
            ret = ov_read(&data->vf, (char*)buf+pos, size-pos, 0, 2, 1, &data->sec);
            pos += ret;
        }
        
        // reached the and?
        if (!ret && (dd->loopsRemaining!=0))
        {
            // we are looping so restart from the beginning
            
            ret = 1;
            ov_pcm_seek(&data->vf, /*((data->cable->nLoopIn*(data->cable->samples))/1000)*/0);
            while(ret && pos<size)
            {
                ret = ov_read(&data->vf, (char*)buf+pos, size-pos, 0, 2, 1, &data->sec);
                pos += ret;
            }
            
            // Decrease from Loops Remaining (don't touch the -1 infinite loop stuff)
            if (dd->loopsRemaining > 0)
                dd->loopsRemaining--;
        }
        else if (!ret && (dd->loopsRemaining==0)){
            dd->outOfData = true;
        }
        
        return pos;
    }
    
    ov_callbacks vorbisCallbacks;
    
    bool isstrnum( char *str )
    {
        if (str[0] == 0) return true;
        if (str[0] == ' ' || isalnum(str[0])) return isstrnum(str+1);
        return false;
    }
    
    int oggInit( AudioDecodeData *dd )
    {
        OggDecoderData *data = OGGDATA(dd);
        data->seek = -1;
        
        bool bOk = 0;
        
        {
            // Now we have our file in memory (how ever it got there!), we need to let the vorbis libs know how to read it
            // To do this, we provide callback functions that enable us to do the reading.  the Vorbis libs just want the result
            // of the read.  They dont actually do it themselves
            // Save the function pointersof our read files...
            vorbisCallbacks.read_func = VorbisRead;
            vorbisCallbacks.close_func = VorbisClose;
            vorbisCallbacks.seek_func = VorbisSeek;
            vorbisCallbacks.tell_func = VorbisTell;
        }
        
        if (!bOk)
        {
            const char *cOpen = dd->audAsset->filename.c_str();
            SOggFile *sogg = (SOggFile*)malloc(sizeof(SOggFile));
            sogg->dataRead = 0;
            
            FILE *f = fopen(cOpen,"rb");
            
            if (!f){
                return 1;
            }
            else
            {
                fseek(f,0,SEEK_END);
                sogg->dataSize = ftell(f);
                fseek(f,0,SEEK_SET);
                
                sogg->dataPtr = (char*)malloc(sogg->dataSize);
                sogg->key = &g_mKey;
                
                fread(sogg->dataPtr,sogg->dataSize,1,f);
                
                fclose(f);
                
                if (ov_open_callbacks( sogg, &data->vf, NULL, 0, vorbisCallbacks )<0){
                    f = fopen(cOpen,"rb");
                    if (!f || ov_open(f,&data->vf,0,0)<0){
                        return 1;
                    }
                }
            }
        }
        
        {
            vorbis_info *vi=ov_info(&data->vf,-1);
            
            if (!vi)
            {
                return 1;
            }
            
            dd->samplerate = vi->rate;
            dd->ch = vi->channels;
        }
        
        dd->total = (long)(((float)ov_pcm_total(&data->vf,-1)/(float)dd->samplerate) * 1000.0f); 
        
        return 0;
    }
    
    void oggFree( AudioDecodeData *dd )
    {
        OggDecoderData *data = OGGDATA(dd);
        ov_clear(&data->vf);
        free(data);
        dd->decoderData = 0;
        return;
    }
    
    AudioDecodeData *OggDecoder::RequestData( AudioAsset &asset )
    {
        OggDecoderData *data = new OggDecoderData;
        AudioDecodeData *dd = new AudioDecodeData(44100, 16, 2, this, (void*)data, &asset);
        
        if (oggInit(dd)) {
            // Error
            oggFree(dd);
            return 0;
        }
        
        return dd;
    }
    
    unsigned long OggDecoder::Render( unsigned long size, void *buf, AudioDecodeData &dd )
    {
        return oggRender(size, buf, &dd);
    }
    
    void OggDecoder::FreeDecoderData( AudioDecodeData &dd )
    {
        oggFree(&dd);
    }
    
}

#endif