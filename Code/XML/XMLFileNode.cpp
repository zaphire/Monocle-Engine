#include "XMLFileNode.h"
#include <iostream>
#include "../Vector2.h"
#include "../Color.h"

namespace Monocle
{
	/// WRITE
	void XMLFileNode::Write(const std::string &name, const int &value)
	{
		element->SetAttribute(name, value);
	}

	void XMLFileNode::Write(const std::string &name, const float &value)
	{
		element->SetAttribute(name, value);
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
			std::istringstream read(*element->Attribute(name));
			read >> value.r >> value.g >> value.b >> value.a;
		}
	}

}
