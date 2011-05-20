#pragma once

#include "../FileNode.h"
//#include "tinyxml.h"

class TiXmlElement;

namespace Monocle
{
	class XMLFileNode : public FileNode
	{
	public:
		XMLFileNode();
		XMLFileNode(TiXmlElement *element);
		~XMLFileNode();

		void Write(const std::string &name, const int &value);
		void Write(const std::string &name, const float &value);
		void Write(const std::string &name, const std::string &value);
		void Write(const std::string &name, const Vector2 &value);
		void Write(const std::string &name, const Color &value);
		void Write(const std::string &name, const bool &value);

		void Read(const std::string &name, int &value);
		void Read(const std::string &name, float &value);
		void Read(const std::string &name, std::string &value);
		void Read(const std::string &name, Vector2 &value);
		void Read(const std::string &name, Color &value);
		void Read(const std::string &name, bool &value);

		FileNode* FirstChildNode(const std::string &name);
		FileNode* NextChildNode(const std::string &name);

		//FileNode* NewNode(const std::string &name);
		//FileNode* InsertEndChildNode(FileNode *fileNode);

		TiXmlElement *element;
		XMLFileNode *iterator;
	};
}
