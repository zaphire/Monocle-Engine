// This file defines macros for handling low-level exceptions
// such as access volations and division by zero, which may
// occur inside external DLLs.  It makes extensive use of
// macros because handling such low-level exceptions is
// inherently non-portable.

#ifndef STATIC_LowLevel
#define STATIC_LowLevel

#include <lua.hpp>
#include "autobind/types.h"
#include "autobind/binding/lua.h"

// Include the files that are required to handle such
// exceptions under the build environment, or show a
// compiler error indicating that the compiler or host
// environment doesn't support them.
#ifdef WIN32
extern "C" unsigned long __cdecl _exception_code(void); // Predeclare
int __filter(unsigned int code);
typedef int __guardable(lua_State * L);
int __guard(__guardable function, lua_State * L);
int __guard_default(); // Used to bypass class constructor requirement for default throw.
template<class T>
	class __guarded
	{
	public:
		static int __guard(T * obj, int (T::*function)(lua_State * L), lua_State * L)
		{
			//__try
			//{
				return (obj->*function)(L);
			//}
			//__except (__filter(_exception_code()))
			//{
			//	return __guard_default();
			//}
		}

		static T * __guard(bool byuser, lua_State * L)
		{
			// Caution!  Memory access errors and other low-level
			// hardware exceptions can not be handled in object
			// constructors.
			return new T(L, byuser);
		}
	};
#else
#ifndef EXCEPTIONS_IGNORE_LOWLEVEL
#error Your compiler or host machine does not support catching
#error low-level exceptions such as access violations.  You can
#error ignore this warning by defining EXCEPTIONS_IGNORE_LOWLEVEL
#error as a preprocessor directive on the command line.
#else
int __filter(unsigned int code);
typedef int __guardable(lua_State * L);
inline int __guard(__guardable function, lua_State * L) { return function(L); }
template<class T>
	class __guarded
	{
	public:
		static int __guard(T * obj, int (T::*function)(lua_State * L), lua_State * L)
		{
			return (obj->*function)(L);
		}
	};
#endif
#endif

#endif