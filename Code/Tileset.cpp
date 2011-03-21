#include "Tileset.h"
#include "Assets.h"

namespace Monocle
{
	Tileset::Tileset(const std::string &name, const std::string &filename, float tileWidth, float tileHeight)
		: texture(NULL), tileWidth(tileWidth), tileHeight(tileHeight)
	{
		texture = Assets::RequestTexture(filename, FILTER_NONE);
	}
}