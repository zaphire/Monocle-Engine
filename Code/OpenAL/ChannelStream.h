/*
 *  ChannelStream.h
 *  GulpGulp
 *
 *  Created by Josh Whelchel on 11/18/09.
 *  Copyright 2009 jwmusic.org. All rights reserved.
 *
 */
 
#pragma once

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include "Generator.h"

#define BUFFER_SIZE (4096 * 128)
#define NUM_BUFFERS 3

class ChannelStream {
	
public:
	ChannelStream();
	~ChannelStream();
	
public:
	
	void open( int channels, int bits, int samplerate );
	void close();
	
	// Returns number of buffers to fill
	int needsUpdate();
	
	// When update is needed, use getBuffer() to obtain the buffer to fill
	unsigned char *getBuffer( unsigned int *size );
	
	// Use lockBuffer() after you've filled the buffer obtained from getBuffer()
	void lockBuffer( unsigned int size );
	
	void lockNumberedBuffer( unsigned int size, unsigned int buff );
	unsigned char *getStaticBuffer( unsigned int *size );
	
	void play();
	void stop();
	
	bool isPlaying();
	void check();                 // checks OpenAL error state
	
	static int init();
	static void exit();
	
	float getPlaybackPosition();
	
protected:
	
	void empty();                 // empties the queue
	char *errorString(int code); // stringify an error code	

private:
	
	ALuint buffers[NUM_BUFFERS]; // buffers
	ALuint source;     // audio source
	ALenum format;     // format
	ALuint active_buffer; // active buffer (0 or 1)
	ALuint samplerate;
	
	bool started;
	
	unsigned char obtainedBuffer[BUFFER_SIZE];
};

