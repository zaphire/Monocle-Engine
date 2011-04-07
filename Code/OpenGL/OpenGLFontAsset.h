#pragma once

#include "../FontAsset.h"
#include "../Quad.h"

#include "stb_truetype.h"


namespace Monocle
{
    class OpenGLFontAsset: public FontAsset
    {
    public:
        OpenGLFontAsset();
        ~OpenGLFontAsset();

        bool Load(const std::string &filename, float size);

        void Reload();
        void Unload();

        void GetGlyphData(char c, float* x, float* y, Quad& verts, Quad& texCoords) const;

    protected:
        stbtt_bakedchar* fontCData;
        float size;
    };
}
