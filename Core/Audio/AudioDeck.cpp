//
//  AudioDeck.cpp
//
//  Created by Josh Whelchel on 4/26/11.
//

#include <string.h>

#include "Audio.h"
#include "../Platform.h"
#include "../Debug.h"
#include "../MonocleToolkit.h"

//#define MIN(a,b) (a<b)?a:b
//#define MAX(a,b) (a>b)?a:b

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
            unsigned long total = decoder->total;
            unsigned long fadeoutstart;
            unsigned long pos = cs->GetTotalPlayTime();
            unsigned long opos = GetCurrentTime();
            
            // Fix if we go past the fadein point so it doesn't happen again o_O
            if (pos >= this->fades.aFadeInEnd)
                this->fades.aFadeInEnd = this->fades.aFadeInStart = 0;
            
            fadeoutstart =  total - this->fades.nFadeOut;
            
            /** TODO:
                If there is an issue with the looping and fading out of short sound files, it is because of
             this section, specifically: (this->fades.nFadeOut && opos > fadeoutstart && this->decoder->loopsRemaining == 0)
             
             loopsRemaining might be 0 in the written buffer before it's reached the speakers. There is no easy solution to this problem
             other than to shorten the number of buffers or buffer size of the audio stream.
             **/
            
            // Fade In
            if (this->fades.nFadeIn && pos < fadeinend) {
                vol *= ((float)pos) / ((float)this->fades.nFadeIn);
            }
            else
            // Fade Out (only if no loops remain)
            if (this->fades.nFadeOut && opos > fadeoutstart && this->decoder->loopsRemaining == 0) {
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
                    
                    switch (this->fades.fadeAction)
                    {
                        case FADEOUT_AND_PAUSE:
                            Pause();
                            break;
                            
                        case FADEOUT_AND_STOP:
                            Stop();
                            return true; // We return here in case we free the deck on finish!
                            
                        default:
                        case FADEOUT_AND_MUTE:
                            this->fades.bSilent = true;
                            break;
                            
                            
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
        this->fadeAction = FADEOUT_AND_PAUSE;
        this->bSilent = false;
    }
    
    void AudioDeck::ResetDeck()
    {
        if (cs->IsOpen())
            cs->Close();
        
        int ss = 1;
        if (decoder->bit==16) ss = 2;
        this->sampsize = ss * decoder->ch;
        
        this->currentPosition = 0;
        this->totsamps = 0;
        this->writtenpos = 0;
        this->failed = false;
        
        this->playStarted = false;
        this->pause = false;
        
        this->decoder->outOfData = false;
        
        this->isFinished = false;
        
        if (IsVisEnabled()){
            vc.Clean();
            vc.Reset();
        }
        this->vizlast = 0;
        
        // We don't init. lastSeekPos because we could have set that before a Flush(). (important)
        
        this->bufferCountdown = cs->GetNumberBuffers();
        cs->Open(decoder->ch, decoder->bit, decoder->samplerate);
        FillBuffers();
        
        cs->SetVolume(volume);
        cs->SetPitch(pitchBend);
        cs->SetPan(pan);
        //memset(&this->fades,0,sizeof(AudioFades));
    }
    
    void AudioDeck::Flush()
    {
        bool wasPlaying = cs->IsPlaying();
        bool wasPaused = this->pause;
        long seek = cs->GetOutputTime() + this->lastSeekPos;
        
        while (seek > decoder->total && decoder->total > 0)
            seek -= decoder->total;
        
        this->lastSeekPos = seek;
        this->decoder->seekOffset = seek;
        ResetDeck();
        
        if (wasPlaying && !wasPaused)
            cs->Play();
        if (wasPaused)
            this->pause = true;
    }
    
    bool AudioDeck::IsVisEnabled()
    {
        // jw: Not sure if this is the best way, but it works.
        if (this->vis) return true;
        return false;
    }
    
    void AudioDeck::EnableVis( bool visEnable )
    {
        if (visEnable && this->vis)
            return;
        
        if (visEnable)
        {
            // Careful calculations calculated the buflen
//            vc.Init((BUFFER_SIZE/32768)*(NUM_BUFFERS+2), decoder->samplerate);
            vc.Init((cs->GetBufferSize()/32768)*(cs->GetNumberBuffers()+2),decoder->samplerate);
            
            vc.Clean();
            
            this->vis = new AudioVis();
            this->vis->PrepData();
            
//            if (cs->IsPlaying())
            Flush();
            
            if (cs->IsOpen())
                this->vizlast = cs->GetTotalPlayTime();
            else
                this->vizlast = 0;
        }
        
        if (!visEnable && this->vis){
            vc.Destroy();
            delete this->vis;
        }
    }
    
    void AudioDeck::Init()
    {
        this->freeDeckOnFinish = false;
        this->nextDeck = 0;
        
        if (!this->decoder){
            Debug::Log("AUDIO: No Decoder Data given to AudioDeck, crash imminent");
        }
        
        this->cleanVis = false;
        
        this->vis = NULL;
        
        this->pitchBend = 1.0;
        this->volume = 1.0;
        this->pan = 0.0;
        
        this->lastSeekPos = 0;
        this->fades.Reset();
        
        // This, because we set vis = NULL, should never happen.
        // Eventually maybe we'll enable vis before a deck opens.
        if (IsVisEnabled()){
            // Careful calculations calculated the buflen
            vc.Init((cs->GetBufferSize()/32768)*(cs->GetNumberBuffers()+2), decoder->samplerate);
            vc.Clean();
            
            this->vis = new AudioVis();
        }
        this->vizlast = 0;
        
        /** these are replaced by resetdeck(), but just in case? **/
        this->sampsize = 2;
        this->currentPosition = 0;
        this->totsamps = 0;
        this->writtenpos = 0;
        this->failed = false;
        this->playStarted = false;
        this->pause = false;
        this->decoder->outOfData = false;
        this->isFinished = false;
        this->vizlast = 0;

        // Open the channel and reset the deck.
        this->cs = new ChannelStream();
        ResetDeck();
    }
    
    AudioDeck::AudioDeck( AudioDeck **deckSetter, AudioDecoder *decoder, bool freeDecoderWithDeck )
    {
        this->freeDecoder = freeDecoderWithDeck;
        this->decoder = decoder;
        deckSetter[0] = this;
        this->prevDeckPointerToHere = deckSetter;
        
        Init();
    }
    
    AudioDeck::AudioDeck( AudioDeck *prevDeck, AudioDecoder *decoder, bool freeDecoderWithDeck )
    {
        this->freeDecoder = freeDecoderWithDeck;
        this->decoder = decoder;
        prevDeck->nextDeck = this;
        this->prevDeckPointerToHere = &prevDeck->nextDeck;
        
        Init();
    }
    
    AudioDeck::~AudioDeck()
    {
        this->cs->Close();
        delete this->cs;
        
        if (this->decoder && freeDecoder) delete this->decoder;
        
        if (this->vis)
            delete this->vis;
        
        this->prevDeckPointerToHere[0] = this->nextDeck;
        
        if (this->nextDeck)
            this->nextDeck->prevDeckPointerToHere = this->prevDeckPointerToHere;
    }
    
    void AudioDeck::UpdateVizJunk()
    {
        long viznow = this->cs->GetTotalPlayTime();
        
        if (!IsVisEnabled()) return;
        
        if (this->cleanVis || viznow - vizlast > 2000){
            vc.Clean();
            this->cleanVis = false;
            vizlast=viznow;
        }
        
        if (this->vis && this->vis->bDisabled) return;
        
        if (this->pause) {
            if (this->vis) this->vis->bClear = true;
            return;
        }
        
        // Fill in 20ms increments
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
                else
                {
                    // We've run out of brainpower somewhere.
//                    this->cleanVis = true;
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
            if (decoder->seekOffset == -1)
                decoder->seekOffset = 0;
            long seekTo = decoder->seekOffset;
            ResetDeck();
            this->lastSeekPos = seekTo;
        }
        
        if (playStarted && !cs->IsPlaying())
            cs->Resume();       // If we already started playing but aren't playing, we probably need to resume
        else if (!cs->IsPlaying())
            cs->Play();
        
        playStarted = true;
        
        if (Audio::IsPaused())
            cs->Pause();
    }
    
    bool AudioDeck::IsPaused( bool absolute )
    {
        if (absolute)
            return this->pause || Audio::IsPaused();
        else
            return this->pause;
    }
    
    void AudioDeck::Pause()
    {
        this->pause = true;
        cs->Pause();
        
        fades.aFadeOutStart = 0;
		fades.aFadeOutEnd = 0;
		fades.fadeAction = FADEOUT_AND_MUTE;
    }
    
    void AudioDeck::PauseWithFade( unsigned long msFade )
    {
        if (msFade == 0)
            Pause();
        else
        {
            fades.aFadeOutStart = cs->GetTotalPlayTime();
            fades.aFadeOutEnd = msFade + fades.aFadeOutStart;
            fades.fadeAction = FADEOUT_AND_PAUSE;
        }
    }
    
    void AudioDeck::Stop()
    {
        this->pause = true;
        this->isFinished = true;
        this->playStarted = false;
        cs->Close();
        
        if (freeDeckOnFinish){
            delete this;
            return;
        }
        
        fades.aFadeOutStart = 0;
		fades.aFadeOutEnd = 0;
		fades.fadeAction = FADEOUT_AND_MUTE;
    }
    
    void AudioDeck::StopWithFade( unsigned long msFade )
    {
        if (msFade == 0)
            Stop();
        else
        {
            fades.aFadeOutStart = cs->GetTotalPlayTime();
            fades.aFadeOutEnd = msFade + fades.aFadeOutStart;
            fades.fadeAction = FADEOUT_AND_STOP;
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
            fades.fadeAction = FADEOUT_AND_MUTE;
        }
    }
    
    bool AudioDeck::IsMuted()
    {
        if (fades.bSilent) return true;
        if (fades.aFadeOutStart && fades.fadeAction == FADEOUT_AND_MUTE) return true;
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
            decoder->seekOffset = pos;
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
            // Not paused... so we probably LOST SYNC. oh god. ):
            if (this->playStarted && !cs->IsPlaying()){
//                cs->Resume(); // resume please
                
                // Make sure we're not just ending naturally!
                if (!decoder->outOfData)
                {
                    // We're supposed to be playing, and we're not.
                    // flush and play :O
                    
                    Debug::Log("AUDIO: ChannelStream stopped playing. The audio thread could not keep up with the buffer - try increasing buffer size or reducing latency.");
                    
                    Flush();
                    Play();
                    
                    return;
                }
            }
        }
        
        /**
        // Check to see if we need to seek...
        if (decoder->seekOffset != -1 && 0)
        {
            cs->Close();
            delete cs;
            cs = new ChannelStream();
            cs->Open(decoder->ch, decoder->bit, decoder->samplerate);
            
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
				writtenpos = (this->lastSeekPos/1000)*decoder->samplerate*sampsize;
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
        
        if (decoder->seekOffset != -1){
            long seekTo = decoder->seekOffset;
            bool repause = this->pause;
            ResetDeck();
            lastSeekPos = seekTo;
            if (!repause) cs->Play();
            this->pause = repause;
        }
        
		if (UpdateFades()){
            // UpdateFades() returns true if we stopped.
            // We return in case we deleted this deck on finish.
            return;
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
            if (decoder->outOfData&&bufferCountdown>0){
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
            if (decoder->bit == 8)
                memset(data,0x80,size); // unsigned!
            else
                memset(data,0,size); // signed (:
            
            // jw: this can be simplified because we're only helping out the visualizers.
            // If the decoder says we're out of data...
            if (decoder->outOfData)
            {   
                // Write silence to vc
                memset(temp_waveL,0x80,576);
                memset(temp_waveR,0x80,576);
                
                while (pos<size){
                    
                    if (IsVisEnabled())
                    {
                        vc.SetWrittenTime(this->writtenpos);
                        
                        vc.PutWaveLeft(temp_waveL);
                        vc.PutWaveRight(temp_waveR);
                        vc.SetEngineerData(0,0,0,0);
                        
                        vc.EndEntry();
                    }
                    
                    UpdateVizJunk();
                    
                    l = MIN(576,size-pos);
                    
                    unsigned long nlen;
                    nlen = (unsigned long)(float)((float)l*1000.0f)/((float)(decoder->samplerate*sampsize));
                    totsamps += l;
                    this->writtenpos = (totsamps*1000)/(decoder->samplerate*sampsize);
                    this->currentPosition += nlen;
                    
                    pos += l;
                }
            }
            else
            {
                if (IsVisEnabled()) this->vis->bClear = false;
            }
            

            while (l && pos<size && !decoder->outOfData){
                
                // What to do if the Decoder says we're almost out of data?
                
                if (IsVisEnabled())
                {
                    vc.SetWrittenTime( this->writtenpos );
                    vc.SetEngineerData(0,0,0,0);
                }
                
                l = this->decoder->Render((size-pos<576*sampsize)?(size-pos):576*sampsize,(void*)((long)data+(long)pos));
                
                if (IsVisEnabled())
                {
                    if (l >= 576*sampsize)
                    {				
                        int i = 0;
                        int samp = 0;
                        
                        if (decoder->bit == 16)
                        {
                            short *copybuf = (short*)((long)data+(long)pos);
                            
                            for (i=0; i<576; i++, samp+=decoder->ch)
                            {
                                temp_waveL[i] = C168(copybuf[samp]);
                                
                                if (decoder->ch == 1)
                                    temp_waveR[i] = C168(copybuf[samp]);
                                else
                                    temp_waveR[i] = C168(copybuf[samp+1]); // Stereo (:
                                
                                //temp_waveL[i] = copybuf[samp] >> 8;
                                //temp_waveR[i] = copybuf[samp+1] >> 8;
                                //temp_waveL[i] = 0xFF;
                                //temp_waveR[i] = 0xFF;
                            }
                        }
                        else if (decoder->bit == 8)
                        {
                            unsigned char *copybuf = (unsigned char*)((long)data+(long)pos);
                            
                            for (i=0; i<576; i++, samp+=decoder->ch)
                            {
                                temp_waveL[i] = copybuf[samp];
                                
                                if (decoder->ch == 1)
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
                
                }
                
                UpdateVizJunk();
                
                unsigned long nlen;
                nlen = (unsigned long)(float)((float)l*1000.0f)/((float)(decoder->samplerate*sampsize));
                totsamps += l;
                this->writtenpos = (totsamps*1000)/(decoder->samplerate*sampsize);
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
        return decoder->total;
    }
    
    bool AudioDeck::IsDone()
    {
        return this->isFinished;
    }
    
    void AudioDeck::SetLoops(int loops)
    {
        decoder->loopsRemaining = loops-1;
    }
    
    int AudioDeck::LoopsRemaining()
    {
        return decoder->loopsRemaining;
    }
    
    unsigned long AudioDeck::GetCurrentTime()
    {
        unsigned long opos = cs->GetTotalPlayTime() + lastSeekPos;
        
        if (!cs->IsPlaying())
            return 0;
        
        while (opos > decoder->total && decoder->total > 0)
            opos -= decoder->total;
        
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
    
    float AudioDeck::GetVisWaveform( int index, int channel )
    {
        if (!IsVisEnabled()) return 0.0;
        if (channel < 0 || channel > 1) return 0.0;
        if (index < 0 || index >= 576) return 0.0;
        return this->vis->fWaveform[channel][index] / 128.0;
    }
    
    float AudioDeck::GetVisSpectrum( int index, int channel )
    {
        if (!IsVisEnabled()) return 0.0;
        if (channel < -1 || channel > 1) return 0.0;
        if (index < 0 || index >= 512) return 0.0;
        
        if (channel == -1)
            return MAX(this->vis->fSpectrum[0][index]/10.0,this->vis->fSpectrum[1][index]/10.0);
        else
            return this->vis->fSpectrum[channel][index]/10.0;
    }
    
    int AudioDeck::GetVisLoudestSpectrumIndex( float *loudestValue, int channel, int startIndex, int endIndex)
    {
        int loudestIndex=0;
        float loudestIndexValue=-1;
        int ind;
        
        if (!IsVisEnabled()) return 0;
        if (startIndex < 0 || startIndex >= 512) return 0;
        if (endIndex < 0 || endIndex >= 512) return 0;
        if (endIndex < startIndex) return 0;
        
        for (ind=startIndex;ind<=endIndex;ind++)
        {
            float val = GetVisSpectrum(ind,channel);
            if (val > loudestIndexValue) { 
                loudestIndex = ind; 
                loudestIndexValue = val; 
            }
        }
        
        if (loudestValue) loudestValue[0] = loudestIndexValue;
        
        return loudestIndex;
    }
    
    float AudioDeck::GetVisBandAverage( int band, VisBandLength length, int channel)
    {
        if (!IsVisEnabled()) return 0.0;
        if (band < 0 || band >= 16) return 0.0;
        if (channel < -1 || channel > 1) return 0.0;
        
        if (channel >= 0){
            switch (length)
            {
                case VIS_BAND_LONG:
                    return this->vis->long_avg[channel][band];
                case VIS_BAND_SHORT:
                    return this->vis->imm[channel][band];
                case VIS_BAND_MEDIUM:
                    return this->vis->med_avg[channel][band];
            }
        }else{
            switch (length)
            {
                case VIS_BAND_LONG:
                    return MAX(this->vis->long_avg[0][band],this->vis->long_avg[1][band]);
                case VIS_BAND_SHORT:
                    return MAX(this->vis->imm[0][band], this->vis->imm[1][band]);
                case VIS_BAND_MEDIUM:
                    return MAX(this->vis->med_avg[0][band],this->vis->med_avg[1][band]);
            }
        }
        
        return 0.0;
    }
    
    ChannelStream *AudioDeck::GetChannelStream()
    {
        return this->cs;
    }
}
