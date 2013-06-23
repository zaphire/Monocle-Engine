/*
 * This file was automatically generated by AutoBind.
 * Any changes made to this file will be lost when it
 * is next regenerated.
 */

#include "autobind/types.h"
#include "autobind/binding/lua.h"
#include "Object.h"
#include "ObjectDisposedException.h"

namespace Engine
{
    /* Variable assignments */
    const char* ObjectDisposedException::Message = "The object you are calling is no longer valid.";

    /* Method and constructor definitions */
    ObjectDisposedException::ObjectDisposedException()
    {
        }

	/// <summary>
	/// Exception constructor for Lua code.
	/// </summary>
    ObjectDisposedException::ObjectDisposedException(lua_State * L, bool byuser)
    {
        }

	/// <summary>
	/// Returns the static Message variable.  Required for downcasting.
	/// <summary>
    const char* ObjectDisposedException::GetMessage()
    {
        return this->Message;
	}

	/// <summary>
	/// Returns the static ClassName variable.  Required for downcasting.
	/// <summary>
    const char* ObjectDisposedException::GetName()
    {
        return this->ClassName;
	}

	/// <summary>
	/// A property to access the exception message.
	/// </summary>
	/// <readonly>true</readonly>
    int ObjectDisposedException::GetMessageL(lua_State * L)
    {

        return Bindings<::string>::Result(L, Engine::Exception::GetParsedMessage(this->Message, this->Arguments).c_str());
	}

    /* Automatic dispatchers for overloaded methods */
    /* Automatic property getter-setter definitions */
    /* Binding variables */
    const char* ObjectDisposedException::ClassName = "Engine.ObjectDisposedException";
    const char* ObjectDisposedException::Inherits = "Engine.EngineException";
    const Bindings<ObjectDisposedException>::FunctionType ObjectDisposedException::Functions[] =
    {
        {0}
    };
    const Bindings<ObjectDisposedException>::PropertyType ObjectDisposedException::Properties[] =
    {
        {"Message", &ObjectDisposedException::GetMessageL, NULL},
        {0}
    };
    int (__cdecl *ObjectDisposedException::Dispatcher)(lua_State * L) = &(Bindings<ObjectDisposedException>::FunctionDispatch);
} 
