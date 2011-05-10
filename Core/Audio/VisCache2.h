#pragma once

namespace Monocle {

    typedef struct vc2_entry_s
    {
        // 576 8bit Stereo Samples 
        unsigned char	cWaveLeft[576];
        unsigned char	cWaveRight[576];

        // With a recorded time
        unsigned long	nPos;

        // Engineer Data (4 longs should be plenty)
        // (eg, want to store information on the current order of a module, etc)
        long			engineerData[4];
        
        bool            isSet;
    } vc2_entry;

    /**
        A cache mechanism for syncing written audio data with when it is actually played through the speakers.
     */
    class VisCache2
    {
    public:
        VisCache2(void);
    public:
        ~VisCache2(void);

    public:

        // Init with max latency in MS, Sample rate in Samples per Second
        void Init( long maxLatency, long srate );
        void Clean();	
        void Reset();
        
        // Sets data to be retrieved later at position "pos" in MS
        void SetWrittenTime( unsigned long pos );

        void PutWaveLeft( unsigned char * data );
        void PutWaveRight( unsigned char * data );

        void SetEngineerData( long data1, long data2, long data3, long data4 );

        void EndEntry();

        // Gets the latent data at play position "pos" in MS
        // This will return 0 if it's nothing new
        int GetLatentData( unsigned long pos );

        void GetWaveLeft(unsigned char *out);
        void GetWaveRight(unsigned char *out);

        void GetEngineerData( long *data1, long *data2, long *data3, long *data4 );

        void Destroy();

    private:

        long m_nBufLen;
        vc2_entry *m_buf;

        long m_lastIndex;
        long m_curIndex;
        long m_readIndex;
    };
}
