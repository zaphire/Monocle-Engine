#pragma once

#include <string>
#include "TextureAsset.h"

namespace Monocle
{
	class Tileset
	{
	public:
		Tileset(const std::string &name, const std::string &filename, float tileWidth, float tileHeight);
		
		std::string name;				// name of the set
		float tileWidth, tileHeight;		// how big is a tile in the set
		TextureAsset *texture;			// texture ref for the set
	};
}