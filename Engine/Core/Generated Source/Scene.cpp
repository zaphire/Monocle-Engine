/*
 * This file was automatically generated by AutoBind.
 * Any changes made to this file will be lost when it
 * is next regenerated.
 */

#include "autobind/types.h"
#include "autobind/binding/lua.h"
#include "Object.h"
#include "Scene.h"
#include "InternalConstructorOnlyException.h"

namespace Engine
{
    /* Variable assignments */

    /* Method and constructor definitions */
    Scene::Scene(lua_State * L, bool byuser)
    {
        if (byuser)
		{
			throw new Engine::InternalConstructorOnlyException();
		}
		// TODO: Implement.
	}

    void Scene::Begin()
    {
        }

    void Scene::Update()
    {
        }

    void Scene::Render()
    {
        }

    void Scene::End()
    {
        }

    /* Automatic dispatchers for overloaded methods */
    /* Automatic property getter-setter definitions */
    /* Binding variables */
    const char* Scene::ClassName = "Engine.Scene";
    const char* Scene::Inherits = "Object";
    const Bindings<Scene>::FunctionType Scene::Functions[] =
    {
        {0}
    };
    const Bindings<Scene>::PropertyType Scene::Properties[] =
    {
        {0}
    };
    int (__cdecl *Scene::Dispatcher)(lua_State * L) = &(Bindings<Scene>::FunctionDispatch);
} 

