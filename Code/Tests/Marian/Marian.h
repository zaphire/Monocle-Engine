#include "../../Monocle.h"
#include "../../TextureAsset.h"

using namespace Monocle;

namespace Marian
{

	class Sprite : public Entity
	{
	public:
		Sprite();
		void Render();
		TextureAsset *texture;
	};

	class TitleScene : public Scene
	{
	public:
		void Begin();
		void End();
	};

}