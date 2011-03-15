// This file defines macros for handling low-level exceptions
// such as access volations and division by zero, which may
// occur inside external DLLs.  It makes extensive use of
// macros because handling such low-level exceptions is
// inherently non-portable.

#include "LowLevel.h"
#include "AccessViolationException.h"
#include "DivideByZeroException.h"
#include "OverflowException.h"
#include "UnderflowException.h"
#include "IllegalInstructionException.h"
#include "PrivilegedInstructionException.h"
#include "DataMisalignedException.h"

#ifdef WIN32
#include <windows.h> // for the lowlevel exception macros.
#include <excpt.h>
int __filter(unsigned int code)
{
	if (code == EXCEPTION_ACCESS_VIOLATION)
	{
		throw new Engine::AccessViolationException();
		return EXCEPTION_EXECUTE_HANDLER;
	}
	else if (code == STATUS_FLOAT_DIVIDE_BY_ZERO || code == STATUS_INTEGER_DIVIDE_BY_ZERO)
	{
		throw new Engine::DivideByZeroException();
		return EXCEPTION_EXECUTE_HANDLER;
	}
	else if (code == STATUS_FLOAT_OVERFLOW || code == STATUS_INTEGER_OVERFLOW)
	{
		throw new Engine::OverflowException();
		return EXCEPTION_EXECUTE_HANDLER;
	}
	else if (code == STATUS_FLOAT_UNDERFLOW)
	{
		throw new Engine::UnderflowException();
		return EXCEPTION_EXECUTE_HANDLER;
	}
	else if (code == STATUS_ILLEGAL_INSTRUCTION)
	{
		throw new Engine::IllegalInstructionException();
		return EXCEPTION_EXECUTE_HANDLER;
	}
	else if (code == STATUS_PRIVILEGED_INSTRUCTION)
	{
		throw new Engine::PrivilegedInstructionException();
		return EXCEPTION_EXECUTE_HANDLER;
	}
	else if (code == STATUS_DATATYPE_MISALIGNMENT)
	{
		throw new Engine::DataMisalignedException();
		return EXCEPTION_EXECUTE_HANDLER;
	}
	else
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}
}

int __guard(__guardable function, lua_State * L)
{
	//__try
	//{
		return function(L);
	//}
	//__except (__filter(GetExceptionCode()))
	//{
	//	return __guard_default();
	//}
}

int __guard_default()
{
	throw Engine::LowLevelException();
	return 0;
}

#endif