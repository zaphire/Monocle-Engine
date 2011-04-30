#pragma once

#include <string>

namespace Monocle 
{        

    /**
        Encrypts and decrypts a data stream
     */
    class AudioCryptKey
    {
    public:
        
        /**
         @param keyStr Decode string
         */
        AudioCryptKey( std::string keyStr );
        ~AudioCryptKey(void);

        /**
         Encodes an entire stream of data based on the key string.
         
         @param pData Data buffer
         @param nLength Length of data
         */
        void EncodeStream( unsigned char *pData, unsigned long nLength );
        
        /**
         Decodes an entire stream of data based on the key string. You cannot provide a stream that begins at an offset other than 5,
         and if the data is less than five bytes worth it will be assumed to be the END of the full stream of data.  If this is not the case,
         use DecodeSlice5b().
         
         @param pData Data buffer
         @param nLength Length of data
         */
        void DecodeStream( unsigned char *pData, unsigned long nLength );
        
        /**
         Decodes a slice of data in 5 byte chunks. This needs an entire 5 bytes plus more to look ahead and look behind (in order to align it
         to 5 byte slices in relation to the full stream of data.  To achieve this, nLength specifies the length of the data you wish to
         alter, with nOffset providing how many bytes away from being a multiple of 5 origData points to. 
         
         For instance, if origData begins on the 3rd byte, you should provide 3 for nOffset. If it is byte 7, you will provide 7%5 (= 2).
         
         lookAhead is responsible for notifying how many bytes of data are available to seek past the end of origData (you should provide
         the entire length remaining after nLength if possible, otherwise it simply needs to align to a full chunk of 5. When in doubt, read
         5 bytes OR the space until EOF if less).
         
         @param origData Pointer to the original data to read. Will read also before this pointer nOffset bytes and beyond it up to 5 (or lookAhead if less).
         @param newData Pointer to a buffer of nLength size to fill with decoded data.
         @param lookAhead How many bytes exist after nLength in origData pointer that can be read. Will read up to 5, but should be less if EOF is sooner.
         @param nOffset How many bytes away from a multiple 5 origData starts at in the entirety of the datastream. This is usually just Tell()%5.
         */
        void DecodeSlice5b(unsigned char *origData, unsigned char *newData, unsigned long nLength, unsigned long nOffset, unsigned long lookAhead=0);

        void ResetUses();

        unsigned long m_lUses[4];

    private:

        unsigned long m_nPart1;
        unsigned long m_nPart2;
        unsigned long m_nPart3;
        unsigned long m_nPart4;

        unsigned long m_bLookup1[ 256 ];
        unsigned long m_bLookup2[ 256 ];
        unsigned long m_bLookup3[ 256 ];
        unsigned long m_bLookup4[ 256 ];

        static void KeyStringToFourParts(char *cKeyStr, unsigned long *nPart1, unsigned long *nPart2, unsigned long *nPart3, unsigned long *nPart4);

        void DecodeSegment( unsigned char *newByte, unsigned char *oldByte );
        void EncodeSegment( unsigned char *newByte, unsigned char *oldByte );

    };

}