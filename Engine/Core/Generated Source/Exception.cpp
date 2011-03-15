/*
 * This file was automatically generated by AutoBind.
 * Any changes made to this file will be lost when it
 * is next regenerated.
 */

#include "autobind/types.h"
#include "autobind/binding/lua.h"
#include "Object.h"
#include "Exception.h"
#include <string>
#include <sstream>
#include <vector>
#include "Utility.h"
#include "Program.h"
#include "StackPoint.h"

namespace Engine
{
    /* Variable assignments */
    const char* Exception::Message = "A general exception was raised.";

    /* Method and constructor definitions */
    Exception::Exception()
    {
        if (Monocle::Program::GlobalState != NULL)
			this->GatherEnvironmentData(Monocle::Program::GlobalState);
		else
		{
			this->LineNumber = 0;
			this->FileName = "<outside lua>";
		}
	}

	/// <summary>
	/// Exception constructor for Lua code.
	/// </summary>
    Exception::Exception(lua_State * L, bool byuser)
    {
        this->GatherEnvironmentData(L);
	}

	/// <summary>
	/// A function to gather information about the current environment
	/// in which the exception was thrown.
	/// </summary>
    void Exception::GatherEnvironmentData(lua_State * L)
    {
        // Get information about the data on the Lua stack.
		for (int i = 1; i <= lua_gettop(L); i += 1)
		{
			Monocle::DataStackPoint p;
			std::stringstream a;
			
			a << i;
			p.Level = a.str().c_str();
			p.BaseType = lua_typename(L, lua_type(L, i));
			if (lua_type(L, i) == LUA_TTABLE)
			{
				std::stringstream s;
				s << lua_advtypename(L, i) << ": " << (int)lua_topointer(L, i);
				p.StringRepr = s.str().c_str();
			}
			else if (lua_type(L, i) == LUA_TSTRING)
				p.StringRepr = lua_tostring(L, i);
			else if (lua_type(L, i) == LUA_TNUMBER)
				p.StringRepr = lua_tostring(L, i);
			else if (lua_type(L, i) == LUA_TBOOLEAN)
				p.StringRepr = lua_toboolean(L, i) ? "true" : "false";
			else if (lua_type(L, i) == LUA_TNIL)
				p.StringRepr = "";
			else
			{
				std::stringstream s;
				s << p.BaseType << ": " << (int)lua_topointer(L, i);
				p.StringRepr = s.str().c_str();
			}

			this->DataStackTrace.insert(this->DataStackTrace.end(), p);
		}
		
		// Get information about the call stack.
		lua_Debug ar;
		int ret;
		int i = 0;
		bool hasdefinedline = false;
		while ((ret = lua_getstack(L, i, &ar)) == 1)
		{
			lua_getinfo(L, "S", &ar);
			Monocle::CallStackPoint p;
			std::stringstream a;
			std::stringstream b;

			if (strcmp(ar.what, "Lua") == 0)
			{
				lua_getinfo(L, "ln", &ar);

				a << (i + 1);
				b << ar.currentline;
				p.Level = a.str().c_str();
				p.Source = ar.short_src;
				if (ar.name != NULL)
				{
					p.Function = ar.name;
					p.FuncType = ar.namewhat;
				}
				else
				{
					p.Function = "???";
					p.FuncType = "Lua";
				}
				p.LineNumber = b.str().c_str();

				if (!hasdefinedline)
				{
					this->LineNumber = ar.linedefined;
					this->FileName = ar.short_src;
					hasdefinedline = true;
				}
			}
			else if (strcmp(ar.what, "C") == 0)
			{
				lua_getinfo(L, "n", &ar);

				a << (i + 1);
				p.Level = a.str().c_str();
				p.Source = "<native>";
				if (ar.name != NULL)
				{
					p.Function = ar.name;
					p.FuncType = ar.namewhat;
				}
				else
				{
					p.Function = "???";
					p.FuncType = "C";
				}
				p.LineNumber = "???";
			}
			else if (strcmp(ar.what, "main") == 0)
			{
				lua_getinfo(L, "l", &ar);

				a << (i + 1);
				b << ar.currentline;
				p.Level = a.str().c_str();
				p.Source = "<main>";
				p.Function = ar.short_src;
				p.FuncType = "";
				p.LineNumber = b.str().c_str();
			}
			else if (strcmp(ar.what, "tail") == 0)
			{
				a << (i + 1);
				p.Level = a.str().c_str();
				p.Source = "<tail>";
				p.Function = "";
				p.FuncType = "";
				p.LineNumber = "";
			}
			else
			{
				i++;
				continue;
			}

			this->CallStackTrace.insert(this->CallStackTrace.end(), p);

			i++;
		}
	}

	/// <summary>
	/// A function which parses the static Message variable according
	/// to the arguments in the Arguments vector.
	/// <summary>
    std::string Exception::GetParsedMessage(std::string msg, std::vector<std::string> args)
    {
        std::string ret = msg;
		for (unsigned int i = 0; i < args.size(); i++)
		{
			std::stringstream from;
			from << "${" << (i + 1) << "}";
			ret = Monocle::Utility::ReplaceAll(ret, from.str(), args[i]);
		}
		return ret;
	}

	/// <summary>
	/// Returns the static Message variable.  Required for downcasting.
	/// <summary>
    const char* Exception::GetMessage()
    {
        return this->Message;
	}

	/// <summary>
	/// Returns the static ClassName variable.  Required for downcasting.
	/// <summary>
    const char* Exception::GetName()
    {
        return this->ClassName;
	}

	/// <summary>
	/// A property to access the exception message.
	/// </summary>
	/// <readonly>true</readonly>
    int Exception::GetMessageL(lua_State * L)
    {

        return Bindings<::string>::Result(L, Engine::Exception::GetParsedMessage(this->Message, this->Arguments).c_str());
	}

    /* Automatic dispatchers for overloaded methods */
    /* Automatic property getter-setter definitions */
    /* Binding variables */
    const char* Exception::ClassName = "Engine.Exception";
    const char* Exception::Inherits = "Object";
    const Bindings<Exception>::FunctionType Exception::Functions[] =
    {
        {0}
    };
    const Bindings<Exception>::PropertyType Exception::Properties[] =
    {
        {"Message", &Exception::GetMessageL, NULL},
        {0}
    };
    int (__cdecl *Exception::Dispatcher)(lua_State * L) = &(Bindings<Exception>::FunctionDispatch);
} 

