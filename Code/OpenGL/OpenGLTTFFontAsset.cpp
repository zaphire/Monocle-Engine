#include "../TTFFontAsset.h"

#include <algorithm>

#include "../Debug.h"

// STB Setup
#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation

#include <cstdlib>
#include <fstream>
#define STBTT_malloc(x,u)  malloc(x)
#define STBTT_free(x,u)    free(x)

#include "stb_truetype.h"

// OpenGL Headers
#include "GL/glew.h"

#include "../Macros.h"

// If DirectX supported in the future, change this define accordingly
#define USE_OPENGL


namespace Monocle
{
	TTFFontAsset::TTFFontAsset()
        : FontAsset(), fontCData(NULL), textureWidth(512), textureHeight(512), glyphRanges(GlyphRanges())
	{
	}
    
    TTFFontAsset::~TTFFontAsset()
    {
        Unload();
    }

	bool TTFFontAsset::Load(const std::string &filename, float size, int textureWidth, int textureHeight, const GlyphRanges& glyphRanges)
	{
		if (textureWidth != -1)
			this->textureWidth = textureWidth;
		if (textureHeight != -1)
			this->textureHeight = textureHeight;
        
        this->glyphRanges = glyphRanges;

        FILE *fp = fopen(filename.c_str(), "rb");
        if (fp == NULL)
        {
			Debug::Log("Error: Failed to open font: " + filename);
            return false;
        }
        
        this->size = size;
        
        fontCData = (BakedChar*)malloc(sizeof(BakedChar) * 96);

        unsigned char* ttf_buffer = (unsigned char*)malloc(1 << 20);
        fread(ttf_buffer, 1, 1<<20, fp);
        fclose(fp);

        unsigned char* temp_bitmap = (unsigned char*)malloc(this->textureWidth * this->textureHeight);
        BakeFontBitmap(ttf_buffer, 0, size, temp_bitmap, this->textureWidth, this->textureHeight, glyphRanges, fontCData);
        free(ttf_buffer);

        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, this->textureWidth, this->textureHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
        
        free(temp_bitmap);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
        lineHeight = size;
        
        return true;
	}

    
	void TTFFontAsset::Reload()
	{
		Unload();
		Load(filename, size, textureWidth, textureHeight, glyphRanges);
	}
    
	void TTFFontAsset::Unload()
	{
		Debug::Log("Freeing font texture memory for: " + filename);
		glDeleteTextures(1, &texID);
        
        if (fontCData)
        {
			free(fontCData);
			fontCData = NULL;
		}
	}

	float TTFFontAsset::GetTextWidth(const std::string &text)
	{
		float width = 0;
		for (int i = 0; i < text.size(); i++)
		{
			float x=0, y=0;
			Rect verts;
			Rect texCoords;
			GetGlyphData(text[i], &x, &y, verts, texCoords);
			width += verts.bottomRight.x - verts.topLeft.x;
		}
		return width;
	}

	float TTFFontAsset::GetTextHeight(const std::string &text)
	{
		float height = 0;
		for (int i = 0; i < text.size(); i++)
		{
			float x=0, y=0;
			Rect verts;
			Rect texCoords;
			GetGlyphData(text[i], &x, &y, verts, texCoords);
			height = MAX(height, verts.bottomRight.y - verts.topLeft.y);
		}
		return height;
	}

	void TTFFontAsset::GetGlyphData(int unicodeCodepoint, float* x, float* y, Rect& verts, Rect& texCoords) const
	{
        AlignedQuad q;
        GetBakedQuad(fontCData, textureWidth, textureHeight, unicodeCodepoint, x, y, &q);

        verts.topLeft.x = q.x0;
		verts.topLeft.y = q.y0;
        verts.bottomRight.x = q.x1;
		verts.bottomRight.y = q.y1;

        texCoords.topLeft.x = q.s0;
		texCoords.topLeft.y = q.t0;
        texCoords.bottomRight.x = q.s1;
		texCoords.bottomRight.y = q.t1;
    }

    // Bake a given set of ranges of chars
    int TTFFontAsset::BakeFontBitmap(const unsigned char *data, int offset,  // font location (use offset=0 for plain .ttf)
                                     float pixel_height,                     // height of font in pixels
                                     unsigned char *pixels, int pw, int ph,  // bitmap to be filled in
                                     const GlyphRanges& ranges,              // characters to bake
                                     BakedChar *chardata)
    {
        stbtt_fontinfo f;
        stbtt_InitFont(&f, data, offset);
        STBTT_memset(pixels, 0, pw*ph); // background of 0 around pixels
        
        int x = 1, y = 1;
        int bottom_y = 1;
        
        glyphMap.clear();
        
        float scale = stbtt_ScaleForPixelHeight(&f, pixel_height);
        
        int bakedIdx = 0;
        for (GlyphRanges::const_iterator iter = ranges.begin(); iter != ranges.end(); ++iter)
        {
            const Range<int>& range = *iter;
            int numItemsInRange = range.high - range.low;
            for (int i = 0; i <= numItemsInRange; i++, bakedIdx++)
            {
                int unicodeCodepoint = i + range.low;
                glyphMap.insert(std::make_pair(unicodeCodepoint, bakedIdx));
                
                int advance, lsb, x0,y0,x1,y1,gw,gh;
                int g = stbtt_FindGlyphIndex(&f, unicodeCodepoint);
                stbtt_GetGlyphHMetrics(&f, g, &advance, &lsb);
                stbtt_GetGlyphBitmapBox(&f, g, scale,scale, &x0,&y0,&x1,&y1);
                
                gw = x1-x0;
                gh = y1-y0;
                if (x + gw + 1 >= pw)
                    y = bottom_y, x = 1; // advance to next row
                if (y + gh + 1 >= ph) // check if it fits vertically AFTER potentially moving to next row
                    return -i;
                
                STBTT_assert(x+gw < pw);
                STBTT_assert(y+gh < ph);
                
                stbtt_MakeGlyphBitmap(&f, pixels+x+y*pw, gw,gh,pw, scale,scale, g);
                
                chardata[bakedIdx].x0 = (stbtt_int16) x;
                chardata[bakedIdx].y0 = (stbtt_int16) y;
                chardata[bakedIdx].x1 = (stbtt_int16) (x + gw);
                chardata[bakedIdx].y1 = (stbtt_int16) (y + gh);
                chardata[bakedIdx].xadvance = scale * advance;
                chardata[bakedIdx].xoff     = (float) x0;
                chardata[bakedIdx].yoff     = (float) y0;
                
                x = x + gw + 2;
                if (y+gh+2 > bottom_y)
                    bottom_y = y+gh+2;
            }
        }
        return bottom_y;
    }
    
    
    void TTFFontAsset::GetBakedQuad(BakedChar *chardata, int pw, int ph, int unicodeCodepoint, float *xpos,
                                    float *ypos, AlignedQuad *q) const
    {
        // Bias for DirectX rendering
#ifdef USE_OPENGL
        float d3d_bias = 0;
#else
        float d3d_bias = -0.5f;
#endif // USE_OPENGL
        
        float ipw = 1.0f / pw, iph = 1.0f / ph;
        GlyphMap::const_iterator codepointIter = glyphMap.find(unicodeCodepoint);
        if (codepointIter != glyphMap.end())
        {
            int bakedIdx = codepointIter->second;
            BakedChar *b = chardata + bakedIdx;
            int round_x = STBTT_ifloor((*xpos + b->xoff) + 0.5);
            int round_y = STBTT_ifloor((*ypos + b->yoff) + 0.5);
            
            q->x0 = round_x + d3d_bias;
            q->y0 = round_y + d3d_bias;
            q->x1 = round_x + b->x1 - b->x0 + d3d_bias;
            q->y1 = round_y + b->y1 - b->y0 + d3d_bias;
            
            q->s0 = b->x0 * ipw;
            q->t0 = b->y0 * ipw;
            q->s1 = b->x1 * iph;
            q->t1 = b->y1 * iph;
            
            *xpos += b->xadvance;
        }
    }
}
