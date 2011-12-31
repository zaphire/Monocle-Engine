#pragma once

#include <map>
#include <string>
#include "Vector2.h"
#include "Monocle.h"

namespace Monocle
{
	//! \brief A quick way to load in various datatypes from an XML config file.
	//! Config file should be XML of format:
	//! <config> <value name="variable_name" float="10.5"/> </config>
	//! Supports: int/float/string/bool/vector2
	//! Stolen from Alec Holowka's code
	class ConfigValues
	{
	public:
		ConfigValues();
		static void Load(const std::string &filename);
		static std::string GetString(const std::string &valueName);
		static int GetInt(const std::string &valueName);
		static float GetFloat(const std::string &valueName);
		static bool GetBool(const std::string &valueName);
		static Vector2 GetVector2(const std::string &valueName);

	private:
		static ConfigValues *instance;

		std::map<std::string, int>			ints;
		std::map<std::string, float>		floats;
		std::map<std::string, std::string>	strings;
		std::map<std::string, bool>			bools;
		std::map<std::string, Vector2>		vector2s;
	};

}