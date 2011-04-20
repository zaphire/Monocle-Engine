/*
 *  Generator.h
 *  GulpGulp
 *
 *  Created by Josh Whelchel on 11/19/09.
 *  Copyright 2009 University of Cincinnati. All rights reserved.
 *
 */

#pragma once
#include "Modulation.h"

class Generator {
	
public:
	
	Generator( int mods, int samplerate, int channels );
	~Generator();
	
	void init();
	void kill();
	
	void reset();
	
	Modulation *getModulation();
	
	void step();
	virtual unsigned long fillBuffer( unsigned char *buffer, unsigned long size ) = 0;
	
	void setAmplitude(float amp) {
		m_fAmplitude = amp;
	}
	
	float getAmplitude(){
		return m_fAmplitude;
	}
	
	float getPan(){
		return m_fPan;
	}
	
	void setPan( float pan ){
		m_fPan = pan;
	}
	
protected:
	
	Modulation *m_mModulator;
	float m_fIncAmnt;
	float m_fAmplitude;
	float m_fPan;			// 0.0 is LEFT, 1.0 is RIGHT, 0.5 is CENTER
	int m_nSampleRate;
	int m_nChannels;
	int m_nEnvelopeSamples;
	
	void ins_buff( unsigned char *buffer, unsigned long pos, float val, unsigned long allsize = 0 );
	float get_buff( unsigned char *buffer, unsigned long pos );
};