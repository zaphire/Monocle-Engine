/*
 * This header file is part of the AutoBind
 * toolset.  It defines the Bindings<> template
 * for use within the LuaTranslator code.
 */

#ifndef AUTOBIND_lua
#define AUTOBIND_lua

#include <string>
#include <vector>
#include <lua.hpp>
#include "LowLevel.h"

void *luaL_testudata (lua_State *L, int ud, const char *tname);
void lua_gettablevalue (lua_State * luaVM, int tableindex, int valueindex);

namespace Engine { class Exception; }

template<class T>
	class Bindings
	{
		// Used to store the object within the userdata
		// field in Lua.
		typedef union
		{
			T *pT;
		} UserDataType;

	public:
		// Defines an empty return result.
		static const int EmptyResult = 0;

		// Defines the structure of the PropertyTypes list
		// (used for telling the bindings how to handle
		// properties in a given object).
		struct PropertyType
		{
			const char* Name;
			int (T::*Getter)(lua_State * L);
			int (T::*Setter)(lua_State * L);
		};

		// Defines the structre of the FunctionTypes list
		// (used for telling the bindings which functions
		// are bound to Lua).
		struct FunctionType
		{
			const char* Name;
			int (T::*Function)(lua_State * L);
		};

		// A static function for retrieving an object from
		// the arguments provided to a function.  It is also
		// used during property setting, where arg should be
		// -1 to get the value that was assigned.
		static T* GetArgument(lua_State * L, int narg)
		{
			// Convert to absolute reference.
			if (narg < 0) narg = lua_gettop(L) + narg + 1;

			// Check to make sure that it's a table at that
			// position.  If it isn't, then it can't be a
			// class.
			if (lua_istable(L, narg))
			{
				// Call lua_is to check to see whether the class is
				// of the correct type (or of a derived type).  We
				// push the type to compare against and then we have
				// to repush the argument so that it's on top of the
				// stack for lua_is.
				Bindings<T>::PushType(L);
				lua_pushvalue(L, narg);

				// Check to make sure that it is of the correct type.
				if (!lua_is(L))
				{
					// Throw an exception indicating that it is not of the
					// correct type.
					throw new Engine::ArgumentTypeNotValidException(narg,
						lua_advtypename(L, narg), T::ClassName);
				}

				// Cleanup the stack after the type check (pop the copy of
				// the userdata and the type to compare against).
				lua_pop(L, 2);

				// Now we get the value at the first index of the table,
				// which is the userdata associated with the Lua object.
				lua_gettablevalue(L, narg, 0);

				// Make sure it is a userdata type.
				if (!lua_isuserdata(L, -1))
				{
					// Throw an exception indicating that it is not of the
					// correct type.
					throw new Engine::ArgumentTypeNotValidException(narg,
						lua_advtypename(L, narg), T::ClassName);
				}

				// At this point we know that the userdata is of the correct
				// type because we've previously checked with lua_is. Convert
				// the value that was just pushed onto the stack into userdata
				// if possible.
				UserDataType* ud = static_cast<UserDataType*>(lua_touserdata(L, -1));
				if (ud == NULL)
				{
					// Throw an exception indicating that it is not of the
					// correct type.
					throw new Engine::ArgumentTypeNotValidException(narg,
						lua_advtypename(L, narg), T::ClassName);
				}

				// Pop the value from the stack as we no longer need it.
				lua_pop(L, 1);

				// Return a pointer to the object in the specified argument
				// position.
				return ud->pT;
			}
			else
			{
				// Throw an exception indicating that it is not of the
				// correct type.
				throw new Engine::ArgumentTypeNotValidException(narg,
					lua_advtypename(L, narg), T::ClassName);
			}
		}
		
		// Same as the previous function but accepts a default
		// parameter in case the number of arguments passed
		// is less than narg.  Still triggers type errors
		// if the wrong type is in narg.  Comments are
		// omitted because they are the same as the GetArgument
		// function above.
		static T* GetArgument(lua_State * L, int narg, T * def)
		{
			if (narg < 0) narg = lua_gettop(L) + narg + 1;

			if (narg > lua_gettop(L) || lua_isnil(L, narg)) return def;

			if (lua_istable(L, narg))
			{
				Bindings<T>::PushType(L);
				lua_pushvalue(L, narg);

				if (!lua_is(L))
				{
					throw new Engine::ArgumentTypeNotValidException(narg,
						lua_advtypename(L, narg), T::ClassName);
				}

				lua_pop(L, 2);
				lua_gettablevalue(L, narg, 0);

				if (!lua_isuserdata(L, -1))
				{
					throw new Engine::ArgumentTypeNotValidException(narg,
						lua_advtypename(L, narg), T::ClassName);
				}

				UserDataType* ud = static_cast<UserDataType*>(lua_touserdata(L, -1));
				if (ud == NULL)
				{
					throw new Engine::ArgumentTypeNotValidException(narg,
						lua_advtypename(L, narg), T::ClassName);
				}

				lua_pop(L, 1);
				return ud->pT;
			}
			else
			{
				throw new Engine::ArgumentTypeNotValidException(narg,
					lua_advtypename(L, narg), T::ClassName);
			}
		}

		// A static function for for detecting whether or not an
		// argument provided to a function is a specific object.
		static bool IsArgument(lua_State * L, int narg)
		{
			// Convert to absolute reference.
			if (narg < 0) narg = lua_gettop(L) + narg + 1;

			// Check to make sure that it's a table at that
			// position.  If it isn't, then it can't be a
			// class.
			if (lua_istable(L, narg))
			{
				// Get the value at the first index of the table, and
				// push it onto the stack.
				lua_gettablevalue(L, narg, 0);

				// Convert the value that was just pushed onto the
				// stack into userdata if possible.  The luaL_testudata
				// checks to make sure that the class is the correct
				// type before converting it.
				UserDataType* ud = static_cast<UserDataType*>(luaL_testudata(L, -1, T::ClassName));
				if (ud == NULL)
				{
					// Return false to indicate that the argument is not
					// of the specified type.
					return false;
				}

				// Pop the value from the stack as we no longer need it.
				lua_pop(L, 1);

				// Return true since it is of the correct type.
				return true;
			}
			else
			{
				// Return false to indicate that the argument could not
				// be handled by the Bindings<> class.
				return false;
			}
		}

		// A static function for retrieving an base object (i.e.
		// numeric or string value) from the arguments provided
		// to a function.  It is also used during property setting,
		// where arg should be -1 to get the value that was assigned.
		static T GetArgumentBase(lua_State * L, int narg);

		// Same as the previous function, but also accepts a default
		// argument in case the number of arguments is less than
		// narg (still triggers type errors if an invalid argument
		// is passed in that slot though).
		static T GetArgumentBase(lua_State * L, int narg, T def);

		// A static function for detecting whether or not an
		// argument provided to a function is a specific base object
		// (i.e. numeric or string value).
		static bool IsArgumentBase(lua_State * L, int narg);

		// Registers the given class with the Lua engine.
		static void Register(lua_State * L)
		{
			// We use the std::string class to parse the
			// T::ClassName variable.  T::ClassName not only
			// contains the class name of the class, but also
			// the namespace context as well.
			std::string cls = T::ClassName;
			std::string buf = "";
			std::vector<std::string> elms;
			for (unsigned int i = 0; i < cls.length(); i++)
			{
				if (cls[i] == '.')
				{
					if (buf.length() > 0)
					{
						elms.insert(elms.end(), buf);
						buf = "";
					}
				}
				else
					buf += cls[i];
			}
			if (buf.length() > 0)
			{
				elms.insert(elms.end(), buf);
				buf = "";
			}

			// Make sure there is enough stack space to perform
			// all of the namespace loading, etc..
			lua_checkstack(L, elms.size() + 5);

			// Now loop through elms, loading all but the last element
			// as namespace tables.
			int topop = 0;
			for (unsigned int i = 0; i < elms.size(); i++)
			{
				if (i < elms.size() - 1)
				{
					if (i == 0)
					{
						// Fetch the first from the global area.
						lua_getglobal(L, elms[i].c_str());
						if (lua_isnil(L, -1))
						{
							// The namespace does not yet exist, create it.
							lua_pop(L, 1);
							lua_newtable(L);
							lua_setglobal(L, elms[i].c_str());
							lua_getglobal(L, elms[i].c_str());
						}
						topop++;
					}
					else
					{
						lua_getfield(L, -1, elms[i].c_str());
						if (lua_isnil(L, -1))
						{
							// The namespace does not yet exist, create it.
							lua_pop(L, 1);
							lua_newtable(L);
							lua_setfield(L, -2, elms[i].c_str());
							lua_getfield(L, -1, elms[i].c_str());
						}
						topop++;
					}
				}
				else
				{
					if (i == 0)
					{
						// Our constructors are actually functables
						// in Lua; tables with __call.  This allows us
						// to provide a __type metamethod so that the
						// user can do 'var is NativeObject'.

						// Create a table and a metatable.
						lua_newtable(L);
						lua_newtable(L);
						int mt = lua_gettop(L);

						// Push the __call (constructor) and __type
						// metamethods onto the metatable.
						lua_pushstring(L, "__call");
						lua_pushcfunction(L, &Bindings<T>::Constructor);
						lua_rawset(L, mt);

						lua_pushstring(L, "__type");
						Bindings<T>::Type(L);
						lua_rawset(L, mt);

						// Set the table's metatable to mt.  The table
						// is directly below the metatable.  lua_setmetatable
						// pops the metatable off the stack, so the table
						// is on top and ready for lua_setglobal.
						lua_setmetatable(L, mt - 1);

						// Set directly into the global namespace.
						lua_setglobal(L, elms[i].c_str());
					}
					else
					{
						// Our constructors are actually functables
						// in Lua; tables with __call.  This allows us
						// to provide a __type metamethod so that the
						// user can do 'var is NativeObject'.

						// See the main 'if' for explainations of what
						// is happening here.
						lua_newtable(L);
						lua_newtable(L);
						int mt = lua_gettop(L);

						lua_pushstring(L, "__call");
						lua_pushcfunction(L, &Bindings<T>::Constructor);
						lua_rawset(L, mt);

						lua_pushstring(L, "__type");
						Bindings<T>::Type(L);
						lua_rawset(L, mt);

						lua_setmetatable(L, mt - 1);

						// Set into the namespace table.
						lua_setfield(L, -2, elms[i].c_str());
					}
				}
			}
			lua_pop(L, topop);

			// Create a new metatable, used for storing the userdata
			// associated with our class registration.
			luaL_newmetatable(L, T::ClassName);
			int metatable = lua_gettop(L);

			// Add our metatable functions such as __gc, __index,
			// __setindex and __type.
			lua_pushstring(L, "__gc");
			lua_pushcfunction(L, &Bindings<T>::GCObj);
			lua_rawset(L, metatable);

			lua_pushstring(L, "__index");
			lua_pushcfunction(L, &Bindings<T>::PropertyGetter);
			lua_rawset(L, metatable);

			lua_pushstring(L, "__setindex");
			lua_pushcfunction(L, &Bindings<T>::PropertySetter);
			lua_rawset(L, metatable);

			// Execute our ::Type function to get our type
			// data onto the top of the stack, then push it
			// into __type.
			lua_pushstring(L, "__type");
			Bindings<T>::Type(L);
			lua_rawset(L, metatable);
		}

		// The callback function used when a class is constructed.
		static int Constructor(lua_State * L)
		{
			// Create a new table with which to return
			// our new Lua object.
			lua_newtable(L);

			// Push the index of the userdata.
			lua_pushnumber(L, 0);

			// Essentially here we're setting the userdata in
			// the metatable to the Bindings<T> object.  This
			// allows the function callbacks and property setter
			// and getter functions to know what C++ object they
			// are dealing with.
			T** ud = (T**)lua_newuserdata(L, sizeof(T*));

			// We have to make sure that the stack only contains
			// the arguments passed to the function, so that
			// lua_gettop() returns the number of arguments.
			// Therefore, we have to temporarily store the new
			// table and userdata in the C registry.
			lua_setfield(L, LUA_REGISTRYINDEX, "Roket3D_Class_Construction_Temporary_Userdata");
			lua_pop(L, 1);
			lua_setfield(L, LUA_REGISTRYINDEX, "Roket3D_Class_Construction_Temporary_Table");

			// There's a table at the start of the stack (unsure
			// at this time what it holds).  Remove it so that
			// the arguments are the only things on the stack
			// when the constructor is called.
			lua_remove(L, 1);

			// Now call the class constructor.
			T* obj = new T(L, true);
			obj->Grab();
			*ud = obj;

			// Clear the entire stack, and place the userdata,
			// an index of 0 and new table onto the stack in
			// their place.
			lua_pop(L, lua_gettop(L));
			lua_getfield(L, LUA_REGISTRYINDEX, "Roket3D_Class_Construction_Temporary_Table");
			lua_pushnil(L);
			lua_setfield(L, LUA_REGISTRYINDEX, "Roket3D_Class_Construction_Temporary_Table");
			int newtable = lua_gettop(L);
			lua_pushnumber(L, 0); // Index which SetupObject expects to have.
			lua_getfield(L, LUA_REGISTRYINDEX, "Roket3D_Class_Construction_Temporary_Userdata");
			lua_pushnil(L);
			lua_setfield(L, LUA_REGISTRYINDEX, "Roket3D_Class_Construction_Temporary_Userdata");
			int userdata = lua_gettop(L);

			// Call the SetupObject() function which sets up the
			// properties and functions on the new object and
			// pushes it onto the stack.  We use this function so
			// that we don't repeat code between Constructor, CreateNew
			// and CreateFromExisting.
			Bindings<T>::SetupObject(L, newtable, userdata);

			// Return a value of 1, indicating that we're returning
			// a new object.
			return 1;
		}

		// Creates a new instance of the C++ object, like the Constructor
		// callback, but returns the object itself for use in C++ code
		// (unlike Constructor which is intended to be used from Lua).
		static T* CreateNew(lua_State * L)
		{
			// Create a new table with which to return
			// our new Lua object.
			lua_newtable(L);

			// Get the address of the new table on the
			// stack.
			int newtable = lua_gettop(L);

			// Push the index of the userdata.
			lua_pushnumber(L, 0);

			// Essentially here we're setting the userdata in
			// the metatable to the Bindings<T> object.  This
			// allows the function callbacks and property setter
			// and getter functions to know what C++ object they
			// are dealing with.
			T** ud = (T**)lua_newuserdata(L, sizeof(T*));
			T* obj = new T(L, false);
			obj->Grab();
			*ud = obj;

			// Get the address of the userdata on the
			// stack.
			int userdata = lua_gettop(L);

			// Call the SetupObject() function which sets up the
			// properties and functions on the new object and
			// pushes it onto the stack.  We use this function so
			// that we don't repeat code between Constructor, CreateNew
			// and CreateFromExisting.
			Bindings<T>::SetupObject(L, newtable, userdata);

			// Instead of returning the number of objects we pushed
			// onto the stack, we return the actual object itself since
			// CreateNew will be used from C++ code.
			return obj;
		}

		// Creates a new instance of the Lua object, which is wrapping
		// an existing C++ object.  Returns the object itself for use
		// in C++ code (which will be the same as the 'existing' argument).
		static T* CreateFromExisting(lua_State * L, T* existing)
		{
			// Create a new table with which to return
			// our new Lua object.
			lua_newtable(L);

			// Get the address of the new table on the
			// stack.
			int newtable = lua_gettop(L);

			// Push the index of the userdata.
			lua_pushnumber(L, 0);

			// Essentially here we're setting the userdata in
			// the metatable to the Bindings<T> object.  This
			// allows the function callbacks and property setter
			// and getter functions to know what C++ object they
			// are dealing with.
			T** ud = (T**)lua_newuserdata(L, sizeof(T*));
			T* obj = existing;
			obj->Grab();
			*ud = obj;

			// Get the address of the userdata on the
			// stack.
			int userdata = lua_gettop(L);

			// Call the SetupObject() function which sets up the
			// properties and functions on the new object and
			// pushes it onto the stack.  We use this function so
			// that we don't repeat code between Constructor, CreateNew
			// and CreateFromExisting.
			Bindings<T>::SetupObject(L, newtable, userdata);

			// Instead of returning the number of objects we pushed
			// onto the stack, we return the actual object itself since
			// CreateNew will be used from C++ code.
			return obj;
		}

		// A dummy wrapper function which simply returns
		// 1 at the moment.  Eventually, use of CreateNew
		// and CreateFromExisting within B++ files will be
		// superseded by automatic conversion at return time.
		static int Result(lua_State * L, T* ret)
		{
			return 1;
		}

		// Push numeric value onto stack.
		static int Result(lua_State * L, numeric ret)
		{
			lua_pushnumber(L, ret);
			return 1;
		}

		// Push numeric value onto stack.
		static int Result(lua_State * L, unsigned int ret)
		{
			lua_pushnumber(L, ret);
			return 1;
		}

		// Push numeric value onto stack.
		static int Result(lua_State * L, signed int ret)
		{
			lua_pushnumber(L, ret);
			return 1;
		}

		// Push string value onto stack.
		static int Result(lua_State * L, ::string ret)
		{
			lua_pushstring(L, ret);
			return 1;
		}

		// Push string value onto stack.
		static int Result(lua_State * L, const char* ret)
		{
			lua_pushstring(L, ret);
			return 1;
		}

		// Push string value onto stack.
		static int Result(lua_State * L, const wchar_t* ret)
		{
			lua_pushstring(L, ::string(ret));
			return 1;
		}

		// Push table value onto stack.
		static int Result(lua_State * L, table & ret)
		{
			ret.PushAsResult(L);
			return 1;
		}

		// Push boolean value onto stack.
		static int Result(lua_State * L, bool ret)
		{
			lua_pushboolean(L, ret);
			return 1;
		}

		// Push nil value onto stack.
		static int Result(lua_State * L)
		{
			lua_pushnil(L);
			return 1;
		}

		// Sets up the cache.
		static void CacheInit(lua_State * L)
		{
			lua_newtable(L);
			lua_setfield(L, LUA_REGISTRYINDEX, "Roket3D_Return_Cache");
		}

		// Caches the object that is going to be returned in the C
		// registry using luaL_ref.  Assumes that CreateNew or an
		// equivilant function has been called and the object to
		// be cached is on top of the stack.
		static int CacheStore(lua_State * L)
		{
			lua_getfield(L, LUA_REGISTRYINDEX, "Roket3D_Return_Cache");
			int tbl = lua_gettop(L);
			lua_pushvalue(L, -2);
			int ref = luaL_ref(L, tbl);
			lua_rawgeti(L, tbl, ref);
			lua_remove(L, tbl);

			return ref;
		}

		// Retrieves the specific object from the cache.
		static int CacheRetrieve(lua_State * L, int ref)
		{
			lua_getfield(L, LUA_REGISTRYINDEX, "Roket3D_Return_Cache");
			int tbl = lua_gettop(L);
			lua_rawgeti(L, tbl, ref);
			lua_remove(L, tbl);
			return 1;
		}

		// Removes the specified object from the cache.
		static void CacheDelete(lua_State * L, int ref)
		{
			lua_getfield(L, LUA_REGISTRYINDEX, "Roket3D_Return_Cache");
			luaL_unref(L, -1, ref);
			lua_pop(L, 1);
		}

	private:
		// Sets up the object at the specified 'newtable' index and the
		// userdata associated with it at the specified 'userdata' index.
		// This function exists to reduce code duplication.
		static void SetupObject(lua_State * L, int newtable, int userdata)
		{
			// Retrieve the metatable from the registry index (the
			// one we set in the Register function) and set it as
			// the metatable on the userdata.
			luaL_getmetatable(L, T::ClassName);
			lua_setmetatable(L, userdata);

			// Set the first index of the table to the userdata
			// (we pushed the index above using lua_pushnumber).
			lua_rawset(L, newtable);

			// Now we set the same metatable that we associated with
			// the userdata as the metatable on the new object we
			// are returning.
			luaL_getmetatable(L, T::ClassName);
			lua_setmetatable(L, newtable);

			// Now we're going to register the properties with our
			// metatable, so that setting or getting the properties
			// on the Lua object will result in the appropriate
			// callbacks being issued.
			luaL_getmetatable(L, T::ClassName);
			for (int i = 0; T::Properties[i].Name != NULL; i++)
			{
				lua_pushstring(L, T::Properties[i].Name);
				lua_pushnumber(L, i);
				lua_rawset(L, -3);
			}
			lua_pop(L, 1);

			// Now register the functions on the object with the
			// Bindings<T>::FunctionDispatch, which then handles
			// passing the correct context onto the class functions.
			// Note that unlike the properties, the functions are
			// associated with new object and not the metatable.
			for (int i = 0; T::Functions[i].Name; i++)
			{
				lua_pushstring(L, T::Functions[i].Name);
				lua_pushnumber(L, i);
				lua_pushcclosure(L, &Bindings<T>::FunctionDispatch, 1);
				lua_rawset(L, newtable);
			}
		}

	public:
		// The callback which handles when properties are asked
		// to be retrieved from the object.  Externally defined as
		// it requires the ability to throw an exception.
		static int PropertyGetter(lua_State * L);

		// The callback which handles when properties are asked
		// to be created or modified on the object.  Externally defined as
		// it requires the ability to throw an exception.
		static int PropertySetter(lua_State * L);

		// The callback which provides type information back
		// to Lua code (i.e. for catch or is operations).
		// Externally defined as it requires the ability to
		// throw an exception.
		static int Type(lua_State * L);

		// This function pushes the functable (class constructor
		// onto the top of the stack so that it can be used
		// with lua_is).
		static int PushType(lua_State * L);

		// The function is called to handle calling the C++
		// functions from within Lua (externally defined
		// as it requires the ability to throw an exception).
		static int FunctionDispatch(lua_State * L);

		// This function wraps a specified exception and pushes
		// it onto the Lua stack.
		static int RaiseException(lua_State * L, Engine::Exception * err);

		// This function is the callback issued by the Lua
		// garbage collector when there are no more references
		// to an object in Lua.  All of the objects that are
		// binded inherit Bindings<void>::BaseObject at one
		// point, which means they are reference counted.
		static int GCObj(lua_State * L)
		{
			// Get the userdata from the object.
			T** obj = static_cast<T**>(luaL_checkudata(L, -1, T::ClassName));

			// Either destroy the object or decrease the reference count.
			(*obj)->Drop();

			return 0;
		}
	};

// We can only include the exception classes after we define the bindings
// template.  Any functions that require the ability to throw exceptions
// are externally defined (inline) below.
#include "ArgumentTypeNotValidException.h"
#include "ContextNotProvidedException.h"
#include "DivideByZeroException.h"
#include "InheritedClassNotFoundException.h"
#include "ReadOnlyPropertyException.h"

template<class T>
	int Bindings<T>::Type(lua_State * L)
	{
		if (strcmp(T::Inherits, "") == 0 || strcmp(T::Inherits, "RObject") == 0)
		{
			// This object does not inherit any other
			// object.  Simply push it's class name.
			lua_pushstring(L, T::ClassName);
			return 1;
		}
		else
		{
			// This object inherits another object.  First
			// create a new table, set the class name as the
			// first index, and then we need to parse the
			// T::Inherits parameter to load the table for
			// the second index.

			// The table is now on top of the stack.
			lua_newtable(L);

			// Push the class name as the first index.  The
			// index of the table is -3 as it's directly below
			// the key-value pair.  Note that the key and the
			// value are pushed off the stack by lua_rawset,
			// so after this process the table is on top of
			// the stack.
			lua_pushnumber(L, 1);
			lua_pushstring(L, T::ClassName);
			lua_rawset(L, -3);

			// We use the std::string class to parse the
			// T::Inherits variable.  T::Inherits not only
			// contains the class name of the parent class, but
			// also the namespace context as well.
			std::string cls = T::Inherits;
			std::string buf = "";
			std::vector<std::string> elms;
			for (unsigned int i = 0; i < cls.length(); i++)
			{
				if (cls[i] == '.')
				{
					if (buf.length() > 0)
					{
						elms.insert(elms.end(), buf);
						buf = "";
					}
				}
				else
					buf += cls[i];
			}
			if (buf.length() > 0)
			{
				elms.insert(elms.end(), buf);
				buf = "";
			}

			// Make sure there is enough stack space to perform
			// all of the namespace loading, etc..
			lua_checkstack(L, elms.size() + 5);

			// Now loop through elms, loading all but the last element
			// as namespace tables.
			int topop = 0;
			bool failure = false;
			for (unsigned int i = 0; i < elms.size(); i++)
			{
				if (i < elms.size() - 1)
				{
					if (i == 0)
					{
						// Fetch the first from the global area.
						lua_getglobal(L, elms[i].c_str());
						if (lua_isnil(L, -1))
						{
							// The namespace does not yet exist, set
							// failure to true and break out of the
							// for loop.
							failure = true;
							break;
						}
						topop++;
					}
					else
					{
						lua_getfield(L, -1, elms[i].c_str());
						if (lua_isnil(L, -1))
						{
							// The namespace does not yet exist, set
							// failure to true and break out of the
							// for loop.
							failure = true;
							break;
						}
						topop++;
					}
				}
				else
				{
					if (i == 0)
					{
						// Pop all of the values so that the
						// table is back on top of the stack.
						// Then get the inherited class directly
						// from the global namespace and set it
						// as the second value in the table.
						lua_pop(L, topop);
						topop = 0;
						lua_pushnumber(L, 2);
						lua_getglobal(L, elms[i].c_str());
						lua_rawset(L, -3);
					}
					else
					{
						// Bring the table back to the top of the
						// stack.  The namespace we need to fetch
						// the class from will be just below the
						// table (-2), before we push the key, so
						// the index argument will be -3 for
						// lua_getfield.  Since lua_getfield pushes
						// a value onto the stack, the index of the
						// table will also be -3 (as the push shifted
						// -3 up be one to be the table).
						lua_pushvalue(L, -topop - 1);
						lua_pushnumber(L, 2);
						lua_getfield(L, -3, elms[i].c_str());
						lua_rawset(L, -3);

						// Now clean up the stack by popping the table
						// off the top of the stack (remember it's still
						// below all of the namespace / class values).
						lua_pop(L, 1);
					}
				}
			}
			lua_pop(L, topop);

			if (failure)
			{
				// We couldn't locate the parent table in Lua, so
				// it might not be available yet.  We'll throw an
				// exception to indicate this unexpected situation.
				return Bindings<T>::RaiseException(L, new Engine::InheritedClassNotFoundException());
			}

			// Now we have a table that describes the type of the
			// class, and it's on top of the stack.  Return 1.
			return 1;
		}
	}

template<class T>
	int Bindings<T>::PushType(lua_State * L)
	{
		// We're going to push a table which only has a
		// __type value that points to Bindings<T>::Type.
		
		lua_newtable(L);
		int tbl = lua_gettop(L);
		lua_newtable(L);
		int metatable = lua_gettop(L);
		lua_pushstring(L, "__type");
		Bindings<T>::Type(L);
		lua_rawset(L, metatable);
		lua_setmetatable(L, tbl);
		return 1;
	}

template<class T>
	int Bindings<T>::FunctionDispatch(lua_State * L)
	{
		// Check to make sure that the function was called
		// with the : operator and not the . operator.  The
		// . operator does not provide us with an instance
		// context.
		// TODO: Check that the first argument is of the
		// correct type.
		if (lua_gettop(L) == 0 || !lua_istable(L, 1))
		{
			return Bindings<T>::RaiseException(L, new Engine::ContextNotProvidedException());
		}

		// Retrieve the index of the function from the
		// closure that was created during setup.
		int i = (int)lua_tonumber(L, lua_upvalueindex(1));

		// The table is located at index 1, so we can pull
		// the userdata associated with it just like we
		// do in the property functions (except that we don't
		// need to load the metatable onto the stack).
		lua_pushnumber(L, 0);
		lua_rawget(L, 1);

		// Retrieve the userdata.
		T** obj = static_cast<T**>(lua_touserdata(L, -1));

		// Remove the original table off the stack as well
		// as the userdata so that argument 1 is at index 1
		// when the function is called.
		lua_remove(L, 1);
		lua_pop(L, 1);

		// Call the function and return it's result.
		try
		{
			return __guarded<T>::__guard(*obj, T::Functions[i].Function, L);
		}
		catch (Engine::Exception * err)
		{
			return Bindings<T>::RaiseException(L, err);
		}
	}

template<class T>
	int Bindings<T>::PropertyGetter(lua_State * L)
	{
		// When this function is called, the object
		// is at index 1 and the name of the property
		// is at index 2.

		const char * func = lua_tostring(L, 1);
		const char * prop = lua_tostring(L, 2);
		int top = lua_gettop(L);

		// Retrieve the metatable for the object.
		lua_getmetatable(L, 1);

		// Push the name of the property onto the stack
		// and then fetch the index of that property.
		lua_pushvalue(L, 2);
		lua_rawget(L, -2);

		// Check to make sure that the value on the stack
		// is a number (i.e. the index of the property).
		// If it isn't a number, then the property doesn't
		// exist.
		if (lua_isnumber(L, -1))
		{
			// Store the index.
			int index = lua_tonumber(L, -1);

			// Get the first entry in the table (not meta),
			// which is the userdata associated with the
			// object.
			lua_pushnumber(L, 0);
			lua_rawget(L, 1);

			// Retrieve the userdata.
			T** obj = static_cast<T**>(lua_touserdata(L, -1));

			// Push the metatable to the top of the stack
			// for the retrieval function to access if needed.
			lua_pushvalue(L, 3);

			// Call the property retrieval function.
			try
			{
				return __guarded<T>::__guard(*obj, T::Properties[index].Getter, L);
			}
			catch (Engine::Exception * err)
			{
				return Bindings<T>::RaiseException(L, err);
			}
		}
		else
		{
			// The property doesn't exist.
			lua_pushnil(L);
			return 1;
		}
	}

template<class T>
	int Bindings<T>::PropertySetter(lua_State * L)
	{
		// When this function is called, the object
		// is at index 1 and the name of the property
		// is at index 2.  The value to associate the
		// property with is at index 3.
		
		// Retrieve the metatable for the object.
		lua_getmetatable(L, 1);

		// Push the name of the property onto the stack
		// and then fetch the index of that property.
		lua_pushvalue(L, 2);
		lua_rawget(L, -2);

		// Check to see if the result is nil.  If it is,
		// then the property we are setting doesn't exist
		// so we're just going to do a normal set.
		if (lua_isnil(L, -1))
		{
			// Pop the key and the metatable off the stack.
			lua_pop(L, 2);

			// And do a normal set.
			lua_rawset(L, 1);

			return 0;
		}
		else
		{
			// Store the index.
			int index = lua_tonumber(L, -1);

			// Get the first entry in the metatable, which
			// is the userdata associated with the object.
			lua_pushnumber(L, 0);
			lua_rawget(L, 1);

			// Retrieve the userdata.
			T** obj = static_cast<T**>(lua_touserdata(L, -1));

			// Push the value to the top of the stack.
			// This makes the value available at position -1
			// ready for the setter function to use it.
			lua_pushvalue(L, 3);

			// Check to see whether we have a read-only property.
			if (T::Properties[index].Setter == NULL)
			{
				return Bindings<T>::RaiseException(L, new Engine::ReadOnlyPropertyException());
			}

			// Call the property modification function.
			try
			{
				return __guarded<T>::__guard(*obj, T::Properties[index].Setter, L);
			}
			catch (Engine::Exception * err)
			{
				return Bindings<T>::RaiseException(L, err);
			}
		}
	}

template<class T>
	int Bindings<T>::RaiseException(lua_State * L, Engine::Exception * err)
	{
		// Create a new table with which to return
		// our new Lua exception.
		lua_newtable(L);

		// Get the address of the new table on the
		// stack.
		int newtable = lua_gettop(L);

		// Push the index of the userdata.
		lua_pushnumber(L, 0);

		// Essentially here we're setting the userdata in
		// the metatable to the Bindings<Engine::Exception> object.
		// This allows the function callbacks and property setter
		// and getter functions to know what C++ object they
		// are dealing with.
		void** ud = (void**)lua_newuserdata(L, sizeof(void*));
		err->Grab();
		*ud = err;

		// Get the address of the userdata on the
		// stack.
		int userdata = lua_gettop(L);

		// Instead of using SetupObject, we manually create the
		// object because we need to base the class name off the
		// value from the GetName() call as well as the fact it
		// needs to be pushed via lua_error().

		// Retrieve the metatable from the registry index (the
		// one we set in the Register function) and set it as
		// the metatable on the userdata.
		luaL_getmetatable(L, err->GetName());
		lua_setmetatable(L, userdata);

		// Set the first index of the table to the userdata
		// (we pushed the index above using lua_pushnumber).
		lua_rawset(L, newtable);

		// Now we set the same metatable that we associated with
		// the userdata as the metatable on the new object we
		// are returning.
		luaL_getmetatable(L, err->GetName());
		lua_setmetatable(L, newtable);

		// Now we're going to register the properties with our
		// metatable, so that setting or getting the properties
		// on the Lua object will result in the appropriate
		// callbacks being issued.
		luaL_getmetatable(L, err->GetName());
		for (int i = 0; err->Properties[i].Name != NULL; i++)
		{
			lua_pushstring(L, err->Properties[i].Name);
			lua_pushnumber(L, i);
			lua_rawset(L, -3);
		}
		lua_pop(L, 1);

		// Now register the functions on the object with the
		// Bindings<Engine::Exception>::FunctionDispatch, which then handles
		// passing the correct context onto the class functions.
		// Note that unlike the properties, the functions are
		// associated with new object and not the metatable.
		for (int i = 0; err->Functions[i].Name; i++)
		{
			lua_pushstring(L, err->Functions[i].Name);
			lua_pushnumber(L, i);
			lua_pushcclosure(L, err->Dispatcher, 1);
			lua_rawset(L, newtable);
		}

		// Now we push the top of the stack via lua_error.
		// lua_error never returns, but we call return just
		// for looks.
		lua_error(L);
		return 1;
	}

// Type-specific ArgumentBase handlers.  The contents of the function must
// change based on the type specified by T, therefore these are declared
// outside the template.
inline numeric Bindings<numeric>::GetArgumentBase(lua_State * L, int narg)
{
	// Convert to absolute reference.
	if (narg < 0) narg = lua_gettop(L) + narg + 1;

	if (lua_isnumber(L, narg))
		return lua_tonumber(L, narg);
	else
		throw new Engine::ArgumentTypeNotValidException(narg,
			lua_advtypename(L, narg), lua_typename(L, LUA_TNUMBER));
}

inline ::string Bindings<::string>::GetArgumentBase(lua_State * L, int narg)
{
	// Convert to absolute reference.
	if (narg < 0) narg = lua_gettop(L) + narg + 1;

	if (lua_isstring(L, narg))
		return lua_tostring(L, narg);
	else
		throw new Engine::ArgumentTypeNotValidException(narg,
			lua_advtypename(L, narg), lua_typename(L, LUA_TSTRING));
}

inline bool Bindings<bool>::GetArgumentBase(lua_State * L, int narg)
{
	// Convert to absolute reference.
	if (narg < 0) narg = lua_gettop(L) + narg + 1;

	if (lua_isboolean(L, narg))
		return (lua_toboolean(L, narg) == 1);
	else
		throw new Engine::ArgumentTypeNotValidException(narg,
			lua_advtypename(L, narg), lua_typename(L, LUA_TBOOLEAN));
}

inline table Bindings<table>::GetArgumentBase(lua_State * L, int narg)
{
	// Convert to absolute reference.
	if (narg < 0) narg = lua_gettop(L) + narg + 1;

	if (lua_istable(L, narg))
		return table::GetFromStack(L, narg);
	else
		throw new Engine::ArgumentTypeNotValidException(narg,
			lua_advtypename(L, narg), lua_typename(L, LUA_TTABLE));
}

inline function Bindings<function>::GetArgumentBase(lua_State * L, int narg)
{
	// Convert to absolute reference.
	if (narg < 0) narg = lua_gettop(L) + narg + 1;

	if (lua_isfunction(L, narg))
		return function(L, narg);
	else
		throw new Engine::ArgumentTypeNotValidException(narg,
			lua_advtypename(L, narg), lua_typename(L, LUA_TFUNCTION));
}

// Default versions.
inline numeric Bindings<numeric>::GetArgumentBase(lua_State * L, int narg, numeric def)
{
	// Convert to absolute reference.
	if (narg < 0) narg = lua_gettop(L) + narg + 1;

	if (narg > lua_gettop(L) || lua_isnil(L, narg)) return def;

	if (lua_isnumber(L, narg))
		return lua_tonumber(L, narg);
	else
		throw new Engine::ArgumentTypeNotValidException(narg,
			lua_advtypename(L, narg), lua_typename(L, LUA_TNUMBER));
}

inline ::string Bindings<::string>::GetArgumentBase(lua_State * L, int narg, ::string def)
{
	// Convert to absolute reference.
	if (narg < 0) narg = lua_gettop(L) + narg + 1;

	if (narg > lua_gettop(L) || lua_isnil(L, narg)) return def;

	if (lua_isstring(L, narg))
		return lua_tostring(L, narg);
	else
		throw new Engine::ArgumentTypeNotValidException(narg,
			lua_advtypename(L, narg), lua_typename(L, LUA_TSTRING));
}

inline bool Bindings<bool>::GetArgumentBase(lua_State * L, int narg, bool def)
{
	// Convert to absolute reference.
	if (narg < 0) narg = lua_gettop(L) + narg + 1;

	if (narg > lua_gettop(L) || lua_isnil(L, narg)) return def;

	if (lua_isboolean(L, narg))
		return (lua_toboolean(L, narg) == 1);
	else
		throw new Engine::ArgumentTypeNotValidException(narg,
			lua_advtypename(L, narg), lua_typename(L, LUA_TBOOLEAN));
}

inline bool Bindings<numeric>::IsArgumentBase(lua_State * L, int narg)
{
	// Convert to absolute reference.
	if (narg < 0) narg = lua_gettop(L) + narg + 1;

	return (lua_isnumber(L, narg) == 1);
}

inline bool Bindings<::string>::IsArgumentBase(lua_State * L, int narg)
{
	// Convert to absolute reference.
	if (narg < 0) narg = lua_gettop(L) + narg + 1;

	return (lua_isstring(L, narg) == 1);
}

inline bool Bindings<bool>::IsArgumentBase(lua_State * L, int narg)
{
	// Convert to absolute reference.
	if (narg < 0) narg = lua_gettop(L) + narg + 1;

	return (lua_isboolean(L, narg) == 1);
}

inline bool Bindings<table>::IsArgumentBase(lua_State * L, int narg)
{
	// Convert to absolute reference.
	if (narg < 0) narg = lua_gettop(L) + narg + 1;

	return (lua_istable(L, narg) == 1);
}

inline bool Bindings<function>::IsArgumentBase(lua_State * L, int narg)
{
	// Convert to absolute reference.
	if (narg < 0) narg = lua_gettop(L) + narg + 1;

	return (lua_isfunction(L, narg) == 1);
}

#endif