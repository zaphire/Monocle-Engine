// This file provides a struct for storing information
// about a call stack.

#ifndef STATIC_StackPoint
#define STATIC_StackPoint

#include <string>

namespace Monocle
{
	struct CallStackPoint
	{
		std::string Level;
		std::string Source;
		std::string Function;
		std::string FuncType;
		std::string LineNumber;
	};

	struct DataStackPoint
	{
		std::string Level;
		std::string BaseType;
		std::string StringRepr;
	};
}

#endif