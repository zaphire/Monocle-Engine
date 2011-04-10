
#include "../TTFFontAsset.h"
#include "../Debug.h"


// STB Setup
#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation

#include <cstdlib>
#define STBTT_malloc(x,u)  malloc(x)
#define STBTT_free(x,u)    free(x)

#include "stb_truetype.h"


// OpenGL Headers
#if defined(MONOCLE_MAC)
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <OpenGL/glu.h>
#elif defined(MONOCLE_LINUX)
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#endif


namespace Monocle
{

	TTFFontAsset::TTFFontAsset()
        : FontAsset(), fontCData(NULL)
	{
	}
    
    TTFFontAsset::~TTFFontAsset()
    {
        Unload();
    }

    
	bool TTFFontAsset::Load(const std::string &filename, float size)
	{
        FILE *fp = fopen(filename.c_str(), "rb");
        if (fp == NULL)
        {
			Debug::Log("Failed to open font: " + filename);
            return false;
        }
        
        this->size = size;
        
        fontCData = (void*)(stbtt_bakedchar*)malloc(sizeof(stbtt_bakedchar) * 96);

        
        unsigned char* ttf_buffer = (unsigned char*)malloc(1 << 20);
        fread(ttf_buffer, 1, 1<<20, fp);
        fclose(fp);

        unsigned char* temp_bitmap = (unsigned char*)malloc(512 * 512);
        stbtt_BakeFontBitmap(ttf_buffer, 0, size, temp_bitmap, 512, 512, 32, 96, (stbtt_bakedchar*)fontCData);
        free(ttf_buffer);

        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
        
        free(temp_bitmap);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
        lineHeight = size;
        
        return true;
	}
    
    
	void TTFFontAsset::Reload()
	{
		Unload();
		Load(filename, size);
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

    
    void TTFFontAsset::GetGlyphData(char c, float* x, float* y, Rect& verts, Rect& texCoords) const
    {
        if (c == '\n')
        {
            
        }
        else
        {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad((stbtt_bakedchar *)fontCData, 512, 512, c-32, x, y, &q, 1);//1=opengl,0=old d3d
            verts.topLeft.x = q.x0;  verts.topLeft.y = q.y0;
            verts.bottomRight.x = q.x1;  verts.bottomRight.y = q.y1;
            texCoords.topLeft.x = q.s0;    texCoords.topLeft.y = q.t0;
            texCoords.bottomRight.x = q.s1;    texCoords.bottomRight.y = q.t1;
        }
    }
}
