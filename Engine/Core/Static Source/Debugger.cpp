// This file provides a debugging interface for the engine
// to report errors and warnings back to.

#include "Debugger.h"
#include "LowLevel.h"
#include "Exception.h"
#include "DebuggerNotAttachedException.h"
#include "InterpreterException.h"
#include "InvalidObjectThrownException.h"
#include <iostream>
#include "Profiler.h"

namespace Monocle
{
	bool Debugger::m_IsConnected = false;

	void Debugger::RaiseException(Engine::Exception * err)
	{
		if (Debugger::m_IsConnected)
		{
			// TODO: Send the exception over the network socket to the IDE.
		}
		else
		{
			std::string msg = Engine::Exception::GetParsedMessage(err->GetMessage(), err->Arguments);
			for (unsigned int i = 60; i < msg.length(); i += 60)
			{
				// Find the previous space character.
				int oi = i;
				while (msg[oi] != ' ' && oi > 0) oi--;
				if (oi == 0 && msg[oi] != ' ')
				{
					oi = i;
					msg.insert(i, "\n           ");
				}
				else
				{
					i = oi;
					msg.insert(i, "\n          ");
				}
				i += 12;
			}

			std::cout << "An unhandled exception occurred within the game and there" << std::endl;
			std::cout << "was no debugger attached to handle the exception.  A detailed" << std::endl;
			std::cout << "error report about the exception that occurred can be found" << std::endl;
			std::cout << "below:" << std::endl;
			std::cout << std::endl;
			std::cout << "  == " << err->GetName() << " ==" << std::endl;
			std::cout << std::endl;
			std::cout << "  Message: " << msg << std::endl;
			std::cout << "  Source:  Line " << err->LineNumber << " of " << err->FileName << std::endl;
			std::cout << std::endl;
			std::cout << "  == Detailed Call Stack Trace ==" << std::endl;
			std::cout << std::endl;
			std::cout << "  --------------v----------------------------v-------------" << std::endl;
			std::cout << "  Location      | Function [Type]            | Line Number " << std::endl;
			std::cout << "  --------------+----------------------------+-------------" << std::endl;

			for (unsigned int i = 0; i < err->CallStackTrace.size(); i++)
			{
				std::string level = err->CallStackTrace[i].Level;
				std::string source = err->CallStackTrace[i].Source;
				std::string function = err->CallStackTrace[i].Function;
				std::string functype = err->CallStackTrace[i].FuncType;
				std::string linenumber = err->CallStackTrace[i].LineNumber;

				if (functype != "")
				{
					functype.insert(0, "[");
					functype.insert(functype.length(), "]");
				}

				// Ensure strings are of the correct size.
				while (level.length() < 3) level += " ";
				while (source.length() < 8) source += " ";
				while (function.length() + 1 + functype.length() < 26) functype += " ";

				std::cout << "  " << level << ": " << source << " | " <<
					function << " " << functype << " | " <<
					linenumber << std::endl;
			}

			std::cout << "  --------------^----------------------------^-------------" << std::endl;
			std::cout << std::endl;
			std::cout << "  == Detailed Data Stack Trace ==" << std::endl;
			std::cout << std::endl;
			std::cout << "  ---------v------------v----------------------------------" << std::endl;
			std::cout << "  Position | Base Type  | String Representation            " << std::endl;
			std::cout << "  ---------+------------+----------------------------------" << std::endl;

			for (unsigned int i = 0; i < err->DataStackTrace.size(); i++)
			{
				std::string level = err->DataStackTrace[i].Level;
				std::string basetype = err->DataStackTrace[i].BaseType;
				std::string stringrepr = err->DataStackTrace[i].StringRepr;

				// Ensure strings are of the correct size.
				while (level.length() < 8) level += " ";
				while (basetype.length() < 10) basetype += " ";

				std::cout << "  " << level << " | " << basetype << " | " <<
					stringrepr << std::endl;
			}

			std::cout << "  ---------^------------^----------------------------------" << std::endl;
			std::cout << std::endl;
			std::cout << "Contact the game author with a copy of this message for" << std::endl;
			std::cout << "further assistance." << std::endl;
			throw new Engine::DebuggerNotAttachedException();
		}
	}

	int Debugger::LuaExceptionHandle(lua_State * L)
	{
		// The error message is on top of the stack.
		// This is called when a lua_pcall() operation
		// fails.  We need to check whether the error
		// is a string or an Exception object just like
		// in LuaPanicHandle.
		if (lua_isstring(L, -1))
		{
			// It's a general Lua error.
			Debugger::RaiseException(new Engine::InterpreterException(lua_tostring(L, -1)));
			return 0;
		}
		else
		{
			// Fetch the exception from the stack.  Check
			// to see whether the exception is of Engine::Exception
			// (i.e. we can safely pass it to Debugger::RaiseException,
			// or whether it is of another type, in which case
			// we should throw Engine::InvalidObjectThrownException
			// to indicate that we can't directly get the meaning
			// of the exception.

			// First push the type onto the stack.
			Bindings<Engine::Exception>::PushType(L);

			// Now repush the error object on top.
			lua_pushvalue(L, -2);   // TODO: This value is not valid when errors are raised by OP_RAISE

			// Check to see if it inherits from Engine::Exception.
			if (lua_is(L))
			{
				// Push the exception directly to Debugger::RaiseException.
				Engine::Exception * err = Bindings<Engine::Exception>::GetArgument(L, -1);
				Debugger::RaiseException(err);
			}
			else
			{
				// We can't read the exception meaning directly, so throw
				// a Engine::InvalidObjectThrownException to inform the user
				// that it's the case.
				Debugger::RaiseException(new Engine::InvalidObjectThrownException());
			}
			
			return 0;
		}
	}

	int Debugger::LuaPCALLHandle(lua_State * L)
	{
		// The error message is on top of the stack, so we
		// check to see whether it's a string (Lua error)
		// or an Exception object.  If it's the former,
		// we use Debugger::RaiseException with a
		// GeneralLuaException.  If it's the latter, we
		// pass it as the parameter for the
		// Debugger::RaiseException call.
		if (lua_isstring(L, -1))
		{
			// It's a general Lua error.
			try
			{
				Debugger::RaiseException(new Engine::InterpreterException(lua_tostring(L, -1)));
			}
			catch (Engine::DebuggerNotAttachedException * err)
			{
				// We don't want to propagate the DebuggerNotAttachedException otherwise
				// we'll get Debugger::RaiseException called twice.
			}
			return 0;
		}
		else
		{
			// Fetch the exception from the stack.  Check
			// to see whether the exception is of Engine::Exception
			// (i.e. we can safely pass it to Debugger::RaiseException,
			// or whether it is of another type, in which case
			// we should throw Engine::InvalidObjectThrownException
			// to indicate that we can't directly get the meaning
			// of the exception.

			// First push the type onto the stack.
			Bindings<Engine::Exception>::PushType(L);

			// Now repush the error object on top.
			lua_pushvalue(L, -2);

			// Check to see if it inherits from Engine::Exception.
			try
			{
				if (lua_is(L))
				{
					// Push the exception directly to Debugger::RaiseException.
					Engine::Exception * err = Bindings<Engine::Exception>::GetArgument(L, -1);
					Debugger::RaiseException(err);
				}
				else
				{
					// We can't read the exception meaning directly, so throw
					// a Engine::InvalidObjectThrownException to inform the user
					// that it's the case.
					Debugger::RaiseException(new Engine::InvalidObjectThrownException());
				}
			}
			catch (Engine::DebuggerNotAttachedException * err)
			{
				// We don't want to propagate the DebuggerNotAttachedException otherwise
				// we'll get Debugger::RaiseException called twice.
			}
			
			return 0;
		}
	}

	void Debugger::LuaHookInitalize(lua_State * L)
	{
		lua_sethook(L, &Debugger::LuaHookOnEvent, LUA_MASKCALL | LUA_MASKRET | LUA_MASKLINE, 0);
	}

	void Debugger::LuaHookOnEvent(lua_State * L, lua_Debug * D)
	{
		Profiler::LuaHookOnEvent(L, D);

		if (Debugger::m_IsConnected)
		{
			// TODO: Send the debugging information back to the IDE over the socket.
			//       The IDE will send back data based on what the engine should do
			//       (i.e. pause at a breakpoint).
		}
	}
}