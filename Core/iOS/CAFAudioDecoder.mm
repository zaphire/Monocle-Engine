//
//  CAFAudioDecoder.m
//  MonocleIOS
//
//  Created by Josh Whelchel on 8/4/11.
//  Copyright 2011 jwmusic.org. All rights reserved.
//

#ifdef MONOCLE_IOS

#import "CAFAudioDecoder.h"
#include "MonocleToolkit.h"
#include "Debug.h"

namespace Monocle
{
    /***
     THIS CODE is responsible for registering this decoder with the file extension.
     See more info in Audio::Init.
     ***/
    AudioDecoder *makeCAFDecFunc( AudioAsset *asset )
    {
        return new CAFDecoder(asset);
    }
        
    CAFDecoder::CAFDecoder( AudioAsset *asset )
    {
        OSStatus						status = noErr;
        NSString *filePath = [NSString stringWithCString:asset->filename.c_str() 
                                                    encoding:[NSString defaultCStringEncoding]];
        NSURL *fileURL = [NSURL fileURLWithPath:filePath];
        
        UInt32							thePropertySize = sizeof(theFileFormat);
        
        this->extRef = NULL;
        this->audAsset = asset;

        // Open a file with ExtAudioFileOpen()
        status = ExtAudioFileOpenURL((CFURLRef)fileURL, &extRef);
        if (status != noErr)
        {
            extRef = NULL;
            return;
        }
        
        status = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileDataFormat, &thePropertySize, &theFileFormat);
        if (status != noErr)
        {
            ExtAudioFileDispose(extRef);
            extRef = NULL;
            return;
        }
        
        theOutputFormat.mSampleRate = theFileFormat.mSampleRate;
        theOutputFormat.mChannelsPerFrame = theFileFormat.mChannelsPerFrame;
        
        theOutputFormat.mFormatID = kAudioFormatLinearPCM;
        theOutputFormat.mBytesPerPacket = 2 * theOutputFormat.mChannelsPerFrame;
        theOutputFormat.mFramesPerPacket = 1;
        theOutputFormat.mBytesPerFrame = 2 * theOutputFormat.mChannelsPerFrame;
        theOutputFormat.mBitsPerChannel = 16;
        theOutputFormat.mFormatFlags = kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;
        
        status = ExtAudioFileSetProperty(extRef, kExtAudioFileProperty_ClientDataFormat, sizeof(theOutputFormat), &theOutputFormat);
        if (status != noErr)
        {
            ExtAudioFileDispose(extRef);
            extRef = NULL;
            return;
        }
        
        thePropertySize = sizeof(theFileLengthInFrames);
        status = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileLengthFrames, &thePropertySize, &theFileLengthInFrames);
        if (status != noErr)
        {
            ExtAudioFileDispose(extRef);
            extRef = NULL;
            return;
        }
        
        Init(theOutputFormat.mSampleRate, theOutputFormat.mBitsPerChannel, theOutputFormat.mChannelsPerFrame);
        
        total = (long)(((float)theFileLengthInFrames/(float)samplerate) * 1000.0f); 
    }
    
    unsigned long CAFDecoder::Render( unsigned long size, void *buf )
    {
        OSStatus						status = noErr;
        UInt32 readSize = size;
        
        if (!extRef){
            outOfData = true;
            return 0;
        }
        
        if (seekOffset != -1)
        {
            long seek = (long)(((float)seekOffset/1000.0f) * (float)samplerate);
            ExtAudioFileSeek(extRef,seek);
            seekOffset = -1;
            outOfData = false;
        }
        
        AudioBufferList		theDataBuffer;
		theDataBuffer.mNumberBuffers = 1;
		theDataBuffer.mBuffers[0].mDataByteSize = size;
		theDataBuffer.mBuffers[0].mNumberChannels = theOutputFormat.mChannelsPerFrame;
		theDataBuffer.mBuffers[0].mData = buf;
        
        readSize = size / theOutputFormat.mBytesPerFrame;
		
		// Read the data into an AudioBufferList
		status = ExtAudioFileRead(extRef, (UInt32*)&readSize, &theDataBuffer);
		if(status == noErr)
		{
			// success
            readSize *= theOutputFormat.mBytesPerFrame;
            
            if (readSize < size && (loopsRemaining!=0))
            {
                unsigned long offset = readSize;
                readSize = size - readSize;
                
                theDataBuffer.mBuffers[0].mDataByteSize = readSize;
                theDataBuffer.mBuffers[0].mNumberChannels = theOutputFormat.mChannelsPerFrame;
                theDataBuffer.mBuffers[0].mData = (void*)((unsigned long)buf + offset);
                
                readSize = readSize / theOutputFormat.mBytesPerFrame;
                
                ExtAudioFileSeek(extRef,0);  // Loop back position, TODO: If we want to loop to a specific point, here is the code for that!
                ExtAudioFileRead(extRef, (UInt32*)&readSize, &theDataBuffer);
                
                readSize *= theOutputFormat.mBytesPerFrame;
                
                if (loopsRemaining > 0)
                    loopsRemaining--;
            }
            else if (readSize < size && (loopsRemaining==0)){
                outOfData = true;
            }
            
			return readSize;
		}
		else
		{
			return 0;
		}
    }
    
    CAFDecoder::~CAFDecoder()
    {
        if (extRef) ExtAudioFileDispose(extRef);
    }
}


#endif

