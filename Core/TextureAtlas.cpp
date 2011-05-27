#include "TextureAtlas.h"

#include <XML/XMLFileNode.h>
#include <TinyXML/tinyxml.h>

#include <sstream>

namespace Monocle
{
	TextureAtlasEntry::TextureAtlasEntry()
		: textureAtlas(NULL), x(0), y(0), width(0), height(0)
	{
	}

	TextureAtlasEntry::TextureAtlasEntry(TextureAtlas *textureAtlas)
		: textureAtlas(textureAtlas), x(0), y(0), width(0), height(0)
	{
	}

	int TextureAtlasEntry::GetWidth()
	{
		return width;
	}

	int TextureAtlasEntry::GetHeight()
	{
		return height;
	}

	Vector2 TextureAtlasEntry::GetTextureOffset()
	{
		return Vector2(x/float(textureAtlas->width), y/float(textureAtlas->height));
	}

	Vector2 TextureAtlasEntry::GetTextureScale()
	{
		return Vector2(width/float(textureAtlas->width), height/float(textureAtlas->height));
	}

	void TextureAtlasEntry::Save(FileNode *fileNode)
	{
		fileNode->Write("name", name);
		std::ostringstream os;
		os << x << " " << y << " " << width << " " << height;
		fileNode->Write("coords", os.str());
	}

	void TextureAtlasEntry::Load(FileNode *fileNode)
	{
		fileNode->Read("name", name);
		std::string read;
		fileNode->Read("coords", read);
		std::istringstream is(read);
		is >> x >> y >> width >> height;
	}

	TextureAtlas::TextureAtlas()
		: width(0), height(0)
	{
	}

	TextureAtlas::~TextureAtlas()
	{
		for (std::map<std::string, TextureAtlasEntry*>::iterator i = entries.begin(); i != entries.end(); ++i)
		{
			delete (*i).second;
		}
		entries.clear();
	}

	TextureAtlasEntry* TextureAtlas::GetEntryByName(const std::string &name)
	{
		return entries[name];
	}

	void TextureAtlas::Save(TiXmlDocument *document)
	{
		TiXmlElement xmlTextureAtlas("TextureAtlas");
		XMLFileNode xmlFileNodeTextureAtlas(&xmlTextureAtlas);
		xmlFileNodeTextureAtlas.Write("image", image);
		xmlFileNodeTextureAtlas.Write("width", width);
		xmlFileNodeTextureAtlas.Write("height", height);

		for (std::map<std::string, TextureAtlasEntry*>::iterator i = entries.begin(); i != entries.end(); ++i)
		{
			TextureAtlasEntry *textureAtlasEntry = (*i).second;

			TiXmlElement xmlAtlasEntry("TextureAtlasEntry");
			XMLFileNode xmlFileNode(&xmlAtlasEntry);
			textureAtlasEntry->Save(&xmlFileNode);

			xmlTextureAtlas.InsertEndChild(xmlAtlasEntry);
		}

		document->InsertEndChild(xmlTextureAtlas);
	}

	void TextureAtlas::Load(TiXmlElement *element)
	{
		XMLFileNode xmlFileNodeTextureAtlas(element);
		xmlFileNodeTextureAtlas.Read("image", image);
		xmlFileNodeTextureAtlas.Read("width", width);
		xmlFileNodeTextureAtlas.Read("height", height);

		TiXmlElement *xmlAtlasEntry = element->FirstChildElement("TextureAtlasEntry");
		while (xmlAtlasEntry)
		{
			XMLFileNode xmlFileNode(xmlAtlasEntry);

			TextureAtlasEntry *textureAtlasEntry = new TextureAtlasEntry(this);
			textureAtlasEntry->Load(&xmlFileNode);
			if (textureAtlasEntry->name != "")
				entries[textureAtlasEntry->name] = textureAtlasEntry;

			xmlAtlasEntry = xmlAtlasEntry->NextSiblingElement("TextureAtlasEntry");
		}
	}

	std::string TextureAtlas::GetImageName()
	{
		return image;
	}
}
