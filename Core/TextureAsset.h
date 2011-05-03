#pragma once

#include "Asset.h"

namespace Monocle
{
	class TextureAsset : public Asset
	{
	public:
		TextureAsset();
		void Load(const std::string &filename, FilterType filter, bool repeatX, bool repeatY);
		void Reload();
		void Unload();

		FilterType filter;
		bool repeatX, repeatY;
		unsigned int width, height;
		unsigned int texID;
	};
}