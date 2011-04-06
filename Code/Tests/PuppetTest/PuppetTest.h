#include "../../Monocle.h"
#include "../../TextureAsset.h"
#include "Puppet.h"

using namespace Monocle;

namespace PuppetTest
{
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
	};
}