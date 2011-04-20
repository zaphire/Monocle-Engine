/*
 *  Generator.cpp
 *  GulpGulp
 *
 *  Created by Josh Whelchel on 11/19/09.
 *  Copyright 2009 jwmusic.org. All rights reserved.
 *
 */

#include <math.h>
#include "Generator.h"

Generator::Generator( int mods, int samplerate, int channels ) {
	m_mModulator = new Modulation(mods);
	m_fIncAmnt = 1.0/samplerate;
	m_fAmplitude = 0.4;
	m_nSampleRate = samplerate;
	m_nChannels = channels;
	m_fPan = 0.5;
	
	//m_nEnvelopeSamples = (float)samplerate*0.01;
	m_nEnvelopeSamples = 8;
	
	init();
}

Generator::~Generator() {
	delete m_mModulator;
}

void Generator::init() {
	reset();
}

void Generator::kill() {
}

void Generator::reset() {
}

void Generator::step() {
	m_mModulator->stepAll();
}

void Generator::ins_buff( unsigned char *buffer, unsigned long pos, float val, unsigned long allsize )
{
	short *ptr;
	ptr = ((short*)((unsigned long)buffer + pos));

	// Enveloping
	if (allsize>0){
		if (m_nChannels == 1){
			if (pos < m_nEnvelopeSamples)
				val *= (float)pos/(float)m_nEnvelopeSamples;
			else if (allsize - pos <= m_nEnvelopeSamples)
				val *= (float)(allsize-pos)/(float)m_nEnvelopeSamples;
		}else if (m_nChannels==2){
			unsigned long pos2 = floorf((float)pos/2.0);
			unsigned long allsize2 = floorf((float)allsize/2.0);
			if (pos2 < m_nEnvelopeSamples)
				val *= (float)pos2/(float)m_nEnvelopeSamples;
			else if (allsize2 - pos2 <= m_nEnvelopeSamples)
				val *= (float)(allsize2-pos2)/(float)m_nEnvelopeSamples;
		}
	}
	
	if (val >= 1.0)
		ptr[0] = 32767;
	else if (val <= -1.0)
		ptr[0] = -32766;
	else
		ptr[0] = val * 32766;
}

float Generator::get_buff( unsigned char *buffer, unsigned long pos )
{
	short val;
	short *ptr;
	ptr = ((short*)((unsigned long)buffer + pos));
	
	val = ptr[0];
	
	if (val >= 32767)
		return 1.0;
	else if (val <= -32766)
		return -1.0;
	else
		return val / 32766.0;
}

Modulation *Generator::getModulation()
{
	return m_mModulator;
}