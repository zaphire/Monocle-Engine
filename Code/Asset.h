#pragma once

#include <string>

namespace Monocle
{
	enum AssetType
	{
		ASSET_UNDEFINED,
		ASSET_TEXTURE,
		ASSET_AUDIO,
		ASSET_MAX,
	};

	enum FilterType
	{
		FILTER_NONE,
		FILTER_LINEAR
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
		virtual void Reload()=0;

	protected:
		friend class Assets;
		// allow derived classes to add whatever params they want to load
		//virtual void Load(const std::string &filename)=0;
		virtual void Unload() = 0;
	};
}