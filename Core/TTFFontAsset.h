#pragma once

#include "FontAsset.h"

namespace Monocle
{
    class TTFFontAsset: public FontAsset
    {
    public:
        TTFFontAsset();
        ~TTFFontAsset();

        bool Load(const std::string &filename, float size, int textureWidth=-1, int textureHeight=-1);

        void Reload();
        void Unload();

		void GetGlyphData(char c, float* x, float* y, Rect& verts, Rect& texCoords) const;
		float GetTextWidth(const std::string &text) const;
		float GetTextHeight(const std::string &text) const;
		//float GetTextWidth(const std::string &text);

    protected:
        void* fontCData;
        float size;
		int textureWidth, textureHeight;
    };
}
