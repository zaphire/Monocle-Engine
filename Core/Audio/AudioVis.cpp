//
//  AudioVis.cpp
//
//  Created by Josh Whelchel on 4/26/11.
//

#include "Audio.h"
#include "AudioVis.h"

#include "fft.h"
#include <math.h>
#include <string.h> // required for memset when compiling via GCC
#include "../MonocleToolkit.h"

namespace Monocle {
    
    class AudioVisData {
    public:
        AudioDeck *deck;
        
        FFT *fft;
        
        int frame;
        
        int m_has_fallen;
        float m_thresh;
        float m_limit;
        float m_last_hit;
    };

    // Here is the OGG engineer!

    float AdjustRateToFPS(float per_frame_decay_rate_at_fps1, float fps1, float actual_fps)
    {
        // returns the equivalent per-frame decay rate at actual_fps
        
        // basically, do all your testing at fps1 and get a good decay rate;
        // then, in the real application, adjust that rate by the actual fps each time you use it.
        
        float per_second_decay_rate_at_fps1 = powf(per_frame_decay_rate_at_fps1, fps1);
        float per_frame_decay_rate_at_fps2 = powf(per_second_decay_rate_at_fps1, 1.0f/actual_fps);
        
        return per_frame_decay_rate_at_fps2;
    }

    #define NUM_FREQUENCIES 512

    #define m_fps 76

    void AudioVis::BeatDetect()
    {
        AudioVisData *d = this->data;
        // do some (very) simple beat detection, based on volume thresholds.
        // sets 'm_is_beat' to 0 or 1, based on whether or not the volume has spiked.
        {
            //        float vol = 0.3333f*(this->avg[0][0]/this->med_avg[0][0] + this->avg[0][1]/this->med_avg[0][1] + this->avg[0][2]/this->med_avg[0][2]);
            //        float decay = 0.997f;
            float decay = 0.997f;
            float vol = 0.0f;
            float voltotal = 0.0f;
            
            /*
             for (int i=0;i<576;i++)
             {
             voltotal += fabs(this->fWaveform[0][i]); 
             voltotal += fabs(this->fWaveform[1][i]);
             }
             
             voltotal += (this->avg[0][0]/this->med_avg[0][0])*10.0f;
             voltotal += (this->avg[1][0]/this->med_avg[1][0])*10.0f;
             
             voltotal /= (596.0f/2.5f);
             vol = voltotal;*/
            
            for (int i=0;i<16;i++)
            {
                if (this->med_avg[0][i] > 0.0f)
                    voltotal += (this->avg[0][i]/this->med_avg[0][i]);
                if (this->med_avg[1][i] > 0.0f)
                    voltotal += (this->avg[1][i]/this->med_avg[1][i]);
            }
            
            voltotal /= (16.0f);
            vol = voltotal;
            
            // make the threshold decay over time, toward the limit.
            // this is fps-independent, thanks to AdjustRateToFPS().
            d->m_thresh = d->m_thresh*decay + d->m_limit*(1-decay);
            
            if (!d->m_has_fallen)
            {
                // wait for it to get a bit quieter before allowing another beat
                if (vol < d->m_thresh)
                { 
                    d->m_has_fallen   = 1;
                    d->m_thresh       = d->m_last_hit*1.03f;
                    d->m_limit        = 1.1f;
                }
            }
            else
            {
                // signal fell; now wait for vol to reach m_thresh, then signal a beat.
                if (vol > d->m_thresh && vol > 0.2f)
                {
                    this->bIsBeat++;
                    
                    // TODO: BEAT DETECT
                    //				Beep(440,10);
                    //				if (d->rdPtr && d->rdPtr->vis)
                    //					GenerateEvent(d->rdPtr,CND_ONBEAT);
                    
                    d->m_last_hit     = vol;
                    
                    d->m_has_fallen   = 0;
                    d->m_thresh       = MAX(1.0f, vol*0.85f);  
                    d->m_limit        = MAX(1.0f, vol*0.70f);
                }
            }
        }
    }

    /***********************************************************************
     *           C816
     *
     * Converts a 8 bit sample to a 16 bit one
     */
    static inline short C816(unsigned char b)
    {
        return (b - 128) << 8;
    }

    void AudioVis::AnalyzeNewSound(long rsize)
    {
        // we get 576 samples in from winamp.
        // the output of the fft has 'num_frequencies' samples,
        //   and represents the frequency range 0 hz - 22,050 hz.
        // usually, plugins only use half of this output (the range 0 hz - 11,025 hz),
        //   since >10 khz doesn't usually contribute much.
        
        int i;
        float temp_wave[2][576];
        
        int samp=0;
        int old_i = 0;
        
        AudioVisData *data = this->data;
        AudioVis *rd = this;
        FFT *m_fftobj = data->fft;
        
        for (i=0; i<576; i++, samp+=2)
        {
            //		rd->fWaveform[0][i] = (float)(((float)C816(rd->cWaveformL[i]))/32768.0f);
            //        rd->fWaveform[1][i] = (float)(((float)C816(rd->cWaveformR[i]))/32768.0f);
            
            rd->fWaveform[0][i] = (float)((rd->cWaveformL[i]) -128);
            rd->fWaveform[1][i] = (float)((rd->cWaveformR[i]) -128);
            
            //		fWaveform[0][i] = (float)((pWave[samp]/(0x8000)));
            //        fWaveform[1][i] = (float)((pWave[samp+1]/(0x8000)));
            
            // simulating single frequencies from 200 to 11,025 Hz:
            //float freq = 1.0f + 11050*(GetFrame() % 100)*0.01f;
            //rd->fWaveform[0][i] = 10*sinf(i*freq*6.28f/44100.0f);
            
            // damp the input into the FFT a bit, to reduce high-frequency noise:
            temp_wave[0][i] = 0.5f*(rd->fWaveform[0][i] + rd->fWaveform[0][old_i]);
            temp_wave[1][i] = 0.5f*(rd->fWaveform[1][i] + rd->fWaveform[1][old_i]);
            old_i = i;
        }
        
        m_fftobj->time_to_frequency_domain(temp_wave[0], rd->fSpectrum[0]);
        m_fftobj->time_to_frequency_domain(temp_wave[1], rd->fSpectrum[1]);
        
        // sum (left channel) spectrum up into 3 bands
        // [note: the new ranges do it so that the 3 bands are equally spaced, pitch-wise]
        float min_freq = 60.0f;
        float max_freq = 11025.0f;
        float net_octaves = (logf(max_freq/min_freq) / logf(2.0f));     // 5.7846348455575205777914165223593
        float octaves_per_band = net_octaves / 16.0f;                    // 1.9282116151858401925971388407864
        float mult = powf(2.0f, octaves_per_band); // each band's highest freq. divided by its lowest freq.; 3.805831305510122517035102576162
        // [to verify: min_freq * mult * mult * mult should equal max_freq.]
        for (int ch=0; ch<2; ch++)
        {
            for (i=0; i<16; i++)
            {
                // old guesswork code for this:
                //   float exp = 2.1f;
                //   int start = (int)(NUM_FREQUENCIES*0.5f*powf(i/3.0f, exp));
                //   int end   = (int)(NUM_FREQUENCIES*0.5f*powf((i+1)/3.0f, exp));
                // results:
                //          old range:      new range (ideal):
                //   bass:  0-1097          200-761
                //   mids:  1097-4705       761-2897
                //   treb:  4705-11025      2897-11025
                int start = (int)(NUM_FREQUENCIES * min_freq*powf(mult, i  )/11025.0f);
                int end   = (int)(NUM_FREQUENCIES * min_freq*powf(mult, i+1)/11025.0f);
                if (start < 0) start = 0;
                if (end > NUM_FREQUENCIES) end = NUM_FREQUENCIES;
                
                rd->imm[ch][i] = 0;
                for (int j=start; j<end; j++)
                    rd->imm[ch][i] += rd->fSpectrum[ch][j];
                rd->imm[ch][i] /= (float)(end-start);
            }
        }
        
        // do temporal blending to create attenuated and super-attenuated versions
        for (int ch=0; ch<2; ch++)
        {
            for (i=0; i<16; i++)
            {
                // rd->avg[i]
                {
                    float avg_mix;
                    if (rd->imm[ch][i] > rd->avg[ch][i])
                        avg_mix = AdjustRateToFPS(0.2f, 76.0f, m_fps);
                    else
                        avg_mix = AdjustRateToFPS(0.5f, 76.0f, m_fps);
                    rd->avg[ch][i] = rd->avg[ch][i]*avg_mix + rd->imm[ch][i]*(1-avg_mix);
                }
                
                // rd->med_avg[i]
                // rd->long_avg[i]
                {
                    float med_mix  = 0.91f;//0.800f + 0.11f*powf(t, 0.4f);    // primarily used for velocity_damping
                    float long_mix = 0.96f;//0.800f + 0.16f*powf(t, 0.2f);    // primarily used for smoke plumes
                    med_mix  = AdjustRateToFPS( med_mix, 76.0f, m_fps);
                    long_mix = AdjustRateToFPS(long_mix, 76.0f, m_fps);
                    rd->med_avg[ch][i]  =  rd->med_avg[ch][i]*(med_mix ) + rd->imm[ch][i]*(1-med_mix );
                    rd->long_avg[ch][i] = rd->long_avg[ch][i]*(long_mix) + rd->imm[ch][i]*(1-long_mix);
                }
            }
        }
        
        BeatDetect();
        
        this->updateCount++;
    }
    
    AudioVis::AudioVis(){
        this->data = new AudioVisData();
        memset(this->fWaveform,0,sizeof(this->fWaveform));
        memset(this->fSpectrum,0,sizeof(this->fSpectrum));
        memset(this->cWaveformL,0,sizeof(this->cWaveformL));
        memset(this->cWaveformR,0,sizeof(this->cWaveformR));
        memset(this->imm,0,sizeof(this->imm));
        memset(this->long_avg,0,sizeof(this->long_avg));
        memset(this->med_avg,0,sizeof(this->med_avg));
        memset(this->avg,0,sizeof(this->avg));
        memset(this->engineerData,0,sizeof(this->engineerData));
        
        this->bDisabled = false;
        this->bClear = false;
        this->bIsBeat = false;
        this->updateCount = 0;
    }
    
    AudioVis::~AudioVis(){
        delete this->data;
    }

    void AudioVis::PrepData()
    {
        data->m_has_fallen   = 1;
        data->m_thresh       = 1;
        data->m_limit        = 1;
        
        data->frame = 0;
        
        data->fft = new FFT();
        data->fft->Init(576,512);
        return;
    }
}
