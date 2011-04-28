//
//  AudioDecoder.h
//  MonocleTest
//
//  Created by Josh Whelchel on 4/19/11.
//

#pragma once

#include "VisCache2.h"

namespace Monocle
{
    class AudioAsset;
    class AudioDecoder;
    
    class AudioDecodeData
    {
    public:
        
        AudioDecodeData( int samplerate, int bit, int ch, AudioDecoder *decoder, void *decoderData, AudioAsset *audAsset );
        ~AudioDecodeData();
        
        // Source Sound info... (set by the decoder)
        int samplerate;			// Sample Rate (44100, 22050, etc)
        int bit;				// Bits (8, 16)
        int ch;					// Channels (1, 2)
        
        // # Loops remaining
        // This value is initially set by the scene, and should then be handled by the decoder.
        // A value of -1 should represent infinite looping.
        int loopsRemaining;
        
        unsigned long total;	// Total # of milliseconds in track (ignore looping in this calculation!)
        long seekOffset;		// milliseconds in track that was 'seeked' to, in order to return a proper time (-1 for no seek)
        
        bool outOfData;				// Set to true when no more data is to be generated (and the decoder has exhausted all loops...)
      
        AudioDecoder *decoder;
        AudioAsset *audAsset;
        
        // Decoder Specific Data
        void *decoderData;
    };
    
    class AudioDecoder
    {
    public:
        
        AudioDecoder() { };
        ~AudioDecoder() { };
        
        virtual AudioDecodeData *RequestData( AudioAsset *asset )=0;
        virtual unsigned long Render( unsigned long size, void *outputBuffer, AudioDecodeData &decodeData)=0;
        
        virtual void FreeDecoderData( AudioDecodeData &dd )=0;
        
    protected:
        
    };
}

#ifdef MONOCLE_AUDIO_OGG
#include "oggvorbis/OggDecoder.h"
#endif

/*
 typedef int (WINAPI * onu_init_callback)( onu_deck_cable_s *cable );
 typedef void (WINAPI * onu_free_callback)( onu_deck_cable_s *cable );
 typedef int (WINAPI * onu_getversion_callback)( );
 typedef void (WINAPI * onu_freecable_callback)( onu_deck_cable_s *cable );
 typedef int (WINAPI * onu_seek_callback)( onu_deck_cable_s *cable, long pos );
 typedef void (WINAPI * onu_setcacheinfo_callback)( onu_deck_cable_s *cable, long *data1, long *data2, long *data3, long *data4 );
*/