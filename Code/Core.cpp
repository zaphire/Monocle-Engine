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
		scene			= NULL;
		switchTo		= NULL;
		switchScenes	= false;
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

		Scene* s = new Scene();
		setScene(s);
		Entity* newEntity = (Entity*)new Player();
		s->Add(newEntity);
#endif

		bool isDone = false;
		while (!isDone)
		{
			//Switch scenes if necessary
			if (switchScenes)
			{
				switchScenes = false;
				if (scene != NULL)
					scene->End();
				scene = switchTo;
				switchTo = NULL;
				if (scene != NULL)
					scene->Begin();
			}

			// **** BEGIN UPDATE
			if (scene != NULL)
				scene->Update();
			// **** END UPDATE

			// **** BEGIN RENDER
			graphics.BeginFrame();
			
			// iterate through all the entities/gameObjects
			// call render on them
			if (scene != NULL)
				scene->Render();
			
			graphics.EndFrame();

			graphics.ShowBuffer();
			// **** END RENDER
		}
	}

	void Core::setScene(Scene* scene)
	{
		switchTo = scene;
		switchScenes = true;
	}

	Scene* Core::getScene()
	{
		return scene;
	}
}