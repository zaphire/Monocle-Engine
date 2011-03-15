#pragma once

namespace Monocle
{
	enum AssetType
	{
		ATUndefined,
		ATTexture,
		ATAudio,
		/*
		ATMesh,
		ATText,
		ATXML,
		*/
		ATMax
	};

	class Asset
	{
	public:
		Asset(AssetType type);

		const char *name; // <- what string class are we using?

		AssetType type;

		int referenceCount; // <- how many things use this

		bool isCached; // <- should we free ourselves?

		void AddReference();
		void RemoveReference();

		virtual void Load(const char *filename) = 0;
		virtual void Unload() = 0;
	};
}

