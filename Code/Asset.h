#pragma once

namespace Monocle
{
	enum AssetType
	{
		Undefined,
		Image,
		Audio,
		/*
		Mesh,
		Text,
		XML,
		*/
		Max
	};

	class Asset
	{
	public:
		Asset();

		const char *name; // <- what string class are we using?

		AssetType type;

		int referenceCount; // <- how many things use this

		bool isCached; // <- should we free ourselves?

		void AddReference();
		void RemoveReference();

		virtual void Load(const char *filename);
		virtual void Unload();
	};
}