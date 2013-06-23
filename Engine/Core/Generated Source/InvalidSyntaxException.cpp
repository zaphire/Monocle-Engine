/*
 * This file was automatically generated by AutoBind.
 * Any changes made to this file will be lost when it
 * is next regenerated.
 */

#include "autobind/types.h"
#include "autobind/binding/lua.h"
#include "Object.h"
#include "InvalidSyntaxException.h"

namespace Engine
{
    /* Variable assignments */
    const char* InvalidSyntaxException::Message = "The engine attempted to load a file which contains one or more syntax errors.  '${1}'";

    /* Method and constructor definitions */
    InvalidSyntaxException::InvalidSyntaxException(std::string msg)
    {
        this->Arguments.insert(this->Arguments.end(), msg);
	}

	/// <summary>
	/// Exception constructor for Lua code.
	/// </summary>
    InvalidSyntaxException::InvalidSyntaxException(lua_State * L, bool byuser)
    {
        ::string msg = Bindings<::string>::GetArgumentBase(L, 1);

        this->Arguments.insert(this->Arguments.end(), msg);
	}

	/// <summary>
	/// Returns the static Message variable.  Required for downcasting.
	/// <summary>
    const char* InvalidSyntaxException::GetMessage()
    {
        return this->Message;
	}

	/// <summary>
	/// Returns the static ClassName variable.  Required for downcasting.
	/// <summary>
    const char* InvalidSyntaxException::GetName()
    {
        return this->ClassName;
	}

	/// <summary>
	/// A property to access the exception message.
	/// </summary>
	/// <readonly>true</readonly>
    int InvalidSyntaxException::GetMessageL(lua_State * L)
    {

        return Bindings<::string>::Result(L, Engine::Exception::GetParsedMessage(this->Message, this->Arguments).c_str());
	}

    /* Automatic dispatchers for overloaded methods */
    /* Automatic property getter-setter definitions */
    /* Binding variables */
    const char* InvalidSyntaxException::ClassName = "Engine.InvalidSyntaxException";
    const char* InvalidSyntaxException::Inherits = "Engine.InterpreterException";
    const Bindings<InvalidSyntaxException>::FunctionType InvalidSyntaxException::Functions[] =
    {
        {0}
    };
    const Bindings<InvalidSyntaxException>::PropertyType InvalidSyntaxException::Properties[] =
    {
        {"Message", &InvalidSyntaxException::GetMessageL, NULL},
        {0}
    };
    int (__cdecl *InvalidSyntaxException::Dispatcher)(lua_State * L) = &(Bindings<InvalidSyntaxException>::FunctionDispatch);
} 
