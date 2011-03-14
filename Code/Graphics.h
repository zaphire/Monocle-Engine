#pragma strict

namespace Monocle
{
	class Graphics
	{
	public:
		void Init();
		bool SetResolution(int w, int h, int bits, bool full);

		void Render();
		void ShowBuffer();
	};

}