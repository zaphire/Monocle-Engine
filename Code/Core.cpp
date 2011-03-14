#include "Core.h"

#define TEST_GRAPHICS
//#define SCENE_CODE_WRITTEN

namespace Monocle
{
	Core::Core()
	{
	}

	void Core::Init()
	{
		platform.Init();
		debug.Init();
		input.Init();
		graphics.Init();
	}

	void Core::Main()
	{
#ifdef TEST_GRAPHICS
		graphics.SetCameraPosition(Vector3(0,0,-6));
#endif

		bool isDone = false;
		while (!isDone)
		{
			//Debug::Log("Core::Main...");

			// **** BEGIN UPDATE
			scene.Update();
			// **** END UPDATE

			// **** BEGIN RENDER
			graphics.BeginFrame();
			
				// iterate through all the entities/gameObjects
				// call render on them
			scene.Render();

#ifdef TEST_GRAPHICS
			graphics.RenderQuad(0.5f);
#endif
			
			graphics.EndFrame();

			graphics.ShowBuffer();
			// **** END RENDER

			
		}
	}
}