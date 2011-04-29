/*
  LICENSE
  -------
  Copyright (C) 1999-2002 Nullsoft, Inc.

  This source code is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this source code or the software it produces.

  Permission is granted to anyone to use this source code for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this source code must not be misrepresented; you must not
  claim that you wrote the original source code.  If you use this source code
  in a product, an acknowledgment in the product documentation would be
  appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
  misrepresented as being the original source code.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef __NULLSOFT_DX8_PLUGIN_SHELL_FFT_H__
#define __NULLSOFT_DX8_PLUGIN_SHELL_FFT_H__ 1


class FFT
{
public:
    FFT();
    ~FFT();
    void Init(int samples_in, int samples_out, int bEqualize=1, float envelope_power=1.0f);
    void time_to_frequency_domain(float *in_wavedata, float *out_spectraldata);
    int  GetNumFreq() { return NFREQ; };
    void CleanUp();
private:
    int m_ready;
    int m_samples_in;
    int NFREQ;

    void InitEnvelopeTable(float power);
    void InitEqualizeTable();
    void InitBitRevTable();
    void InitCosSinTable();
    
    int   *bitrevtable;
    float *envelope;
    float *equalize;
    float *temp1;
    float *temp2;
    float (*cossintable)[2];
};









#endif