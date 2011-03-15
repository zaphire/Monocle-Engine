/*
 * This file was automatically generated by AutoBind.
 * Any changes made to this file will be lost when it
 * is next regenerated.
 */

#include "autobind/types.h"
#include "autobind/binding/lua.h"
#include "Object.h"
#include "Core.h"
#include "InternalConstructorOnlyException.h"
#include "Input.h"
#include "Graphics.h"
#include "Debug.h"
#include "Platform.h"
#include "Scene.h"

namespace Engine
{
    /* Variable assignments */

    /* Method and constructor definitions */
    Core::Core(lua_State * L, bool byuser)
    {
        // These settings are configurable by the user at runtime.

		this->p_Platform = NULL;
		this->p_Input = NULL;
		this->p_Graphics = NULL;
		this->p_Debug = NULL;

		this->p_Scene = NULL;
		this->p_SwitchTo = NULL;
		this->p_DeltaTime = 0.0;		this->p_FixedTimestep = false;
		this->p_TargetFrames = 60;
	}

	/// <summary>
	/// Initalizes the Monocle engine's subcomponents.
	/// </summary>
    int Core::Init(lua_State * L)
    {
        this->p_Platform = new Platform();
		this->p_Input = new Input();
		this->p_Graphics = new Graphics();
		this->p_Debug = new Debug();
	}

	/// <summary>
	/// Executes the main game loop after you've added all the scenes
	/// and set the initial scene.
	/// </summary>
    int Core::Main(lua_State * L)
    {
        bool isDone = false;
		long lastTick = Platform::GetMilliseconds();
		long tick = 0.0f;

		while (!isDone)
		{
			// Switch scenes if needed.
			if (this->p_SwitchTo != NULL)
			{
				if (this->p_Scene != NULL)
					this->p_Scene->End();
				this->p_Scene = this->p_SwitchTo;
				this->p_SwitchTo = NULL;
				if (this->p_Scene != NULL)
					this->p_Scene->Begin();
			}

			// Calculate the amount of time that has passed since the last frame.


			// Perform an update on the scene.
			tick = Platform::GetMilliseconds();
			this->p_DeltaTime = ((double)(tick - lastTick))/1000.0;			if (this->p_FixedTimestep)
			{
				if ((tick - lastTick) >= this->p_TargetFrames)
				{
					if (this->p_Scene != NULL)
						this->p_Scene->Update();
					lastTick = tick;
				}
			}
			else
			{
				if (this->p_Scene != NULL)
					this->p_Scene->Update();
				lastTick = tick;
			}

			// Perform a render of the scene.
			this->p_Graphics->BeginFrame();
			if (this->p_Scene != NULL)
				this->p_Scene->Render();
			this->p_Graphics->EndFrame();
			this->p_Graphics->ShowBuffer();
		}
	}

	/// <summary>
	/// The current scene.
	/// </summary>


	/// <summary>
    /* Automatic dispatchers for overloaded methods */
    /* Automatic property getter-setter definitions */
    int Core::__autobind_property_get_Scene(lua_State * L)
    {
        return Bindings<numeric>::Result(L, p_Scene);
    }

    int Core::__autobind_property_set_Scene(lua_State * L)
    {
        p_SwitchTo = Bindings<numeric>::GetArgument(L, -1);
        return Bindings<numeric>::EmptyResult;
    }

    int Core::__autobind_property_get_DeltaTime(lua_State * L)
    {
        return Bindings<numeric>::Result(L, p_DeltaTime);
    }

    int Core::__autobind_property_get_FixedTimestep(lua_State * L)
    {
        return Bindings<numeric>::Result(L, p_FixedTimestep);
    }

    int Core::__autobind_property_set_FixedTimestep(lua_State * L)
    {
        p_FixedTimestep = Bindings<numeric>::GetArgument(L, -1);
        return Bindings<numeric>::EmptyResult;
    }

    int Core::__autobind_property_get_TargetFrames(lua_State * L)
    {
        return Bindings<numeric>::Result(L, p_TargetFrames);
    }

    int Core::__autobind_property_set_TargetFrames(lua_State * L)
    {
        p_TargetFrames = Bindings<numeric>::GetArgument(L, -1);
        return Bindings<numeric>::EmptyResult;
    }

    /* Binding variables */
    const char* Core::ClassName = "Engine.Core";
    const char* Core::Inherits = "Object";
    const Bindings<Core>::FunctionType Core::Functions[] =
    {
        {"Init", &Core::Init},
        {"Main", &Core::Main},
        {0}
    };
    const Bindings<Core>::PropertyType Core::Properties[] =
    {
        {"Scene", &Core::__autobind_property_get_Scene, &Core::__autobind_property_set_Scene},
        {"DeltaTime", &Core::__autobind_property_get_DeltaTime, NULL},
        {"FixedTimestep", &Core::__autobind_property_get_FixedTimestep, &Core::__autobind_property_set_FixedTimestep},
        {"TargetFrames", &Core::__autobind_property_get_TargetFrames, &Core::__autobind_property_set_TargetFrames},
        {0}
    };
    int (__cdecl *Core::Dispatcher)(lua_State * L) = &(Bindings<Core>::FunctionDispatch);
} 

