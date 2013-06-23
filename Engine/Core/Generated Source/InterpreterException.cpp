/*
 * This file was automatically generated by AutoBind.
 * Any changes made to this file will be lost when it
 * is next regenerated.
 */

#include "autobind/types.h"
#include "autobind/binding/lua.h"
#include "Object.h"
#include "InterpreterException.h"

namespace Engine
{
    /* Variable assignments */
    const char* InterpreterException::Message = "A general exception was raised by the Lua interpreter.  The error message is '${1}'.";

    /* Method and constructor definitions */
    InterpreterException::InterpreterException()
    {
        }

	/// <summary>
	/// Exception constructor for C++ code.
	/// </summay>
    InterpreterException::InterpreterException(const char* err)
    {
        this->Arguments.insert(this->Arguments.end(), err);
	}

	/// <summary>
	/// Exception constructor for Lua code.
	/// </summary>
    InterpreterException::InterpreterException(lua_State * L, bool byuser)
    {
        }

	/// <summary>
	/// Returns the static Message variable.  Required for downcasting.
	/// <summary>
    const char* InterpreterException::GetMessage()
    {
        return this->Message;
	}

	/// <summary>
	/// Returns the static ClassName variable.  Required for downcasting.
	/// <summary>
    const char* InterpreterException::GetName()
    {
        return this->ClassName;
	}

	/// <summary>
	/// A property to access the exception message.
	/// </summary>
    int InterpreterException::GetMessageL(lua_State * L)
    {

        return Bindings<::string>::Result(L, Engine::Exception::GetParsedMessage(this->Message, this->Arguments).c_str());
	}

    int InterpreterException::SetMessageL(lua_State * L)
    {
        ::string value = Bindings<::string>::GetArgumentBase(L, -1);

        throw new Engine::ReadOnlyPropertyException();
	}

    /* Automatic dispatchers for overloaded methods */
    /* Automatic property getter-setter definitions */
    /* Binding variables */
    const char* InterpreterException::ClassName = "Engine.InterpreterException";
    const char* InterpreterException::Inherits = "Engine.Exception";
    const Bindings<InterpreterException>::FunctionType InterpreterException::Functions[] =
    {
        {0}
    };
    const Bindings<InterpreterException>::PropertyType InterpreterException::Properties[] =
    {
        {"Message", &InterpreterException::GetMessageL, &InterpreterException::SetMessageL},
        {0}
    };
    int (__cdecl *InterpreterException::Dispatcher)(lua_State * L) = &(Bindings<InterpreterException>::FunctionDispatch);
} 
