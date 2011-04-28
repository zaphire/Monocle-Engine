#pragma once
//
//  AudioVis.h
//
//  Created by Josh Whelchel on 4/26/11.
//

#ifndef WINAPI
#define WINAPI void
#endif

#include "VisCache2.h"

namespace Monocle {
    // Contains visualization data (:
    
    class AudioVisData;
    
    class AudioVis
    {
    public:
        
        AudioVis();
        ~AudioVis();
        
        void PrepData();
        
        // Vis Data
        float imm[2][16];
        float avg[2][16];
        float med_avg[2][16];
        float long_avg[2][16];
        
        unsigned char cWaveformR[576];
        unsigned char cWaveformL[576];
        float fWaveform[2][576];
        float fSpectrum[2][512];
        
        bool bClear;				// True when vis should be clear.
        int bIsBeat;				// 0 when not a beat, 1 on a beat (detected :3)
        
        long engineerData[4];			// Current data from the cache
        
        unsigned long updateCount;
        
        bool bDisabled;				// This is set when the freuqnecy is changed.

        AudioVisData *data;
        
        void AnalyzeNewSound(long rsize);
        void BeatDetect();
        
        int _usedCount;
    };
}