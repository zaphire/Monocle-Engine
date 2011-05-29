#pragma once

#include <string>

namespace Monocle
{
	class Vector2;
	class Color;
	class Entity;

	class FileNode
	{
	public:
		virtual void Write(const std::string &name, const int &value)=0;
		virtual void Write(const std::string &name, const float &value)=0;
		virtual void Write(const std::string &name, const std::string &value)=0;
		virtual void Write(const std::string &name, const Vector2 &value)=0;
		virtual void Write(const std::string &name, const Color &value)=0;
		virtual void Write(const std::string &name, const bool &value)=0;

		virtual void Read(const std::string &name, int &value)=0;
		virtual void Read(const std::string &name, float &value)=0;
		virtual void Read(const std::string &name, std::string &value)=0;
		virtual void Read(const std::string &name, Vector2 &value)=0;
		virtual void Read(const std::string &name, Color &value)=0;
		virtual void Read(const std::string &name, bool &value)=0;

		virtual FileNode* FirstChildNode(const std::string &name)=0;
		virtual FileNode* NextChildNode(const std::string &name)=0;
		
		virtual void SaveChildNode(const std::string &name, Entity *entity)=0;
		//virtual FileNode* NewNode(const std::string &name)=0;
		//virtual FileNode* InsertEndChildNode(const std::string &name)=0;
	};
}
