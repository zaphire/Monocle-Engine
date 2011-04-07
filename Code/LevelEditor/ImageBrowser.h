#pragma once

#include "../Entity.h"

namespace Monocle
{
        class SelectionImage : public Entity
        {
        public:
        	SelectionImage(const std::string &image, int width, int height);
		void Update();
        };

	class ImageBrowser : public Entity
	{
	public:
	        ImageBrowser();
                void Update();
                void Render();
		void ScanDirectory(const std::string &directory);
	private:
		bool hasContent;
	};
}
