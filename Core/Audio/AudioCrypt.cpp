#include <math.h>
#include <string.h>
#include "AudioCrypt.h"

#include <stdio.h>

#define MIN(a,b) ((a<b)?a:b)

namespace Monocle
{

    AudioCryptKey::AudioCryptKey( std::string keyStr )
    {
        char *cKeyStr = (char*)keyStr.c_str();
        
        // First we need to generate the four parts from the key string.
        KeyStringToFourParts( cKeyStr, &m_nPart1, &m_nPart2, &m_nPart3, &m_nPart4 );

        // Then we need to generate the lookup tables.
        /**
            4 Table of lookup values are generated for every byte value (0-255) based on each key part.

            First, an offset is given via:
            offsetVal = ((lookupTable#-1 * 64))
            This means that lookup table 0 offset is 0, 1 is 64, and so on

            lookupTable[origValue] = (offsetVal + keyPart) % 256
        **/

        for (int x=0;x<256;x++)
            m_bLookup1[x] = m_nPart1 + (sin((float)x)*1000);
        
        for (int x=0;x<256;x++)
            m_bLookup2[x] = m_nPart2 + (sin((float)x)*2000);
        
        for (int x=0;x<256;x++)
            m_bLookup3[x] = m_nPart3 + (sin((float)x)*800);

        for (int x=0;x<256;x++)
            m_bLookup4[x] = m_nPart4 + (sin((float)x)*1200);

        ResetUses();
    }

    AudioCryptKey::~AudioCryptKey(void)
    {
    }

    unsigned long gcd( unsigned long a, unsigned long b )
    {
        if ( b == 0 ) return a;
        return gcd(b,a%b);
    }

    void AudioCryptKey::KeyStringToFourParts(char *cKeyStr, unsigned long *nPart1, unsigned long *nPart2, unsigned long *nPart3, unsigned long *nPart4)
    {
        /**
        DEFINING THE PARTS FROM A STRING...

        The string is divided into four parts.
        The sum of the bytes in each part go into sum1, sum2, sum3, sum4.

        part1 = sum1 * sum2
        part2 = sum2 * sum3
        part3 = gcd(part1,sum3)
        part4 = gcd(part2,sum4)
        **/

        unsigned long sum[4];
        nPart1[0]=nPart2[0]=nPart3[0]=nPart4[0] = 0;
        sum[0]=sum[1]=sum[2]=sum[3]=1;

        long len = strlen(cKeyStr);
        long plen = (len / 4) + 1; 
        char *cPtr = cKeyStr;

        // For each of four parts
        for (int z=0;z<4;z++)
        {
            // Calc the sum
            for (int i=0;i<plen;i++)
            {
                sum[z] += cPtr[0];
                cPtr++;

                if (cPtr > cKeyStr+len)break;
            }
            if (cPtr > cKeyStr+len)break;
        }

        nPart1[0] = (gcd(sum[1],sum[2]) + sum[3]) * (sum[0]);
        nPart2[0] = (gcd(sum[2],sum[3]) + sum[0]) * (sum[1]);
        nPart3[0] = (gcd(sum[3],sum[0]) + sum[1]) * (sum[2]);
        nPart4[0] = (gcd(sum[0],sum[1]) + sum[2]) * (sum[3]);
    }

    void AudioCryptKey::EncodeStream(unsigned char *pData, unsigned long nLength)
    {
        unsigned long pos;
        unsigned char *ptr = pData;

        unsigned char oldbyte[5], newbyte[4];
        long rdr;
        
        /** For every 4 bytes, lookie is defined
            lookie = lookupTable1[counter++%256] % 4
        **/
        for (pos=0;pos < nLength;pos+=5)
        {
            ptr = pData+pos;

            for (rdr=0;rdr<5;rdr++){
                if (ptr+rdr < pData+nLength)
                    oldbyte[rdr]=ptr[rdr];
                else
                    return;					// We better not mess with the last bit!
            }

            EncodeSegment(newbyte,oldbyte);

            for (rdr=0;rdr<4;rdr++){
                /*long sizer = (long)nLength-(long)pos;
                if (sizer-rdr > 0)*/
                if (ptr+rdr < pData+nLength)
                    ptr[rdr]=newbyte[rdr];
            }
        }
    }

    void AudioCryptKey::DecodeStream(unsigned char *pData, unsigned long nLength)
    {
        unsigned long pos;
        unsigned char *ptr = pData;

        unsigned char oldbyte[5], newbyte[4];
        long rdr;
        
        /** For every 4 bytes, lookie is defined
            lookie = lookupTable1[counter++%256] % 4
        **/
        for (pos=0;pos < nLength;pos+=5)
        {
            ptr = pData+pos;

            for (rdr=0;rdr<5;rdr++){
                if (ptr+rdr < pData+nLength)
                    oldbyte[rdr]=ptr[rdr];
                else
                    return;					// We better not mess with the last bit!
            }

            DecodeSegment(newbyte,oldbyte);

            for (rdr=0;rdr<4;rdr++){
                if (ptr+rdr < pData+nLength)
                    ptr[rdr]=newbyte[rdr];
            }
        }
    }

    void AudioCryptKey::EncodeSegment( unsigned char *newbyte, unsigned char *oldbyte )
    {
        unsigned char lookie = oldbyte[4]%4;
        m_lUses[lookie]++;

        if (lookie == 0)
        {
            newbyte[0] = (oldbyte[0] + m_bLookup4[oldbyte[3]]) % 256;
            newbyte[1] = (oldbyte[1] + m_bLookup3[oldbyte[2]]) % 256;
            newbyte[2] = (oldbyte[2] + m_bLookup2[oldbyte[0]]) % 256;
            newbyte[3] = (oldbyte[3] + m_bLookup1[m_nPart1%256]) % 256;		//<-- STATIC VALUE
        }
        else if (lookie == 1)
        {
            newbyte[0] = (oldbyte[0] + m_bLookup3[oldbyte[2]]) % 256;
            newbyte[1] = (oldbyte[1] + m_bLookup1[m_nPart2%256]) % 256;		//<-- STATIC VALUE
            newbyte[2] = (oldbyte[2] + m_bLookup2[oldbyte[1]]) % 256;
            newbyte[3] = (oldbyte[3] + m_bLookup4[oldbyte[0]]) % 256;
        }
        else if (lookie == 2)
        {
            newbyte[0] = (oldbyte[0] + m_bLookup4[oldbyte[3]]) % 256;
            newbyte[1] = (oldbyte[1] + m_bLookup3[oldbyte[2]]) % 256;
            newbyte[2] = (oldbyte[2] + m_bLookup1[m_nPart3%256]) % 256;		//<-- STATIC VALUE
            newbyte[3] = (oldbyte[3] + m_bLookup2[oldbyte[1]]) % 256;
        }
        else if (lookie == 3)
        {
            newbyte[0] = (oldbyte[0] + m_bLookup4[m_nPart4%256]) % 256;		//<-- STATIC VALUE
            newbyte[1] = (oldbyte[1] + m_bLookup1[oldbyte[3]]) % 256;
            newbyte[2] = (oldbyte[2] + m_bLookup2[oldbyte[0]]) % 256;
            newbyte[3] = (oldbyte[3] + m_bLookup3[oldbyte[2]]) % 256;
        }
    }

    void AudioCryptKey::DecodeSegment( unsigned char *newbyte, unsigned char *oldbyte )
    {
        unsigned char lookie = oldbyte[4]%4;
        m_lUses[lookie]++;

        if (lookie == 0)
        {
            newbyte[3] = (oldbyte[3] - m_bLookup1[m_nPart1%256]) % 256;		//<-- STATIC VALUE
            newbyte[0] = (oldbyte[0] - m_bLookup4[newbyte[3]]) % 256;
            newbyte[2] = (oldbyte[2] - m_bLookup2[newbyte[0]]) % 256;
            newbyte[1] = (oldbyte[1] - m_bLookup3[newbyte[2]]) % 256;
        }
        else if (lookie == 1)
        {
            newbyte[1] = (oldbyte[1] - m_bLookup1[m_nPart2%256]) % 256;		//<-- STATIC VALUE
            newbyte[2] = (oldbyte[2] - m_bLookup2[newbyte[1]]) % 256;
            newbyte[0] = (oldbyte[0] - m_bLookup3[newbyte[2]]) % 256;
            newbyte[3] = (oldbyte[3] - m_bLookup4[newbyte[0]]) % 256;
        }
        else if (lookie == 2)
        {
            newbyte[2] = (oldbyte[2] - m_bLookup1[m_nPart3%256]) % 256;		//<-- STATIC VALUE
            newbyte[1] = (oldbyte[1] - m_bLookup3[newbyte[2]]) % 256;
            newbyte[3] = (oldbyte[3] - m_bLookup2[newbyte[1]]) % 256;
            newbyte[0] = (oldbyte[0] - m_bLookup4[newbyte[3]]) % 256;
        }
        else if (lookie == 3)
        {
            newbyte[0] = (oldbyte[0] - m_bLookup4[m_nPart4%256]) % 256;		//<-- STATIC VALUE
            newbyte[2] = (oldbyte[2] - m_bLookup2[newbyte[0]]) % 256;
            newbyte[3] = (oldbyte[3] - m_bLookup3[newbyte[2]]) % 256;
            newbyte[1] = (oldbyte[1] - m_bLookup1[newbyte[3]]) % 256;
        }
    }

    /*
        "0123456789ABCDEF"
          ^
        Offset = 1
        Length = 11
        Full length = 16
        SegmentLen = 7 - (2) = 5

        "0123456789ABCDEF"
        " XXXXyyyyyWWZZZ "
        "aaaaabbbbbcccccd"
        "0___________0000"

        Offset = 3
        Length = 6
        Full length = 16
        SegmentLen = 3 - 3 = 0
        SegmentStart = 5-3 = 2

        "0123456789ABCDEF"
        "   XXWWWWZ000000"
        "aaaaabbbbbcccccd"
        "000______000000"

        Offset = 4
        Length = 12
        Full length = 12
        SegmentLen = 12-1 - (11%5) = 11 - 1 = 10
        SegmentStart = 1

        "0123456789ABCDEF"
        "    XyyyyyyyyyyW"
        "aaaaabbbbbcccccd"
        "0000____________"

        Offset = 0
        Length = 16
        Full length = 16
        SegmentLen = 15
        SegmentStart = 5-3 = 2

        "0123456789ABCDEF"
        "yyyyyyyyyyyyyyyW"
        "aaaaabbbbbcccccd"
        "________________"

        X = Partial at the front
        W = Partial at the back
        Z = Required Lookahead
        y = Avaiable to just read

        First y = Segment Start
        Segment Start = (5 - nOffset)
        Segment Length = (Length - Start) - ((Length - Start)%5)
        Segment End = SegmentStart+SegmentLength

        To read the front-partial you must:
            Read 5 bytes starting from Start-Offset (from original)
            Decode those 5 Bytes
            Store those 5-nOffset Bytes (starting at offset for each)

        To read the back-partial you must:
            Read MIN(5,FullLength-Length+1) bytes starting from Segment End
            Decode those 5 bytes
            Store those Length-(SegmentLength+(5-nOffset)) Bytes
                        
    */
    void AudioCryptKey::DecodeSlice5b(unsigned char *origData, unsigned char *newData, unsigned long nLength, unsigned long nOffset, unsigned long lookAhead)
    {
        unsigned long nSegmentStart = (5 - nOffset)%5;
        long nSegmentLenIsPos = (nLength-nSegmentStart)-((nLength-nSegmentStart)%5);
        unsigned long nSegmentLen = (nLength-nSegmentStart)-((nLength-nSegmentStart)%5);
        unsigned long nSegmentEnd = nSegmentStart+nSegmentLen;

        if (nSegmentLenIsPos > 0)
        { 
            // First we should copy the segment
            memcpy(newData + nSegmentStart,origData + nSegmentStart,nSegmentLen);
            // Then we need to decode it
            DecodeStream(newData + nSegmentStart, nSegmentLen);
        }
        
        if (nSegmentStart != 0)
        {
            unsigned char newbyte[5];
            unsigned char oldbyte[5];

            memset(oldbyte,0,5);
            memset(newbyte,0,5);

            unsigned long nrlen = 5;
            if (lookAhead < 5)
                nrlen = lookAhead;
            for (unsigned long i=0;i<nrlen;i++)
                oldbyte[i] = ((unsigned char*)(origData-(unsigned char*)nOffset))[i];

            DecodeSegment(newbyte,oldbyte);
            newbyte[4] = oldbyte[4];

            nrlen = 5-nOffset;
            if (nLength < nrlen)
                nrlen = nLength;
            for (unsigned long i=0;i+nOffset<5;i++)
                newData[i] = newbyte[i+nOffset];
        }

        /* We need to copy the very last section if we have to */
        if (lookAhead - nSegmentEnd < 5)
        {
            size_t len = nLength-(nSegmentLen+nSegmentStart);
            if (len > 0)
                memcpy(newData+nSegmentEnd,origData+nSegmentEnd,len);
        }
        else
        /*	To read the back-partial you must:
            Read MIN(5,FullLength-Length+1) bytes starting from Segment End
            Decode those 5 bytes
            Store those Length-(SegmentLength+(5-nOffset)) Bytes
        */
        {
            unsigned char newbyte[5];
            unsigned char oldbyte[5];

            memset(oldbyte,0,5);
            memset(newbyte,0,5);

            unsigned long nrlen = MIN(5,lookAhead-nLength+1);
            for (unsigned long i=0;i<nrlen;i++)
                oldbyte[i] = ((unsigned char*)(origData+nSegmentEnd))[i];

            DecodeSegment(newbyte,oldbyte);

            for (unsigned long i=0;i<5&&i+nSegmentEnd<nLength;i++)
                newData[i+nSegmentEnd] = newbyte[i];
        }

        /*
        unsigned char lookie;
        unsigned char newbyte[5];
        unsigned char oldbyte[5];

        memset(oldbyte,0,5);
        memset(newbyte,0,5);

        _ASSERT(nLength <= 5 && nLength > 0);
        _ASSERT(nOffset < 5 && nOffset >= 0);

        unsigned long nrlen = nLength+lookAhead;
        nrlen += nOffset;
        if (nrlen > 5) nrlen = 5;

        for (unsigned long i=0;i<nrlen;i++)
            oldbyte[i] = ((unsigned char*)(origData-(unsigned char*)nOffset))[i];

        lookie = oldbyte[4]%4;
        m_lUses[lookie]++;

        if (lookie == 0)
        {
            newbyte[3] = (oldbyte[3] - m_bLookup1[m_nPart1%256]) % 256;		//<-- STATIC VALUE
            newbyte[0] = (oldbyte[0] - m_bLookup4[newbyte[3]]) % 256;
            newbyte[2] = (oldbyte[2] - m_bLookup2[newbyte[0]]) % 256;
            newbyte[1] = (oldbyte[1] - m_bLookup3[newbyte[2]]) % 256;
        }
        else if (lookie == 1)
        {
            newbyte[1] = (oldbyte[1] - m_bLookup1[m_nPart2%256]) % 256;		//<-- STATIC VALUE
            newbyte[2] = (oldbyte[2] - m_bLookup2[newbyte[1]]) % 256;
            newbyte[0] = (oldbyte[0] - m_bLookup3[newbyte[2]]) % 256;
            newbyte[3] = (oldbyte[3] - m_bLookup4[newbyte[0]]) % 256;
        }
        else if (lookie == 2)
        {
            newbyte[2] = (oldbyte[2] - m_bLookup1[m_nPart3%256]) % 256;		//<-- STATIC VALUE
            newbyte[1] = (oldbyte[1] - m_bLookup3[newbyte[2]]) % 256;
            newbyte[3] = (oldbyte[3] - m_bLookup2[newbyte[1]]) % 256;
            newbyte[0] = (oldbyte[0] - m_bLookup4[newbyte[3]]) % 256;
        }
        else if (lookie == 3)
        {
            newbyte[0] = (oldbyte[0] - m_bLookup4[m_nPart4%256]) % 256;		//<-- STATIC VALUE
            newbyte[2] = (oldbyte[2] - m_bLookup2[newbyte[0]]) % 256;
            newbyte[3] = (oldbyte[3] - m_bLookup3[newbyte[2]]) % 256;
            newbyte[1] = (oldbyte[1] - m_bLookup1[newbyte[3]]) % 256;
        }

        // xxDATAxx

        nrlen = nLength+nOffset;
        if (nrlen > 4) nrlen = 4;
        for (unsigned long i=0;i<nrlen;i++)
            newData[i] = newbyte[i+nOffset];*/
    }

    void AudioCryptKey::ResetUses()
    {
        m_lUses[0]=m_lUses[1]=m_lUses[2]=m_lUses[3]=0;
    }
}