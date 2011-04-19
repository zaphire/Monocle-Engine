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
		void FileCallback(const std::string &filename, void* pointer);	
		void NewSelectionImage(const std::string &filename);
		
		int selectionWidth, selectionHeight;
		bool hasContent;
		std::list<SelectionImage*> selectionImages;
	};
}
