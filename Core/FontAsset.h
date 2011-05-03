#pragma once

#include "Asset.h"
#include "Rect.h"

namespace Monocle
{
	class FontAsset : public Asset
	{
	public:
        FontAsset(): Asset(ASSET_FONT) {}

		virtual void GetGlyphData(char c, float* x, float* y, Rect& verts, Rect& texCoords) const = 0;
		virtual float GetTextWidth(const std::string &text) = 0;
		virtual float GetTextHeight(const std::string &text) = 0;
		//virtual float GetTextWidth(const std::string &text)=0;

		unsigned int texID;
	};
}