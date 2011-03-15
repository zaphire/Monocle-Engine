#ifdef MONOCLE_OPENGL

#include "../TextureAsset.h"

//#include "glpng\glpng.h"

namespace Monocle
{
	TextureAsset::TextureAsset()
		: Asset(AT_TEXTURE)
	{
	}

	void TextureAsset::Load(const char *filename)
	{
		// load from file
		// check extension to figure out filetype?
		// assume png for now

	}

	void TextureAsset::Unload()
	{
	}

	void TextureAsset::Bind()
	{

	}
}

#endif