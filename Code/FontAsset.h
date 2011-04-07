#pragma once

#include "Asset.h"
#include "Quad.h"

namespace Monocle
{
	class FontAsset : public Asset
	{
	public:
        FontAsset(): Asset(ASSET_FONT) {}

        virtual void GetGlyphData(char c, float* x, float* y, Quad& verts, Quad& texCoords) const = 0;

		unsigned int texID;
	};
}