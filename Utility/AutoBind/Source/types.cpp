/*
 * This source file is part of the AutoBind
 * toolset.  It typedef's generic types such
 * as string and numeric to their C++
 * equivilants.
 */

#include "autobind/types.h"

tableutil::ValueStorage::ValueStorage(const tableutil::ValueStorage & copy)
{
	type = copy.type;
	switch (type)
	{
	case t_nil:
		break;
	case t_bool:
		store.b = copy.store.b;
		break;
	case t_numeric:
		store.n = copy.store.n;
		break;
	case t_string:
		store.s = copy.store.s;
		break;
	case t_table:
		store.t = new table(*(copy.store.t));
		break;
	case t_object:
		store.o = copy.store.o;
		break;
	}
}

tableutil::ValueStorage::~ValueStorage()
{
	if (type == t_table)
		delete store.t;
}

tableutil::ValueStorage & tableutil::ValueStorage::operator =(const tableutil::ValueStorage& rhs)
{
	if (this == &rhs)
		return *this;

	if (type == t_table)
		delete store.t;

	type = rhs.type;
	switch (type)
	{
	case t_nil:
		break;
	case t_bool:
		store.b = rhs.store.b;
		break;
	case t_numeric:
		store.n = rhs.store.n;
		break;
	case t_string:
		store.s = rhs.store.s;
		break;
	case t_table:
		store.t = new table(*(rhs.store.t));
		break;
	case t_object:
		store.o = rhs.store.o;
		break;
	}
}

tableutil::value& tableutil::value::operator =(const bool& v) { container->Set(this->key, v); this->val.store.b = v; this->type = this->val.type = t_bool; return *this; }
tableutil::value& tableutil::value::operator =(const numeric& v) { container->Set(this->key, v); this->val.store.n = v; this->type = this->val.type = t_numeric; return *this; }
tableutil::value& tableutil::value::operator =(const ::string& v) { container->Set(this->key, v); this->val.store.s = v; this->type = this->val.type = t_string; return *this; }
tableutil::value& tableutil::value::operator =(const table*& v)
{
	container->Set(this->key, const_cast<table*>(v));
	this->val.store.t = const_cast<table*>(v);
	this->type = this->val.type = t_table;
	return *this;
}
tableutil::value& tableutil::value::operator =(const RObject*& v)
{
	container->Set(this->key, const_cast<RObject*>(v));
	this->val.store.o = const_cast<RObject*>(v);
	this->type = this->val.type = t_object;
	return *this;
}