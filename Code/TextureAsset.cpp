#include "TextureAsset.h"

namespace Monocle
{
	TextureAsset::TextureAsset()
	{
	}

	void TextureAsset::Load(const char *filename)
	{
		Asset::Load();

		// load from file, whatever the file is
	}

	void TextureAsset::Unload()
	{
		Asset::Unload();
	}
}