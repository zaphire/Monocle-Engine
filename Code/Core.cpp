#include "Core.h"

#define TEST_ENTITY
//#define SCENE_CODE_WRITTEN

namespace Monocle
{

#ifdef TEST_ENTITY
	class Player : Entity
	{
	public:

		void Update()
		{
			Entity::Update();
			position.x += 0.01f;
		}

		void Render()
		{
			Entity::Render();
			Graphics::Translate(position);
			Graphics::RenderQuad(0.5f);
		}
	};
#endif

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
#ifdef TEST_ENTITY
		graphics.SetCameraPosition(Vector3(0,0,-6));

		Entity* newEntity = (Entity*)new Player();
		scene.Add(newEntity);
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
			
			graphics.EndFrame();

			graphics.ShowBuffer();
			// **** END RENDER
		}
	}
}