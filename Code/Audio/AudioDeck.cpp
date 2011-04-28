//
//  AudioDeck.cpp
//
//  Created by Josh Whelchel on 4/26/11.
//

#include <string.h>

#include "AudioDeck.h"
#include "../Platform.h"
#include "../Debug.h"

namespace Monocle {
    
    bool AudioDeck::UpdateFades()
    {
        if (pause || done)  // also check threads?
            return false;
        
        if (fades.nFadeIn || fades.nFadeOut || fades.aFadeInStart || fades.aFadeOutStart)
        {
            float vol = 1.0f;
            unsigned long fadeinend = this->fades.nFadeIn;
            unsigned long total = this->total;
            unsigned long fadeoutstart;
            unsigned long pos = cs->GetTotalPlayTime();
            
            if (this->numLoops > 1)
                total = this->total * this->numLoops;
            else
                total = this->total;
            
            fadeoutstart =  total - this->fades.nFadeOut;
            
            // Fix if we go past the fadein point so it doesn't happen again o_O
            if (pos >= this->fades.aFadeInEnd)
                this->fades.aFadeInEnd = this->fades.aFadeInStart = 0;
            
            // Fade In
            if (this->fades.nFadeIn && pos < fadeinend) {
                vol *= ((float)pos) / ((float)this->fades.nFadeIn);
            }
            else
                // Fade Out (only if no loops remain)
                if (this->fades.nFadeOut && pos > fadeoutstart && this->decodeData->loopsRemaining == 0) {
                    vol *= 1.0f - ((float)(pos - fadeoutstart)) / ((float)this->fades.nFadeOut);
                }
            
            // Active Instant Fade In
            // When would pos be before aFadeInStart? :P
            if (this->fades.aFadeInStart && pos < this->fades.aFadeInEnd)
            {
                unsigned long fadetotal = this->fades.aFadeInEnd - this->fades.aFadeInStart;
                vol *= ((float)(pos - this->fades.aFadeInStart)) / (float)fadetotal;
                
                if (vol > 1.0f) vol = 1.0f;
            }
            else
                // Active Instant Fade Out
                if (this->fades.aFadeOutStart && pos > this->fades.aFadeOutStart)
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
                            Pause(); // Stopped...
                            return true;
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
    
    void AudioDeck::ResetDeck()
    {
        if (!this->decodeData){
            Debug::Log("AUDIO: No Decoder Data given to Deck, crash imminent");
        }
        
        this->done = false;
        this->stopping = 0;
        
        this->pitchBend = 1.0;
        this->volume = 1.0;
        this->pan = 0.0;
        
        this->cleanVis = false;
        this->curpos = 0;
        this->currentPosition = 0;
        this->didSeek = 0;
        this->numLoops = 0;
        this->pause = false;
        this->triggerDisconnectOnFinish = false;
        
        this->lastSeekPos = 0;
        this->total = decodeData->total;
        
        this->bufLen = 1000;

        vc.Init(this->bufLen + 5000, decodeData->samplerate);
        this->vizlast = Platform::GetMilliseconds();
        
        // We need to attach a VizEng internally now.
        this->vis->PrepData();
        
        //memset(&this->fades,0,sizeof(AudioFades));
        this->fades.Reset();
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
    }
    
    void AudioDeck::Init()
    {
        this->nextDeck = 0;
        
        this->vis = new AudioVis();
        this->cs = new ChannelStream();
        ResetDeck();
        
        this->totsamps = 0;
        this->starting = true;
        this->failed = false;
        
        this->done = false;
        this->playStarted = false;
        this->pause = false;
        
        cs->Open(decodeData->ch, decodeData->bit, decodeData->samplerate);
        Update();
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
        long viznow = Platform::GetMilliseconds();
        
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
            
            vizlast = Platform::GetMilliseconds();
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
        fades.aFadeOutStart = cs->GetTotalPlayTime();
		fades.aFadeOutEnd = msFade + fades.aFadeOutStart;
		fades.bPauseOnFadeOut = true;
    }
    
    void AudioDeck::ResumeWithFade( unsigned long msFade )
    {
        fades.aFadeInStart = cs->GetTotalPlayTime();
		fades.aFadeInEnd = msFade + fades.aFadeInStart;
        
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
    
    void AudioDeck::Update()
    {
        int ss = 1;
        if (decodeData->bit==16) ss = 2;
        long sampsize = ss * decodeData->ch;
        
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
        
            if (this->vis) this->vis->bClear = false;
        }
        
        // Check to see if we need to seek...
        if (this->didSeek)
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
        
		if (UpdateFades()){
            // We stop here?
        }
        
//		if (od->done || done || oc->done || oc->almostDone || od->killSwitch)
//			break;
        
		int buffers_to_fill = this->cs->NeedsUpdate();
        unsigned int size;
        
        // If we don't need an update, still update the visualization stuff
        if (!buffers_to_fill){
            // No update needed?
            if (this->vis && !this->pause) this->vis->bClear = false;
            
			//vc.SetReadTime( cP->GetOutputTime() );
			UpdateVizJunk();
            
			//PauseHandler(od,&done,cP,&vc);
        }
		
		while (buffers_to_fill--)
		{
			unsigned char *data;
            unsigned long l = 1;
            unsigned long pos = 0;
            
            data = this->cs->GetBuffer(&size);
            
            // If the decoder says we're out of data...
            if (decodeData->outOfData)
            {
                //for (int i=0;i<32 && !od->done && !done && !oc->done;i++)
                {
                    long time = 0;
                    
                    //                totsamps += size;
                    //od->writtenpos += (DWORD)((float)ns*1000.0f)/((float)(oc->samples*sampsize));
                    //                this->writtenpos = (totsamps*1000)/(decodeData->samplerate*sampsize);
                    
                    /*
                     if (od->triggerDisconnectOnFinish)
                     {
                     od->cable->freecable(od->cable);
                     od->cable = 0;
                     od->triggerDisconnectOnFinish = 0;
                     od->settings.pan = 0;
                     od->settings.volume = 1.0f;
                     od->settings.rate = -1;
                     od->rateMultiplier = 1.0f;
                     }
                     */
                    
                    memset(data,0,size);
                    
                    
                    // Write silence to vc
                    memset(temp_waveL,0,576);
                    memset(temp_waveR,0,576);
                    
                    vc.PutWaveLeft(temp_waveL);
                    vc.PutWaveRight(temp_waveR);
                    vc.SetEngineerData(0,0,0,0);
                    vc.SetWrittenTime(this->writtenpos);
                    
                    vc.EndEntry();
                    
                    if (this->vis) this->vis->bClear = true;
                }
                
                l = 0;
            }
            
            while (l && pos<size && !decodeData->outOfData && !this->done){
                if (this->vis) this->vis->bClear = false;
                
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
                
//                l = oc->render((ns-pos<576*sampsize)?(ns-pos):576*sampsize,(cdsbuf)((long)buf+(long)pos),oc);
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
                
                //			UpdateFades(od,cP,&done); // Do NOT break with an open buffer
                
                unsigned long nlen;
                nlen = (unsigned long)(float)((float)l*1000.0f)/((float)(decodeData->samplerate*sampsize));
                //nlen = (l*1000)/(oc->samples*oc->ch*sampsize);
                totsamps += l;
                //od->writtenpos += nlen;
                this->writtenpos = (totsamps*1000)/(decodeData->samplerate*sampsize);
                this->currentPosition += nlen;
                
                pos += l;
            }
			
//            size = this->decodeData->decoder->Render(size,(void*)data,*this->decodeData);
			
			this->cs->LockBuffer(size);
			
            /*			if (playbackPos >= targetLength)
             break;*/
		}
    }
    
    unsigned long AudioDeck::GetTotalLength()
    {
        return this->total;
    }
}