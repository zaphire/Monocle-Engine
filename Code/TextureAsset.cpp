#include "TextureAsset.h"

namespace Monocle
{
	TextureAsset::TextureAsset()
		: Asset(ATTexture)
	{
	}

	void TextureAsset::Load(const char *filename)
	{
		// load from file
		// switch off extension to figure out filetype?

	}

	void TextureAsset::Unload()
	{
	}
}

