#include <Monocle.h>
#include <Puppet/PuppetEditor.h>

using namespace Monocle;

namespace PuppetTest
{
	class TestScene : public Scene
	{
	public:
		void Begin();
		void End();
        
        void Update();

	protected:
		PuppetEditor *puppetEditor;
	};
}
