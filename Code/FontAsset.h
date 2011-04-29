#pragma once

#include <vector>

#include "Asset.h"
#include "Rect.h"
#include "Range.h"

namespace Monocle
{
	class FontAsset : public Asset
	{
	public:
        // Range of glyphs used by font.  Doesn't need to be contiguous.
        typedef std::vector< Range<int> > GlyphRanges;
        
        
        FontAsset(): Asset(ASSET_FONT), texID(-1), lineHeight(0) {}

		virtual void GetGlyphData(int unicodeCodepoint, float* x, float* y, Rect& verts, Rect& texCoords) const = 0;
		virtual float GetTextWidth(const std::string &text) = 0;
		virtual float GetTextHeight(const std::string &text) = 0;

		unsigned int texID;
        float lineHeight;
	};
}
