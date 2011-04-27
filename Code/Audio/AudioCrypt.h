#pragma once

/**
	responsible for encrypting and decrypting byte streams
**/

namespace Monocle 
{        

    class AudioCryptKey
    {
    public:
        AudioCryptKey( char *cKeyStr );
        ~AudioCryptKey(void);

        void EncodeStream( unsigned char *pData, unsigned long nLength );
        void DecodeStream( unsigned char *pData, unsigned long nLength );
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