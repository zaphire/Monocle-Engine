// This file registers all of the available built-in classes
// with the engine.
#include "ClassRegistration.h"
#include "autobind/types.h"
#include "autobind/binding/lua.h"

// Include all of the classes to be registered.
#include "Asset.h"
#include "Core.h"
#include "Debug.h"
#include "Entity.h"
#include "Graphics.h"
#include "Input.h"
#include "Math.h"
#include "Platform.h"
#include "Scene.h"
#include "TextureAsset.h"
#include "Vector2.h"
#include "Vector3.h"

#include "AccessViolationException.h"
#include "ArgumentCountMismatchException.h"
#include "ArgumentTypeNotValidException.h"
#include "ContextNotProvidedException.h"
#include "DataMisalignedException.h"
#include "DebuggerNotAttachedException.h"
#include "DivideByZeroException.h"
#include "EngineException.h"
#include "EntryPointNotFoundException.h"
#include "Exception.h"
#include "GraphicsEngineNotFoundException.h"
#include "IllegalInstructionException.h"
#include "InheritedClassNotFoundException.h"
#include "InternalConstructorOnlyException.h"
#include "InterpreterException.h"
#include "InterpreterStateNotValidException.h"
#include "InvalidObjectThrownException.h"
#include "InvalidOperationException.h"
#include "InvalidSyntaxException.h"
#include "KeyNotFoundException.h"
#include "LowLevelException.h"
#include "NilReferenceException.h"
#include "ObjectDisposedException.h"
#include "OutOfMemoryException.h"
#include "OverflowException.h"
#include "PathNotFoundException.h"
#include "PermissionDeniedException.h"
#include "PrivilegedInstructionException.h"
#include "ReadOnlyPropertyException.h"
#include "UnderflowException.h"

namespace Monocle
{
	void RegisterAllClasses(lua_State * L)
	{
		Bindings<Engine::Asset>::Register(L);
		Bindings<Engine::Core>::Register(L);
		Bindings<Engine::Debug>::Register(L);
		Bindings<Engine::Entity>::Register(L);
		Bindings<Engine::Graphics>::Register(L);
		Bindings<Engine::Input>::Register(L);
		Bindings<Engine::Math>::Register(L);
		Bindings<Engine::Platform>::Register(L);
		Bindings<Engine::Scene>::Register(L);
		Bindings<Engine::TextureAsset>::Register(L);
		Bindings<Engine::Vector2>::Register(L);
		Bindings<Engine::Vector3>::Register(L);

		// Register the exception classes.  Please note that if they
		// aren't registered when they are fired, they won't be properly
		// accessible because the RaiseException function of the Lua
		// bindings relies on the entry being in the Lua registry.
		Bindings<Engine::AccessViolationException>::Register(L);
		Bindings<Engine::ArgumentCountMismatchException>::Register(L);
		Bindings<Engine::ArgumentTypeNotValidException>::Register(L);
		Bindings<Engine::ContextNotProvidedException>::Register(L);
		Bindings<Engine::DataMisalignedException>::Register(L);
		Bindings<Engine::DebuggerNotAttachedException>::Register(L);
		Bindings<Engine::DivideByZeroException>::Register(L);
		Bindings<Engine::EngineException>::Register(L);
		Bindings<Engine::EntryPointNotFoundException>::Register(L);
		Bindings<Engine::Exception>::Register(L);
		Bindings<Engine::GraphicsEngineNotFoundException>::Register(L);
		Bindings<Engine::IllegalInstructionException>::Register(L);
		Bindings<Engine::InheritedClassNotFoundException>::Register(L);
		Bindings<Engine::InternalConstructorOnlyException>::Register(L);
		Bindings<Engine::InterpreterException>::Register(L);
		Bindings<Engine::InterpreterStateNotValidException>::Register(L);
		Bindings<Engine::InvalidObjectThrownException>::Register(L);
		Bindings<Engine::InvalidOperationException>::Register(L);
		Bindings<Engine::InvalidSyntaxException>::Register(L);
		Bindings<Engine::KeyNotFoundException>::Register(L);
		Bindings<Engine::LowLevelException>::Register(L);
		Bindings<Engine::NilReferenceException>::Register(L);
		Bindings<Engine::ObjectDisposedException>::Register(L);
		Bindings<Engine::OutOfMemoryException>::Register(L);
		Bindings<Engine::OverflowException>::Register(L);
		Bindings<Engine::PathNotFoundException>::Register(L);
		Bindings<Engine::PermissionDeniedException>::Register(L);
		Bindings<Engine::PrivilegedInstructionException>::Register(L);
		Bindings<Engine::ReadOnlyPropertyException>::Register(L);
		Bindings<Engine::UnderflowException>::Register(L);
	}
}