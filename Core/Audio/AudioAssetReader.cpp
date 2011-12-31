//
//  AudioAssetReader.cpp
//
//  Created by Josh Whelchel on 4/27/11.
//

#include "AudioAssetReader.h"
#include "../Debug.h"

#include <stdio.h> // for fopen, etc (GCC)
#include <stdlib.h> // for free and malloc (GCC)
#include <string.h> // for memcpy (GCC)

namespace Monocle {
    
    AudioAssetReader::AudioAssetReader( AudioAsset *asset, std::string key )
    {
        this->asset = asset;
        if (key != "")
            this->cryptKey = new AudioCryptKey(key);
        else
            this->cryptKey = NULL;
        
        file = 0;
        dataRead = 0;
        dataSize = asset->GetDataSize();
        dataPtr = 0;
        
        asset->AddReference();
        
        if (asset->IsStreaming())
        {
            Debug::Log("Streaming " + asset->filename);
            file = fopen(asset->filename.c_str(),"rb");
            fseek(file,0,SEEK_SET);
        }
        else
        {
            dataPtr = (char*)asset->GetDataBuffer();
        }
    }
    
    AudioAssetReader::~AudioAssetReader()
    {
        if (file)
            fclose(file);
        
        asset->RemoveReference();
        
        // Don't free the Data pointer, we come from an asset!
    }
    
    size_t AudioAssetReader::SpaceUntilEOF()
    {
        return dataSize - Tell();
        //return dataSize - dataRead;
    }
    
    size_t AudioAssetReader::Read(void *dst, size_t size)
    {
        if (file)
        {
            // STREAMING
            if (!cryptKey)
            {
                // Read simply, no decryption needed
                return fread(dst,1,size,file);
            }
            else
            {
                // We have to begin at a fresh multiple of 5 in the file to achieve this properly.
                size_t startOffset = Tell();
                size_t trueReadSize = size;
                size_t sizeAdjustToFive = startOffset%5;
                size_t sizeReadAhead = 0;
                size_t spaceUntilEOF;
                
                // Adjust the offset so we're starting at a multiple of 5 in the file.
                trueReadSize += sizeAdjustToFive;
                Seek(-1*sizeAdjustToFive,SEEK_CUR);
                
                // Pad the read with an extra 10 bytes
                sizeReadAhead = 10;
                
                // Make sure we can actually read ahead 10 bytes
                spaceUntilEOF = SpaceUntilEOF();
                if (trueReadSize+sizeReadAhead > spaceUntilEOF)
                    sizeReadAhead = spaceUntilEOF - trueReadSize;
                
                // Add the read ahead padding
                trueReadSize += sizeReadAhead;
                
                // Allocate enough temporary space to make up the difference
                void *tmpBuf = malloc(trueReadSize);
                
                // Read and decode
                fread(tmpBuf,1,trueReadSize,file);

                cryptKey->DecodeSlice5b((unsigned char*)tmpBuf + sizeAdjustToFive, (unsigned char*)dst, size, sizeAdjustToFive, sizeReadAhead+size);
                
                // Seek back what we may have read ahead
                Seek(-1*sizeReadAhead, SEEK_CUR);
                
                return size;
            }
        }
        else
        {
            // READ FROM MEMORY
            size_t actualSize = size;
            size_t space = SpaceUntilEOF();
            
            if (actualSize > space)
                actualSize = space;
            
            memcpy(dst, (char*)dataPtr + dataRead, actualSize);
            
            // DECRYPT
            if (cryptKey)
            {
                // This part will be REALLY cool... o_O
                unsigned long offset;
                //			unsigned long size;
                //			unsigned long pos;
                
                offset = (dataRead) % 5;
                
                /*for (pos=0;pos<(unsigned long)actualSizeToRead;pos+=5)
                 {
                 if (actualSizeToRead-pos <= 5) 
                 size = actualSizeToRead-pos;
                 else 
                 size = 5;
                 
                 vorbisData->key->DecodeSlice5b((unsigned char*)vorbisData->dataPtr + vorbisData->dataRead + pos, (unsigned char*)ptr+pos, size, offset, spaceToEOF-pos);
                 }*/
                
                cryptKey->DecodeSlice5b((unsigned char*)dataPtr + dataRead, (unsigned char*)dst, actualSize, offset, space);
            }
            
            dataRead += actualSize;
            
            return actualSize;
        }
    }
 
    void AudioAssetReader::Seek( size_t offset, int whence )
    {
        if (!file)
        {
            // Not streaming...
            
            size_t actualOffset;
            size_t spaceToEOF;
            
            // Goto where we wish to seek to
            switch (whence)
            {
                case SEEK_SET: // Seek to the start of the data file
                    // Make sure we are not going to the end of the file
                    if (dataSize >= offset)
                        actualOffset = offset;
                    else
                        actualOffset = dataSize;
                    // Set where we now are
                    dataRead = actualOffset;
                    break;
                case SEEK_CUR: // Seek from where we are
                    // Make sure we dont go past the end
                    spaceToEOF = SpaceUntilEOF();
                    if (offset < spaceToEOF)
                        actualOffset = (offset);
                    else
                        actualOffset = spaceToEOF;	
                    // Seek from our currrent location
                    dataRead += actualOffset;
                    break;
                case SEEK_END: // Seek from the end of the file
                    dataRead = dataSize+1;
                    break;
                default:
                    Debug::Log("Unknown seek command in AudioAssetReader::Seek\n"); // ERROR
                    break;
            };
        }
        else
        {
            // Streaming...
            fseek(file,offset,whence);
        }
    }
    
    size_t AudioAssetReader::Tell()
    {
        if (!file)
        {
            return dataRead;
        }
        else
        {
            return ftell(file);
        }
    }
    
    long AudioAssetReader::GetSize()
    {
        return dataSize;
    }
}
