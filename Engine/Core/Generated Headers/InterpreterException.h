/*
 * This file was automatically generated by AutoBind.
 * Any changes made to this file will be lost when it
 * is next regenerated.
 */

#ifndef CLASS_Engine_InterpreterException
#define CLASS_Engine_InterpreterException

/* Imports */
class Object;
namespace Engine { class Exception; } 
#include "Object.h"
#include "Exception.h"

/* Using declarations */

namespace Engine
{
    /* Begin class declaration */
    class InterpreterException : public Engine::Exception
    {
        /* Variable declarations */
        public: static const char* Message;

        /* Method and constructor declarations */
        public: InterpreterException();
        public: InterpreterException(const char* err);
        public: InterpreterException(lua_State * L, bool byuser);
        public: virtual const char* GetMessage();
        public: virtual const char* GetName();
        private: int GetMessageL(lua_State * L);
        private: int SetMessageL(lua_State * L);

        /* Automatic dispatchers for overloaded methods */

        /* Automatic property getter-setter declarations */

        /* Binding variables */
        public: static const char *ClassName;
        public: static const char *Inherits;
        public: static const Bindings<InterpreterException>::FunctionType Functions[];
        public: static const Bindings<InterpreterException>::PropertyType Properties[];
        public: static int (__cdecl *Dispatcher)(lua_State * L);
    };
} 

#endif

