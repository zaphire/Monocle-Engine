#pragma once

#include <string>

namespace Monocle
{
	enum AssetType
	{
		AT_UNDEFINED,
		AT_TEXTURE,
		AT_AUDIO,
		AT_MAX
	};

	class Asset
	{
	public:
		Asset(AssetType type);

		std::string filename;

		AssetType type;

		int referenceCount; // <- how many things use this

		bool isCached; // <- should we free ourselves?

		void AddReference();
		void RemoveReference();

		virtual void Load(const std::string &filename) = 0;
		virtual void Unload() = 0;
	};
}