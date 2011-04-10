#include "../../Monocle.h"
#include "../../TextureAsset.h"
#include "Puppet.h"

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

	class TestScene : public Scene
	{
	public:
		void Begin();
		void End();
        
        void Update();
        
    private:
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
        
        Timeline *timeline;
	};
}