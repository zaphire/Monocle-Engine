#include "XMLFileNode.h"
#include <iostream>
#include "../Vector2.h"
#include "../Color.h"
#include "tinyxml.h"

namespace Monocle
{
	XMLFileNode::XMLFileNode()
		: element(NULL)
	{

	}

	XMLFileNode::XMLFileNode(TiXmlElement *element)
		: element(element)
	{

	}

	/// WRITE
	void XMLFileNode::Write(const std::string &name, const int &value)
	{
		element->SetAttribute(name, value);
	}

	void XMLFileNode::Write(const std::string &name, const float &value)
	{
		std::ostringstream os;
		os << value;
		element->SetAttribute(name, os.str());
		//element->SetAttribute(name, value);
	}

	void XMLFileNode::Write(const std::string &name, const std::string &value)
	{
		element->SetAttribute(name, value);
	}

	void XMLFileNode::Write(const std::string &name, const Vector2 &value)
	{
		std::ostringstream os;
		os << value.x << " " << value.y;
		element->SetAttribute(name, os.str());
	}

	void XMLFileNode::Write(const std::string &name, const Color &value)
	{
		std::ostringstream os;
		os << value.r << " " << value.g << " " << value.b << " " << value.a;
		element->SetAttribute(name, os.str());
	}

	void XMLFileNode::Write(const std::string &name, const bool &value)
	{
		element->SetAttribute(name, (int)value);
	}


	/// READ
	void XMLFileNode::Read(const std::string &name, int &value)
	{
		if (element->Attribute(name))
		{
			std::istringstream read(*element->Attribute(name));
			read >> value;
		}
	}

	void XMLFileNode::Read(const std::string &name, float &value)
	{
		if (element->Attribute(name))
		{
			std::istringstream read(*element->Attribute(name));
			read >> value;
		}
	}

	void XMLFileNode::Read(const std::string &name, std::string &value)
	{
		if (element->Attribute(name))
		{
			value = *element->Attribute(name);
		}
	}

	void XMLFileNode::Read(const std::string &name, Vector2 &value)
	{
		if (element->Attribute(name))
		{
			std::istringstream read(*element->Attribute(name));
			read >> value.x >> value.y;
		}
	}

	void XMLFileNode::Read(const std::string &name, Color &value)
	{
		if (element->Attribute(name))
		{
			std::string string = *element->Attribute(name);
			std::istringstream read(string);
			if (string.find('#') != std::string::npos)
			{
				// TODO: read hex color
			}
			else
			{
				read >> value.r >> value.g >> value.b >> value.a;
			}
		}
	}

	void XMLFileNode::Read(const std::string &name, bool &value)
	{
		if (element->Attribute(name))
		{
			std::string string = *element->Attribute(name);
			std::istringstream read(string);
			read >> value;
		}
	}
}
