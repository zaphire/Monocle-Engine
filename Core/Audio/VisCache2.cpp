#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "VisCache2.h"

namespace Monocle {

    VisCache2::VisCache2(void)
    {
        m_nBufLen = 0;
        m_buf = 0;
        m_lastIndex = 0;
        m_curIndex = 0;
        m_readIndex = 0;
    }

    VisCache2::~VisCache2(void)
    {
        if (m_buf) free(m_buf);
    }

    void VisCache2::Init(long maxLatency, long srate)
    {
        // 44100 p/s * (1s/1000ms) * (1000ms)
        m_nBufLen = (srate/1000) * (maxLatency);
        
        if (m_buf) free(m_buf);
        m_buf = (vc2_entry*)malloc(sizeof(vc2_entry)*m_nBufLen);

        //printf("Allocating %dkb for viscache\n",(sizeof(vc2_entry)*m_nBufLen)/1024);
        
        Reset();
    }

    void VisCache2::EndEntry()
    {
        m_curIndex++;
        if (m_curIndex >= m_nBufLen)
            m_curIndex = 0;
    }

    void VisCache2::Clean()
    {
        memset(m_buf,0,sizeof(vc2_entry)*m_nBufLen);
        // For 8 bit, the 'clean' value is actually 0x80
        if (m_buf){
            for (int i=0;i<m_nBufLen;i++){
                vc2_entry *e = &m_buf[i];
                memset(e->cWaveLeft,0x80,sizeof(e->cWaveLeft));
                memset(e->cWaveRight,0x80,sizeof(e->cWaveRight));
                e->isSet = false;
            }
        }
    }

    void VisCache2::Reset()
    {
        m_lastIndex = 0;
        m_curIndex = 0;
        m_readIndex = 0;
    }

    void VisCache2::SetWrittenTime(unsigned long pos)
    {
        vc2_entry *e = &m_buf[m_curIndex];
        e->nPos = pos;
        e->isSet = true;
    }

    void VisCache2::SetEngineerData(long data1, long data2, long data3, long data4)
    {
        vc2_entry *e = &m_buf[m_curIndex];
        e->engineerData[0] = data1;
        e->engineerData[1] = data2;
        e->engineerData[2] = data3;
        e->engineerData[3] = data4;
    }

    void VisCache2::PutWaveLeft(unsigned char *data)
    {
        vc2_entry *e = &m_buf[m_curIndex];
        memcpy(e->cWaveLeft,data,576);
    }

    void VisCache2::PutWaveRight(unsigned char *data)
    {
        vc2_entry *e = &m_buf[m_curIndex];
        memcpy(e->cWaveRight,data,576);
    }

    // Getting

    int VisCache2::GetLatentData(unsigned long pos)
    {
        // The trick here is positioning the index on the earliest data (but not too early)
        vc2_entry *e;
        long testednum = 0;
        long lastIndex = m_readIndex;

        m_lastIndex = m_readIndex;
        m_readIndex++;

        for (testednum=0;testednum<m_nBufLen;testednum++)
        {
            if (m_readIndex >= m_nBufLen)
                m_readIndex = 0;			// Wrap around

            e = &m_buf[m_readIndex];

            // Test if our position is AFTER the current's read position
            if (pos > e->nPos)
            {
                // If our position is ahead of this current buffer's position, we can use it
                m_lastIndex = m_readIndex;
                m_readIndex++;
            }
            else
            {
                // If the position we are reading is AFTER our position (We aren't here yet) use the last index
                m_readIndex = m_lastIndex;
                break;
            }


            // If we've been around the block we should break out. There's nothing here for us yet.
            // That's the point of the for loop we're encased in.		
        }

        return (m_readIndex != lastIndex); // Returns 0 if it hasn't changed
    }

    void VisCache2::GetWaveLeft(unsigned char *out)
    {
        vc2_entry *e = &m_buf[m_readIndex];
        memcpy(out,e->cWaveLeft,576);
    }

    void VisCache2::GetWaveRight(unsigned char *out)
    {
        vc2_entry *e = &m_buf[m_readIndex];
        memcpy(out,e->cWaveRight,576);
    }

    void VisCache2::GetEngineerData(long *data1, long *data2, long *data3, long *data4)
    {
        vc2_entry *e = &m_buf[m_readIndex];

        if (data1) data1[0] = e->engineerData[0];
        if (data2) data2[0] = e->engineerData[1];
        if (data3) data3[0] = e->engineerData[2];
        if (data4) data4[0] = e->engineerData[3];
    }

    void VisCache2::Destroy()
    {
        if (m_buf) free(m_buf);
        m_buf = 0;
        m_nBufLen = 0;
        m_readIndex = 0;
        m_lastIndex = 0;
        m_curIndex = 0;
    }
}