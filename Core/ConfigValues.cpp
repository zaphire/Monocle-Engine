#include "ConfigValues.h"

namespace Monocle
{
	ConfigValues *ConfigValues::instance = NULL;

	ConfigValues::ConfigValues()
	{
		instance = this;
	}

	void ConfigValues::Load(const std::string &filename)
	{
		TiXmlDocument doc;
		doc.LoadFile(Assets::GetContentPath() + filename);
		TiXmlElement *xmlValues = doc.FirstChildElement("config"); 
		if (xmlValues)
		{
			TiXmlElement *xmlValue = xmlValues->FirstChildElement("value");
			while (xmlValue)
			{
				std::string name = XMLReadString(xmlValue, "name");

				if (xmlValue->Attribute("float"))
					instance->floats[name] = XMLReadFloat(xmlValue, "float");

				if (xmlValue->Attribute("string"))
					instance->strings[name] = XMLReadString(xmlValue, "string");

				if (xmlValue->Attribute("int"))
					instance->ints[name] = XMLReadInt(xmlValue, "int");

				if (xmlValue->Attribute("bool"))
					instance->bools[name] = XMLReadBool(xmlValue, "bool");

				if (xmlValue->Attribute("vector2"))
					instance->vector2s[name] = XMLReadVector2(xmlValue, "vector2");

				xmlValue = xmlValue->NextSiblingElement("value");
			}
		}
	}

	std::string ConfigValues::GetString(const std::string &valueName)
	{
		return instance->strings[valueName];
	}

	int ConfigValues::GetInt(const std::string &valueName)
	{
		return instance->ints[valueName];
	}

	float ConfigValues::GetFloat(const std::string &valueName)
	{
		return instance->floats[valueName];
	}

	bool ConfigValues::GetBool(const std::string &valueName)
	{
		return instance->bools[valueName];
	}

	Vector2 ConfigValues::GetVector2(const std::string &valueName)
	{
		return instance->vector2s[valueName];
	}
}