#pragma once

#include "Asset.h"

namespace Monocle
{
	class TextureAsset : Asset
	{
	public:
		void Load(const char *filename);
		void Unload();
	}
}