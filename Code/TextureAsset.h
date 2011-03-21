#pragma once

#include "Asset.h"

namespace Monocle
{
	class TextureAsset : public Asset
	{
	public:
		TextureAsset();
		void Load(const std::string &filename, FilterType filter, bool repeatX, bool repeatY);
		void Unload();
		void Bind();

		unsigned int width, height;

		unsigned int texID;
	};
}