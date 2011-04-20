#include "../../Monocle.h"
#include "../../TextureAsset.h"
#include "Puppet.h"
#include "../../Editor.h"

using namespace Monocle;

namespace PuppetTest
{
    class Timeline : public Entity
    {
    public:
        Timeline();
        Animation *currentAnimation;
        void Render();
    };
    
	class PuppetEntity : public Entity
	{
	public:
		PuppetEntity();
		void Update();

		Puppet puppet;
	};

	class PuppetEditor : public CameraEditor
	{
	public:
		PuppetEditor();
		void Init(Scene *scene);
		void Update();

		PuppetEntity *puppetEntity;

		KeyCode keyTogglePause;

		KeyCode keyMoveLeft;
		KeyCode keyMoveRight;
		KeyCode keyMoveUp;
		KeyCode keyMoveDown;

		KeyCode keyRotateLeft;
		KeyCode keyRotateRight;

		KeyCode keyBackwards;
		KeyCode keyForwards;

		KeyCode keySetKeyFrame;

		KeyCode keyZero;

		KeyCode keyOffset;

		Timeline *timeline;

		Scene *scene;
	};

	class TestScene : public Scene
	{
	public:
		void Begin();
		void End();
        
        void Update();

	protected:
		PuppetEditor puppetEditor;
	};
}