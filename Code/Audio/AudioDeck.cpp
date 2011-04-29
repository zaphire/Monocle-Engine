//
//  AudioDeck.cpp
//
//  Created by Josh Whelchel on 4/26/11.
//

#include <string.h>

#include "AudioDeck.h"
#include "../Platform.h"
#include "../Debug.h"

#define MIN(a,b) (a<b)?a:b

namespace Monocle {
    
    bool AudioDeck::UpdateFades()
    {
        if (pause)  // also check threads?
            return false;
        
        if (fades.bSilent){
            cs->SetVolume(0.0);
        }
        else if (fades.nFadeIn || fades.nFadeOut || fades.aFadeInStart || fades.aFadeOutStart)
        {
            float vol = 1.0f;
            unsigned long fadeinend = this->fades.nFadeIn;
            unsigned long total = decodeData->total;
            unsigned long fadeoutstart;
            unsigned long pos = cs->GetTotalPlayTime();
            unsigned long opos = GetCurrentTime();
            
            // Fix if we go past the fadein point so it doesn't happen again o_O
            if (pos >= this->fades.aFadeInEnd)
                this->fades.aFadeInEnd = this->fades.aFadeInStart = 0;
            
            fadeoutstart =  total - this->fades.nFadeOut;
            
            /** TODO:
                If there is an issue with the looping and fading out of short sound files, it is because of
             this section, specifically: (this->fades.nFadeOut && opos > fadeoutstart && this->decodeData->loopsRemaining == 0)
             
             loopsRemaining might be 0 in the written buffer before it's reached the speakers. There is no easy solution to this problem
             other than to shorten the number of buffers or buffer size of the audio stream.
             **/
            
            // Fade In
            if (this->fades.nFadeIn && pos < fadeinend) {
                vol *= ((float)pos) / ((float)this->fades.nFadeIn);
            }
            else
                // Fade Out (only if no loops remain)
                if (this->fades.nFadeOut && opos > fadeoutstart && this->decodeData->loopsRemaining == 0) {
                    vol *= 1.0f - ((float)(opos - fadeoutstart)) / ((float)this->fades.nFadeOut);
                }
            
            // Active Instant Fade In
            // When would pos be before aFadeInStart? :P
            if (this->fades.aFadeInStart && pos < this->fades.aFadeInEnd)
            {
                unsigned long fadetotal = this->fades.aFadeInEnd - this->fades.aFadeInStart;
                vol *= ((float)(pos - this->fades.aFadeInStart)) / (float)fadetotal;
                
                if (vol > 1.0f) vol = 1.0f;
            }
            else if (this->fades.aFadeOutStart && pos > this->fades.aFadeOutStart)
            {
                unsigned long fadetotal = this->fades.aFadeOutEnd - this->fades.aFadeOutStart;
                vol *= 1.0f - (((float)(pos - this->fades.aFadeOutStart)) / (float)fadetotal);
                
                if (pos > this->fades.aFadeOutEnd){
                    this->fades.aFadeOutEnd = this->fades.aFadeOutStart = 0;
                    
                    if (this->fades.bPauseOnFadeOut){
                        Pause();
                    }
                    else
                    {
                        this->fades.bSilent = true;
                    }
                }
            }
            
            cs->SetVolume(vol*this->volume);
        }
        else
            cs->SetVolume(this->volume);
        
        if (this->fades.nFadeIn && cs->GetTotalPlayTime() > this->fades.nFadeIn)
            this->fades.nFadeIn = 0;
        
        return false;
    }
    
    AudioFades::AudioFades()
    {
        Reset();
    }
    
    void AudioFades::Reset()
    {
        this->nFadeIn = this->nFadeOut = 0;
        this->aFadeInEnd = this->aFadeInStart = this->aFadeOutEnd = this->aFadeOutStart = 0;
        this->bPauseOnFadeOut = false;
        this->bSilent = false;
    }
    
    void AudioDeck::ResetDeck()
    {
        if (cs->IsOpen())
            cs->Close();
        
        int ss = 1;
        if (decodeData->bit==16) ss = 2;
        this->sampsize = ss * decodeData->ch;
        
        this->currentPosition = 0;
        this->totsamps = 0;
        this->failed = false;
        
        this->playStarted = false;
        this->pause = false;
        
        this->decodeData->outOfData = false;
        
        this->isFinished = false;
        
        vc.Clean();
        vc.Reset();
        this->vizlast = 0;
        
        this->bufferCountdown = NUM_BUFFERS;
        cs->Open(decodeData->ch, decodeData->bit, decodeData->samplerate);
        FillBuffers();
        
        cs->SetVolume(volume);
        cs->SetPitch(pitchBend);
        cs->SetPan(pan);
        //memset(&this->fades,0,sizeof(AudioFades));
    }
    
    void AudioDeck::Init()
    {
        this->freeDeckOnFinish = false;
        this->nextDeck = 0;
        
        if (!this->decodeData){
            Debug::Log("AUDIO: No Decoder Data given to AudioDeck, crash imminent");
        }
        
        this->cleanVis = false;
        
        this->vis = new AudioVis();
        this->cs = new ChannelStream();
        
        this->pitchBend = 1.0;
        this->volume = 1.0;
        this->pan = 0.0;
        
        this->lastSeekPos = 0;
        this->fades.Reset();
        
        // Careful calculations calculated the buflen
        vc.Init((BUFFER_SIZE/32768)*NUM_BUFFERS, decodeData->samplerate);
        this->vizlast = 0;
        
        // We need to attach a VizEng internally now.
        this->vis->PrepData();

        ResetDeck();
//        this->bufferCountdown = NUM_BUFFERS;
//        cs->Open(decodeData->ch, decodeData->bit, decodeData->samplerate);
//        Update();
    }
    
    AudioDeck::AudioDeck( AudioDeck **deckSetter, AudioDecodeData *decodeData, bool freeDataWithDeck )
    {
        this->freeDecoderData = freeDataWithDeck;
        this->decodeData = decodeData;
        deckSetter[0] = this;
        this->prevDeckPointerToHere = deckSetter;
        
        Init();
    }
    
    AudioDeck::AudioDeck( AudioDeck *prevDeck, AudioDecodeData *decodeData, bool freeDataWithDeck )
    {
        this->freeDecoderData = freeDataWithDeck;
        this->decodeData = decodeData;
        prevDeck->nextDeck = this;
        this->prevDeckPointerToHere = &prevDeck->nextDeck;
        
        Init();
    }
    
    AudioDeck::~AudioDeck()
    {
        this->cs->Close();
        delete this->cs;
        
        if (this->decodeData && freeDecoderData) delete this->decodeData;
        
        delete this->vis;
        
        this->prevDeckPointerToHere[0] = this->nextDeck;
        
        if (this->nextDeck)
            this->nextDeck->prevDeckPointerToHere = this->prevDeckPointerToHere;
    }
    
    void AudioDeck::UpdateVizJunk()
    {
        long viznow = this->cs->GetTotalPlayTime();
        
        if (this->cleanVis){
            vc.Clean();
            this->cleanVis = false;
        }
        
        if (this->vis && this->vis->bDisabled) return;
        
        if (this->pause) {
            if (this->vis) this->vis->bClear = true;
            return;
        }
        
        //	if (vc->iWriteTimeMs < vc->iReadTimeMs)
        //		return;
        
        if (viznow - vizlast > 20)
        {
            if (this->vis)
            {
                if (vc.GetLatentData(this->cs->GetTotalPlayTime()))
                {
                    vc.GetWaveLeft(this->vis->cWaveformL);
                    vc.GetWaveRight(this->vis->cWaveformR);
                    this->vis->AnalyzeNewSound(576);
                    vc.GetEngineerData(&this->vis->engineerData[0],&this->vis->engineerData[1],&this->vis->engineerData[2],&this->vis->engineerData[3]);
                }
            }
            
            vizlast = this->cs->GetTotalPlayTime();
        }
    }
    
   /* void AudioDeck::PauseHandler( VisCache2 *vc )
    {
        if (od->pause && !od->done && !done[0] && !od->killSwitch){
            cP->Pause();
            while (od->pause && !od->done && !od->fades.aFadeOutStart && !done[0] && !od->killSwitch){
                if (od->vis) od->vis->bClear = true;
                Sleep(5);
            }
            
            if (od->pause){
                od->pause = 0;
                return;
            }
            
            UpdateFades(od,cP,done);
            
            cP->Resume();
            if (od->vis) od->vis->bClear = false;
            
        }
    }*/
    
    /***********************************************************************
     *           C168
     *
     * Converts a 16 bit sample to a 8 bit one (data loss !!)
     */
#define HIBYTE(w)   ((unsigned char) (((short) (w) >> 8) & 0xFF)) 

    static inline unsigned char C168(short s)
    {
        return HIBYTE(s) ^ (unsigned char)0x80;
    }
    
    void AudioDeck::Play()
    {
        this->pause = false;
        
        if (!cs->IsOpen()){
            // We probably reached the end and are wanting a replay. In this case we should reset the deck and decoder.
            if (decodeData->seekOffset == -1)
                decodeData->seekOffset = 0;
            long seekTo = decodeData->seekOffset;
            ResetDeck();
            this->lastSeekPos = seekTo;
        }
        
        if (playStarted && !cs->IsPlaying())
            cs->Resume();       // If we already started playing but aren't playing, we probably need to resume
        else if (!cs->IsPlaying())
            cs->Play();
        
        playStarted = true;
    }
    
    void AudioDeck::Pause()
    {
        this->pause = true;
        cs->Pause();
        
        fades.aFadeOutStart = 0;
		fades.aFadeOutEnd = 0;
		fades.bPauseOnFadeOut = false;
    }
    
    void AudioDeck::PauseWithFade( unsigned long msFade )
    {
        if (msFade == 0)
            Pause();
        else
        {
            fades.aFadeOutStart = cs->GetTotalPlayTime();
            fades.aFadeOutEnd = msFade + fades.aFadeOutStart;
            fades.bPauseOnFadeOut = true;
        }
    }
    
    void AudioDeck::MuteWithFade( unsigned long msFade )
    {
        if (msFade == 0)
            fades.bSilent = true;
        else
        {
            fades.aFadeOutStart = cs->GetTotalPlayTime();
            fades.aFadeOutEnd = msFade + fades.aFadeOutStart;
            fades.bPauseOnFadeOut = false;
        }
    }
    
    bool AudioDeck::IsMuted()
    {
        if (fades.bSilent) return true;
        if (fades.aFadeOutStart && !fades.bPauseOnFadeOut) return true;
        return false;
    }
    
    void AudioDeck::Mute()
    {
        fades.bSilent = true;
    }
    
    void AudioDeck::Unmute()
    {
        fades.bSilent = false;
    }
    
    void AudioDeck::UnmuteWithFade( unsigned long msFade )
    {
        ResumeWithFade( msFade );
        fades.bSilent = false;
    }
    
    void AudioDeck::ResumeWithFade( unsigned long msFade )
    {
        if (msFade > 0)
        {
            fades.aFadeInStart = cs->GetTotalPlayTime();
            fades.aFadeInEnd = msFade + fades.aFadeInStart;
        }
        
        Play();
    }
    
    void AudioDeck::Resume()
    {
        Play();
    }
    
    void AudioDeck::SetFadeIn( unsigned long msFade )
    {
        fades.nFadeIn = msFade;
    }
    
    void AudioDeck::SetFadeOut( unsigned long msFade )
    {
        fades.nFadeOut = msFade;
    }
    
    void AudioDeck::Seek( long pos )
    {
        if (pos >= 0)
            decodeData->seekOffset = pos;
    }
    
    void AudioDeck::Update()
    {
        // If we're asking for a pause, let's pause!
        if (this->pause){
            if (cs->IsPlaying())
                cs->Pause(); // Double check and make sure we're paused
            
            // Let the visualization know we're to be clear
            if (this->vis) this->vis->bClear = true;
            return;
        }
        else
        {
            // Not paused...
            if (this->playStarted && !cs->IsPlaying())
                cs->Resume(); // resume please
        }
        
        /**
        // Check to see if we need to seek...
        if (decodeData->seekOffset != -1 && 0)
        {
            cs->Close();
            delete cs;
            cs = new ChannelStream();
            cs->Open(decodeData->ch, decodeData->bit, decodeData->samplerate);
            
			cs->SetPan(this->pan);
			cs->SetVolume(this->volume);
            
            if (this->pitchBend != 1.0f)
			{
				cs->SetPitch(this->pitchBend);
				if (this->vis) this->vis->bDisabled = 1;
			}
			else if (this->vis) this->vis->bDisabled = 0;
            
			if (this->lastSeekPos >= 0){
                vc.Clean();
                vc.Reset();
                
				this->writtenpos = this->lastSeekPos;
				writtenpos = (this->lastSeekPos/1000)*decodeData->samplerate*sampsize;
				//od->writtenpos = (totsamps*1000)/(oc->samples*sampsize);
				cs->SetPlayOffset(this->lastSeekPos);
				this->currentPosition = this->lastSeekPos;
			}
			else
			{
				this->lastSeekPos = 0;
				this->writtenpos = 0;
				//if (od->vis) od->vis->bClear = true;
			}
		}
         **/
        
        if (decodeData->seekOffset != -1){
            long seekTo = decodeData->seekOffset;
            bool repause = this->pause;
            ResetDeck();
            lastSeekPos = seekTo;
            if (!repause) cs->Play();
            this->pause = repause;
        }
        
		if (UpdateFades()){
            // We stop here?
        }
        
		FillBuffers();
    }
    
    void AudioDeck::FillBuffers()
    {
        int buffers_to_fill = cs->NeedsUpdate();
        unsigned int size;
        
        // If we don't need an update, still update the visualization stuff
        if (!buffers_to_fill){
            // No update needed?
            if (this->vis && !this->pause) this->vis->bClear = false;
            
			UpdateVizJunk();
            
            if (this->isFinished && this->freeDeckOnFinish){
                delete this;
                return;
            }
        }
        else
        {
            if (decodeData->outOfData&&bufferCountdown>0){
                bufferCountdown--;
            }
            
            if (!bufferCountdown && !this->isFinished){
                this->isFinished = true;
                this->pause = true;
                
                if (cs->IsOpen())
                    cs->Close();
                
                if (this->freeDeckOnFinish)
                    delete this;
                
                return;
            }
        }
		
		while (buffers_to_fill--)
		{
			unsigned char *data;
            unsigned long l = 1;
            unsigned long pos = 0;
            
            data = this->cs->GetBuffer(&size);
            
            // Prepad the buffer with silence!
            if (decodeData->bit == 8)
                memset(data,0x80,size); // unsigned!
            else
                memset(data,0,size); // signed (:
            
            // If the decoder says we're out of data...
            if (decodeData->outOfData)
            {   
                // Write silence to vc
                memset(temp_waveL,0x80,576);
                memset(temp_waveR,0x80,576);
                
                while (pos<size){
                    
                    vc.SetWrittenTime(this->writtenpos);
                    
                    vc.PutWaveLeft(temp_waveL);
                    vc.PutWaveRight(temp_waveR);
                    vc.SetEngineerData(0,0,0,0);
                    
                    vc.EndEntry();
                    
                    UpdateVizJunk();
                    
                    l = MIN(576,size-pos);
                    
                    unsigned long nlen;
                    nlen = (unsigned long)(float)((float)l*1000.0f)/((float)(decodeData->samplerate*sampsize));
                    totsamps += l;
                    this->writtenpos = (totsamps*1000)/(decodeData->samplerate*sampsize);
                    this->currentPosition += nlen;
                    
                    pos += l;
                }
            }
            else
            {
                if (this->vis) this->vis->bClear = false;
            }
            
            while (l && pos<size && !decodeData->outOfData){
                
                // What to do if the Decoder says we're almost out of data?
                
                //			vc.NextFrame();
                //			vc.SetReadTime( cP->GetOutputTime() );
                //			vc.SetWriteTime( od->writtenpos );
                
                vc.SetWrittenTime( this->writtenpos );
                
                // If our engineer wants to set cached data, here is where we do it!
                /*if (oc->setcacheinfo)
                 {
                 //onu_deck_cable_s *cable, long *data1, long *data2, long *data3, long *data4 
                 long data[4];
                 oc->setcacheinfo(oc,&data[0],&data[1],&data[2],&data[3]);
                 vc.SetEngineerData(data[0],data[1],data[2],data[3]);
                 }
                 else
                 vc.SetEngineerData(0,0,0,0);*/
                
                vc.SetEngineerData(0,0,0,0);
                
                l = this->decodeData->decoder->Render((size-pos<576*sampsize)?(size-pos):576*sampsize,(void*)((long)data+(long)pos),*this->decodeData);
                
                if (l >= 576*sampsize)
                {				
                    int i = 0;
                    int samp = 0;
                    
                    if (decodeData->bit == 16)
                    {
                        short *copybuf = (short*)((long)data+(long)pos);
                        
                        for (i=0; i<576; i++, samp+=decodeData->ch)
                        {
                            temp_waveL[i] = C168(copybuf[samp]);
                            
                            if (decodeData->ch == 1)
                                temp_waveR[i] = C168(copybuf[samp]);
                            else
                                temp_waveR[i] = C168(copybuf[samp+1]); // Stereo (:
                            
                            //temp_waveL[i] = copybuf[samp] >> 8;
                            //temp_waveR[i] = copybuf[samp+1] >> 8;
                            //temp_waveL[i] = 0xFF;
                            //temp_waveR[i] = 0xFF;
                        }
                    }
                    else if (decodeData->bit == 8)
                    {
                        unsigned char *copybuf = (unsigned char*)((long)data+(long)pos);
                        
                        for (i=0; i<576; i++, samp+=decodeData->ch)
                        {
                            temp_waveL[i] = copybuf[samp];
                            
                            if (decodeData->ch == 1)
                                temp_waveR[i] = copybuf[samp];
                            else
                                temp_waveR[i] = copybuf[samp+1]; // Stereo (:
                            
                            //temp_waveL[i] = copybuf[samp] >> 8;
                            //temp_waveR[i] = copybuf[samp+1] >> 8;
                            //temp_waveL[i] = 0xFF;
                            //temp_waveR[i] = 0xFF;
                        }
                    }
                    
                    vc.PutWaveLeft(temp_waveL);
                    vc.PutWaveRight(temp_waveR);
                }
                else
                {
                    vc.PutWaveLeft(temp_waveL);
                    vc.PutWaveRight(temp_waveR);
                }
                
                vc.EndEntry();
                
                UpdateVizJunk();
                
                unsigned long nlen;
                nlen = (unsigned long)(float)((float)l*1000.0f)/((float)(decodeData->samplerate*sampsize));
                totsamps += l;
                this->writtenpos = (totsamps*1000)/(decodeData->samplerate*sampsize);
                this->currentPosition += nlen;
                
                pos += l;
            }
			
			this->cs->LockBuffer(size);
			
            /*			if (playbackPos >= targetLength)
             break;*/
		}
    }
    
    unsigned long AudioDeck::GetTotalLength()
    {
        return decodeData->total;
    }
    
    bool AudioDeck::IsDone()
    {
        return this->isFinished;
    }
    
    void AudioDeck::SetLoops(int loops)
    {
        decodeData->loopsRemaining = loops-1;
    }
    
    int AudioDeck::LoopsRemaining()
    {
        return decodeData->loopsRemaining;
    }
    
    unsigned long AudioDeck::GetCurrentTime()
    {
        unsigned long opos = cs->GetTotalPlayTime() + lastSeekPos;
        
        if (!cs->IsPlaying())
            return 0;
        
        while (opos > decodeData->total)
            opos -= decodeData->total;
        
        return opos;
    }
    
    void AudioDeck::FreeDeckOnFinish( bool freeDeckOnFinish )
    {
        this->freeDeckOnFinish = freeDeckOnFinish;
    }
    
    void AudioDeck::SetVolume( float volume )
    {
        if (volume < 0) volume = 0;
        if (volume > 1) volume = 1;
        this->volume = volume;
        UpdateFades(); // Update the volume here.
    }
    
    void AudioDeck::SetPan( float pan )
    {
        if (pan < -1) pan = -1;
        if (pan > 1) pan = 1;
        this->pan = pan;
        this->cs->SetPan(pan);
    }
    
    void AudioDeck::SetPitch( float pitch )
    {
        if (pitch < 0.5) pitch = 0.5;
        if (pitch > 2.0) pitch = 2.0;
        this->pitchBend = pitch;
        this->cs->SetPitch( this->pitchBend );
    }
    
    float AudioDeck::GetVolume()
    {
        return this->volume;
    }
    
    float AudioDeck::GetPan()
    {
        return this->pan;
    }
    
    float AudioDeck::GetPitch()
    {
        return this->pitchBend;
    }
}