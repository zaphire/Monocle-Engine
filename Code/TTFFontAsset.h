#pragma once

#include "FontAsset.h"

#include <map>

namespace Monocle
{
    class TTFFontAsset: public FontAsset
    {
    public:
        TTFFontAsset();
        ~TTFFontAsset();
        
        bool Load(const std::string &filename, float size, int textureWidth, int textureHeight, const GlyphRanges& ranges);

        void Reload();
        void Unload();

		void GetGlyphData(int unicodeCodepoint, float* x, float* y, Rect& verts, Rect& texCoords) const;
		float GetTextWidth(const std::string &text);
		float GetTextHeight(const std::string &text);

    protected:
        struct BakedChar
        {
            unsigned short x0, y0, x1, y1; // coordinates of bbox in bitmap
            float xoff, yoff, xadvance;
        };
        
        struct AlignedQuad
        {
            float x0, y0, s0, t0; // top-left
            float x1, y1, s1, t1; // bottom-right
        };

        
        int BakeFontBitmap(const unsigned char *data, int offset, float pixel_height,
                           unsigned char *pixels, int pw, int ph, const GlyphRanges& ranges, BakedChar *chardata);

        void GetBakedQuad(BakedChar *chardata, int pw, int ph, int unicodeCodepoint, float *xpos, float *ypos, AlignedQuad *q) const;

        
        typedef std::map<int, int> GlyphMap;
        GlyphMap glyphMap;
        BakedChar* fontCData;
        float size;
		int textureWidth, textureHeight;
        GlyphRanges glyphRanges;
    };
}
