#include "../../Monocle.h"
#include "../../TextureAsset.h"
#include "../../Tween.h"
#include "../../Color.h"

using namespace Monocle;

namespace Marian
{

	class LevelEditor : public Editor
	{
	public:
		LevelEditor();
		void Update();
		bool isOn;
		Entity *selectedEntity;
	};

	class Sprite : public Entity
	{
	public:
		Sprite();
		void Render();
		TextureAsset *texture;
		Color color;
		float scale;
	};

	class TitleScene : public Scene
	{
	public:
		void Begin();
		void End();
		void Update();
	};

}