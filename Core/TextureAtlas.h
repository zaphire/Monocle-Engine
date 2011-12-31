#pragma once

#include "FileNode.h"
#include "Vector2.h"

#include <map>

class TiXmlDocument;
class TiXmlElement;

namespace Monocle
{
	class TextureAtlas;

	class TextureAtlasEntry
	{
	public:
		TextureAtlasEntry();
		TextureAtlasEntry(TextureAtlas *textureAtlas);
		void Save(FileNode *fileNode);
		void Load(FileNode *fileNode);

		Vector2 GetTextureOffset();
		Vector2 GetTextureScale();
		int GetWidth();
		int GetHeight();

	private:
		friend class TextureAtlas;
		std::string name;
		int x, y, width, height;
		TextureAtlas *textureAtlas;
	};

	class TextureAtlas
	{
	public:
		TextureAtlas();
		~TextureAtlas();
		TextureAtlasEntry* GetEntryByName(const std::string &name);
		void Save(TiXmlDocument *document);
		void Load(TiXmlElement *element);
		std::string GetImageName();

	private:
		friend class TextureAtlasEntry;
		std::string image;
		int width, height;
		float scale;

	private:
		std::map<std::string, TextureAtlasEntry*> entries;
	};
}
