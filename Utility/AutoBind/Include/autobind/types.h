/*
 * This header file is part of the AutoBind
 * toolset.  It typedef's generic types such
 * as string and numeric to their C++
 * equivilants.
 */

#ifndef AUTOBIND_types
#define AUTOBIND_types

// Include Lua.
#include <lua.hpp>
#include <malloc.h>
#include <string.h>
#include <string>
#include <vector>

typedef lua_Number numeric;
typedef void* variant;

class string
{
public:
	string()
	{
		this->len = 0;
		this->wdata = (wchar_t*)malloc(sizeof(wchar_t));
		this->wdata[0] = 0;

		// Initalize the ASCII version.
		this->InitalizeASCII();
	}

	string(const string & copy)
	{
		// Copy the string's data.
		this->len = copy.len;
		this->wdata = (wchar_t*)malloc((this->len + 1) * sizeof(wchar_t));
		for (size_t i = 0; i < this->len; i += 1)
			this->wdata[i] = copy.wdata[i];
		this->wdata[this->len] = 0; // NULL terminator.

		// Initalize the ASCII version.
		this->InitalizeASCII();
	}

	string(const char* text)
	{
		// Allocate memory to be used by the string.
		this->len = strlen(text);
		this->wdata = (wchar_t*)malloc((this->len + 1) * sizeof(wchar_t));
		for (size_t i = 0; i < this->len; i += 1)
			this->wdata[i] = text[i];
		this->wdata[this->len] = 0; // NULL terminator.

		// Initalize the ASCII version.
		this->InitalizeASCII();
	}

	string(const wchar_t* text)
	{
		// Allocate memory to be used by the string.
		this->len = wcslen(text);
		this->wdata = (wchar_t*)malloc((this->len + 1) * sizeof(wchar_t));
		for (size_t i = 0; i < this->len; i += 1)
			this->wdata[i] = text[i];
		this->wdata[this->len] = 0; // NULL terminator.

		// Initalize the ASCII version.
		this->InitalizeASCII();
	}

	~string()
	{
		// Free the memory used by the string.
		free(this->wdata);
		free(this->cdata);
	}

	operator const char*()
	{
		return const_cast<const char*>(this->cdata);
	}

	operator const wchar_t*()
	{
		return const_cast<const wchar_t*>(this->wdata);
	}

	operator const std::string()
	{
		// Implicit conversion to const std::string.
		const std::string ret = (const char*)this->wdata;
		return ret;
	}

	bool operator ==(const string& b) const
	{
		// String comparison.
		return (wcscmp(this->wdata, b.wdata) == 0);
	}

	string & operator =(const string& rhs)
	{
		// String assignment.
		if (this == &rhs)
			return *this;

		// Deallocate existing data.
		free(this->wdata);

		// Allocate new memory.
		this->len = rhs.len;
		this->wdata = (wchar_t*)malloc((this->len + 1) * sizeof(wchar_t));
		for (size_t i = 0; i < this->len; i += 1)
			this->wdata[i] = rhs.wdata[i];
		this->wdata[this->len] = 0; // NULL terminator.

		// Update the ASCII version.
		this->MaintainASCII();

		return *this;
	}

private:
	void MaintainASCII()
	{
		// Deallocate existing data.
		free(this->cdata);

		// Allocate new memory.
		this->InitalizeASCII();
	}

	void InitalizeASCII()
	{
		// Allocate new memory.
		this->cdata = (char*)malloc((this->len + 1) * sizeof(char));
		for (size_t i = 0; i < this->len; i += 1)
			this->cdata[i] = this->wdata[i];
		this->cdata[this->len] = 0; // NULL terminator.
	}

	wchar_t* wdata;
	char* cdata;
	size_t len;
};

// Predeclare class references for storage structs.
class table;
class RObject;

namespace tableutil
{
	// Enum for returning the type of object.
	enum ValueType
	{
		t_nil,
		t_bool,
		t_numeric,
		t_string,
		t_table,
		t_object,
		t_other
	};

	// Structure for storing keys and values within m_Data.
	struct ValueStorage
	{
		ValueType type;
		struct
		{
			bool b;
			numeric n;
			::string s;
			table * t;
			RObject * o;
		} store;

		ValueStorage() { type = t_nil; }
		ValueStorage(bool v) { type = t_bool; store.b = v; }
		ValueStorage(numeric v) { type = t_numeric; store.n = v; }
		ValueStorage(::string v) { type = t_string; store.s = v; }
		ValueStorage(table * v) { type = t_table; store.t = v; } // CAUTION!  You must pass this function 'new table(value)' where value is the table you want to store.
		ValueStorage(RObject * v) { type = t_object; store.o = v; }
		ValueStorage(const ValueStorage & copy);
		~ValueStorage();

		ValueStorage & operator =(const ValueStorage& rhs);

		bool operator ==(const ValueStorage& b) const
		{
			if (type != b.type) return false;
			switch (type)
			{
			case t_nil:
				return (b.type == t_nil);
			case t_bool:
				return (store.b == b.store.b);
			case t_numeric:
				return (store.n == b.store.n);
			case t_string:
				return (store.s == b.store.s);
			case t_table:
				return (store.t == b.store.t);
			case t_object:
				return (store.o == b.store.o);
			case t_other:
				return false;
			default:
				return false;
			}
		}
	};

	// Structure that's returned when accessing via index.  Overloads
	// the assignment operator so that C++ users can assign to the
	// variable.
	struct value
	{
	private:
		table * container;
		ValueStorage key;
		ValueStorage val;

	public:
		ValueType type;

		value(table * container, ValueStorage key, ValueStorage value)
		{
			this->container = container;
			this->key = key;
			this->val = value;
			this->type = value.type;
		}

		value& operator =(const bool& v);
		value& operator =(const numeric& v);
		value& operator =(const ::string& v);
		value& operator =(const table*& v);
		value& operator =(const RObject*& v);

		operator bool() { if (this->val.type == t_bool) return this->val.store.b; else return false; }
		operator numeric() { if (this->val.type == t_numeric) return this->val.store.n; else return 0; }
		operator ::string() { if (this->val.type == t_string) return this->val.store.s; else return ""; }
		operator table*() { if (this->val.type == t_table) return this->val.store.t; else return NULL; }
		operator RObject *() { if (this->val.type == t_object) return this->val.store.o; else return NULL; }
	};
}

class table
{
public:
	table()
	{
	}

	table(table & copy)
	{
		// Copy contents of the other tables m_Data.
		for (m_DataIterator i = copy.m_Data.begin();
			i != copy.m_Data.end(); i++)
		{
			tableutil::ValueStorage key = i->first;
			tableutil::ValueStorage value = i->second;

			this->m_Data.insert(this->m_Data.end(), m_Pair(key, value));
		}
	}

	void Set(bool key, bool value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(numeric key, bool value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(::string key, bool value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(table key, bool value) { this->SetKeyValue(tableutil::ValueStorage(new table(key)), tableutil::ValueStorage(value)); }
	void Set(RObject * key, bool value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(tableutil::ValueStorage key, bool value) { this->SetKeyValue(key, tableutil::ValueStorage(value)); }
	void Set(bool key, numeric value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(numeric key, numeric value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(::string key, numeric value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(table key, numeric value) { this->SetKeyValue(tableutil::ValueStorage(new table(key)), tableutil::ValueStorage(value)); }
	void Set(RObject * key, numeric value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(tableutil::ValueStorage key, numeric value) { this->SetKeyValue(key, tableutil::ValueStorage(value)); }
	void Set(bool key, ::string value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(numeric key, ::string value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(::string key, ::string value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(table key, ::string value) { this->SetKeyValue(tableutil::ValueStorage(new table(key)), tableutil::ValueStorage(value)); }
	void Set(RObject * key, ::string value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(tableutil::ValueStorage key, ::string value) { this->SetKeyValue(key, tableutil::ValueStorage(value)); }
	void Set(bool key, table value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(new table(value))); }
	void Set(numeric key, table value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(new table(value))); }
	void Set(::string key, table value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(new table(value))); }
	void Set(table key, table value) { this->SetKeyValue(tableutil::ValueStorage(new table(key)), tableutil::ValueStorage(new table(value))); }
	void Set(RObject * key, table value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(new table(value))); }
	void Set(tableutil::ValueStorage key, table value) { this->SetKeyValue(key, tableutil::ValueStorage(new table(value))); }
	void Set(bool key, RObject * value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(numeric key, RObject * value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(::string key, RObject * value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(table key, RObject * value) { this->SetKeyValue(tableutil::ValueStorage(new table(key)), tableutil::ValueStorage(value)); }
	void Set(RObject * key, RObject * value) { this->SetKeyValue(tableutil::ValueStorage(key), tableutil::ValueStorage(value)); }
	void Set(tableutil::ValueStorage key, RObject * value) { this->SetKeyValue(key, tableutil::ValueStorage(value)); }

	tableutil::value operator [](bool key) { return tableutil::value(this, tableutil::ValueStorage(key), this->GetValueByKey(tableutil::ValueStorage(key))); }
	tableutil::value operator [](numeric key) { return tableutil::value(this, tableutil::ValueStorage(key), this->GetValueByKey(tableutil::ValueStorage(key))); }
	tableutil::value operator [](::string key) { return tableutil::value(this, tableutil::ValueStorage(key), this->GetValueByKey(tableutil::ValueStorage(key))); }
	tableutil::value operator [](RObject * key) { return tableutil::value(this, tableutil::ValueStorage(key), this->GetValueByKey(tableutil::ValueStorage(key))); }

	table & operator =(const table& rhs)
	{
		// Remove our existing data.
		this->m_Data.clear();

		// Copy contents of the other tables m_Data.
		for (m_ConstDataIterator i = rhs.m_Data.begin();
			i != rhs.m_Data.end(); i++)
		{
			tableutil::ValueStorage key = tableutil::ValueStorage(i->first);
			tableutil::ValueStorage value = tableutil::ValueStorage(i->second);

			this->m_Data.insert(this->m_Data.end(), m_Pair(key, value));
		}
	}

	void SetKeyValue(tableutil::ValueStorage key, tableutil::ValueStorage value)
	{
		for (m_DataIterator i = this->m_Data.begin();
			i != this->m_Data.end(); i++)
		{
			if (i->first == key)
			{
				i->second = value;
				return;
			}
		}
		
		// Not found, add.
		this->m_Data.insert(this->m_Data.end(), m_Pair(key, value));
		return;
	}

	// This function pushes the entire table to the top of the stack (so
	// that it can be correctly accessed).
	inline void PushAsResult(lua_State * L)
	{
		// Create a new table.
		lua_newtable(L);
		int tbl = lua_gettop(L);

		// Loop through all of our key-value pairs, adding
		// them to the table.
		for (m_DataIterator i = this->m_Data.begin();
			i != this->m_Data.end(); i++)
		{
			// Push the key.
			switch (i->first.type)
			{
			case tableutil::t_nil:
				continue;
			case tableutil::t_bool:
				lua_pushboolean(L, i->first.store.b);
				break;
			case tableutil::t_numeric:
				lua_pushnumber(L, i->first.store.n);
				break;
			case tableutil::t_string:
				lua_pushstring(L, i->first.store.s);
				break;
			case tableutil::t_table:
				(i->first.store.t)->PushAsResult(L);
				break;
			default:
				return; // No support for pushing any other types as table keys.
			}

			// Push the value.
			switch (i->second.type)
			{
			case tableutil::t_nil:
				continue;
			case tableutil::t_bool:
				lua_pushboolean(L, i->second.store.b);
				break;
			case tableutil::t_numeric:
				lua_pushnumber(L, i->second.store.n);
				break;
			case tableutil::t_string:
				lua_pushstring(L, i->second.store.s);
				break;
			case tableutil::t_table:
				(i->second.store.t)->PushAsResult(L);
				break;
			default:
				return; // No support for pushing any other types as table keys.
			}
			
			// Assign the key-value pair.
			lua_rawset(L, tbl);
		}
	}

	// This function retrieves a lua table from the stack and represents
	// it as an instance of the table class.
	static inline table GetFromStack(lua_State * L, int pos)
	{
		table t = table();
		if (pos < 0) pos = lua_gettop(L) + pos + 1;

		// Traverse the keys in the table.
		lua_pushnil(L);  /* first key */
		while (lua_next(L, pos) != 0)
		{
			tableutil::ValueStorage key;
			tableutil::ValueStorage value;

			// TODO: Support tables as both keys and values - we may need
			//       to change the function signature of GetFromStack so
			//       that we can return a pointer as required by ValueStorage
			//       (or implement the copy constructor on table).

			/* uses 'key' (at index -2) and 'value' (at index -1) */
			if (lua_isboolean(L, -2))
				key = tableutil::ValueStorage(lua_toboolean(L, -2) == 1);
			else if (lua_isnumber(L, -2))
				key = tableutil::ValueStorage(lua_tonumber(L, -2));
			else if (lua_isstring(L, -2))
				key = tableutil::ValueStorage(::string(lua_tostring(L, -2)));
			else
			{
				lua_pop(L, 1);
				continue;
			}

			if (lua_isboolean(L, -1))
				value = tableutil::ValueStorage(lua_toboolean(L, -1) == 1);
			else if (lua_isnumber(L, -1))
				value = tableutil::ValueStorage(lua_tonumber(L, -1));
			else if (lua_isstring(L, -1))
				value = tableutil::ValueStorage(::string(lua_tostring(L, -1)));
			else
			{
				lua_pop(L, 1);
				continue;
			}

			// Assign the key-value pair to the table.
			t.SetKeyValue(key, value);

			/* removes 'value'; keeps 'key' for next iteration */
			lua_pop(L, 1);
		}

		return t;
	}

private:
	tableutil::ValueStorage GetValueByKey(tableutil::ValueStorage key)
	{
		for (m_DataIterator i = this->m_Data.begin();
			i != this->m_Data.end(); i++)
		{
			if (i->first == key)
				return i->second;
		}

		return tableutil::ValueStorage(); // Return nil.
	}

	std::vector<std::pair<tableutil::ValueStorage, tableutil::ValueStorage>> m_Data;
	typedef std::vector<std::pair<tableutil::ValueStorage, tableutil::ValueStorage>>::iterator m_DataIterator;
	typedef std::vector<std::pair<tableutil::ValueStorage, tableutil::ValueStorage>>::const_iterator m_ConstDataIterator;
	typedef std::pair<tableutil::ValueStorage, tableutil::ValueStorage> m_Pair;
};

class function
{
private:
	int m_Reference;
	lua_State * m_L;

public:
	function()
	{
		this->m_L = NULL;
		this->m_Reference = -1;
	}

	function(lua_State * L, int pos)
	{
		if (!lua_isfunction(L, pos))
		{
			this->m_L = NULL;
			this->m_Reference = -1;
			return;
		}

		this->m_L = L;
		lua_getfield(this->m_L, LUA_REGISTRYINDEX, "Roket3D_Function_Bindings");
		lua_pushvalue(this->m_L, pos);

		if (!lua_isfunction(this->m_L, -1))
		{
			this->m_L = NULL;
			this->m_Reference = -1;
			return;
		}

		this->m_Reference = luaL_ref(this->m_L, -2);
		lua_pop(this->m_L, 1);
	}

	function(const function & copy)
	{
		this->m_L = copy.m_L;
		lua_getfield(this->m_L, LUA_REGISTRYINDEX, "Roket3D_Function_Bindings");
		lua_rawgeti(this->m_L, -1, copy.m_Reference);

		if (!lua_isfunction(this->m_L, -1))
		{
			this->m_L = NULL;
			this->m_Reference = -1;
			return;
		}

		this->m_Reference = luaL_ref(this->m_L, -2);
		lua_pop(this->m_L, 1);
	}

	~function()
	{
		if (this->m_L != NULL)
		{
			lua_getfield(this->m_L, LUA_REGISTRYINDEX, "Roket3D_Function_Bindings");
			luaL_unref(this->m_L, -1, this->m_Reference);
			lua_pop(this->m_L, 1);
		}
	}

	static void Setup(lua_State * L)
	{
		lua_newtable(L); int tbl = lua_gettop(L);
		lua_newtable(L); int meta = lua_gettop(L);
		lua_pushstring(L, "__setindex");
		lua_pushcfunction(L, &function::CheckSet);
		lua_settable(L, meta);
		lua_setmetatable(L, tbl);
		lua_setfield(L, LUA_REGISTRYINDEX, "Roket3D_Function_Bindings");
	}

	static int CheckSet(lua_State * L)
	{
		// 1 - table
		// 2 - key
		// 3 - value

		if (lua_isfunction(L, 3))
		{
			// set only functions
			lua_rawset(L, 1);
		}
		else
		{
			int x = 0;
		}

		return 0;
	}

	void Call(table args)
	{
		if (this->m_L == NULL)
			return;

		lua_getfield(this->m_L, LUA_REGISTRYINDEX, "Roket3D_Function_Bindings");
		int tbl = lua_gettop(this->m_L);

		lua_rawgeti(this->m_L, tbl, this->m_Reference);
		if (!lua_isfunction(this->m_L, lua_gettop(this->m_L)))
			return;

		int count = 0;
		tableutil::value value = args[(numeric)1];
		while (value.type != tableutil::t_nil)
		{
			count += 1;
			switch (value.type)
			{
			case tableutil::t_bool:
				lua_pushboolean(this->m_L, value);
				break;
			case tableutil::t_numeric:
				lua_pushnumber(this->m_L, value);
				break;
			case tableutil::t_string:
				lua_pushstring(this->m_L, ::string(value));
				break;
			case tableutil::t_table:
				((table*)value)->PushAsResult(this->m_L);
				break;
			default:
				return; // No support for pushing any other types as arguments.
			}
			value = args[(numeric)(count + 1)];
		}

		lua_call(this->m_L, count, 0);
		lua_pop(this->m_L, 1);
	}

	bool CallBool(table args)
	{
		if (this->m_L == NULL)
			return false;

		lua_getfield(this->m_L, LUA_REGISTRYINDEX, "Roket3D_Function_Bindings");
		int tbl = lua_gettop(this->m_L);

		lua_rawgeti(this->m_L, tbl, this->m_Reference);
		if (!lua_isfunction(this->m_L, lua_gettop(this->m_L)))
			return false;

		int count = 0;
		tableutil::value value = args[(numeric)1];
		while (value.type != tableutil::t_nil)
		{
			count += 1;
			switch (value.type)
			{
			case tableutil::t_bool:
				lua_pushboolean(this->m_L, value);
				break;
			case tableutil::t_numeric:
				lua_pushnumber(this->m_L, value);
				break;
			case tableutil::t_string:
				lua_pushstring(this->m_L, ::string(value));
				break;
			case tableutil::t_table:
				((table*)value)->PushAsResult(this->m_L);
				break;
			default:
				return false; // No support for pushing any other types as arguments.
			}
			value = args[(numeric)(count + 1)];
		}

		lua_call(this->m_L, count, 1);
		if (lua_toboolean(this->m_L, -1) == 1)
		{
			lua_pop(this->m_L, 1);
			return true;
		}
		else
		{
			lua_pop(this->m_L, 1);
			return false;
		}
	}

	table CallTable(table args)
	{
		if (this->m_L == NULL)
			return table();

		lua_getfield(this->m_L, LUA_REGISTRYINDEX, "Roket3D_Function_Bindings");
		int tbl = lua_gettop(this->m_L);

		lua_rawgeti(this->m_L, tbl, this->m_Reference);
		if (!lua_isfunction(this->m_L, lua_gettop(this->m_L)))
			return table();

		int count = 0;
		tableutil::value value = args[(numeric)1];
		while (value.type != tableutil::t_nil)
		{
			count += 1;
			switch (value.type)
			{
			case tableutil::t_bool:
				lua_pushboolean(this->m_L, value);
				break;
			case tableutil::t_numeric:
				lua_pushnumber(this->m_L, value);
				break;
			case tableutil::t_string:
				lua_pushstring(this->m_L, ::string(value));
				break;
			case tableutil::t_table:
				((table*)value)->PushAsResult(this->m_L);
				break;
			default:
				return table(); // No support for pushing any other types as arguments.
			}
			value = args[(numeric)(count + 1)];
		}

		lua_call(this->m_L, count, 1);
		table ret = table::GetFromStack(this->m_L, -1);
		lua_pop(this->m_L, 1);
		return ret;
	}
};

#endif