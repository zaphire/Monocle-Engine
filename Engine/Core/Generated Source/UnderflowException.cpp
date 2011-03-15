/*
 * This file was automatically generated by AutoBind.
 * Any changes made to this file will be lost when it
 * is next regenerated.
 */

#include "autobind/types.h"
#include "autobind/binding/lua.h"
#include "Object.h"
#include "UnderflowException.h"

namespace Engine
{
    /* Variable assignments */
    const char* UnderflowException::Message = "A floating point underflow occurred within native code.";

    /* Method and constructor definitions */
    UnderflowException::UnderflowException()
    {
        }

	/// <summary>
	/// Exception constructor for Lua code.
	/// </summary>
    UnderflowException::UnderflowException(lua_State * L, bool byuser)
    {
        }

	/// <summary>
	/// Returns the static Message variable.  Required for downcasting.
	/// <summary>
    const char* UnderflowException::GetMessage()
    {
        return this->Message;
	}

	/// <summary>
	/// Returns the static ClassName variable.  Required for downcasting.
	/// <summary>
    const char* UnderflowException::GetName()
    {
        return this->ClassName;
	}

	/// <summary>
	/// A property to access the exception message.
	/// </summary>
	/// <readonly>true</readonly>
    int UnderflowException::GetMessageL(lua_State * L)
    {

        return Bindings<::string>::Result(L, Engine::Exception::GetParsedMessage(this->Message, this->Arguments).c_str());
	}

    /* Automatic dispatchers for overloaded methods */
    /* Automatic property getter-setter definitions */
    /* Binding variables */
    const char* UnderflowException::ClassName = "Engine.UnderflowException";
    const char* UnderflowException::Inherits = "Engine.LowLevelException";
    const Bindings<UnderflowException>::FunctionType UnderflowException::Functions[] =
    {
        {0}
    };
    const Bindings<UnderflowException>::PropertyType UnderflowException::Properties[] =
    {
        {"Message", &UnderflowException::GetMessageL, NULL},
        {0}
    };
    int (__cdecl *UnderflowException::Dispatcher)(lua_State * L) = &(Bindings<UnderflowException>::FunctionDispatch);
} 

