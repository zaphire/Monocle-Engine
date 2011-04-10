#pragma once

#include "Asset.h"
#include "Rect.h"

namespace Monocle
{
	class FontAsset : public Asset
	{
	public:
        FontAsset(): Asset(ASSET_FONT), texID(-1), lineHeight(0) {}

        virtual void GetGlyphData(char c, float* x, float* y, Rect& verts, Rect& texCoords) const = 0;

		unsigned int texID;
        float lineHeight;
	};
}
