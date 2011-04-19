#pragma once

#include "../Entity.h"

namespace Monocle
{
    class SelectionImage : public Entity
    {
    public:
        SelectionImage(const std::string &image, int size);
	void Update();
    };

	class ImageBrowser : public Entity
	{
	public:
	    ImageBrowser();
        void Update();
        //void Render();
		void ScanDirectory(const std::string &directory);
	
	private:
		static void FileCallback(const std::string &filename, intptr_t pointer);
		void NewSelectionImage(const std::string &filename);
		
		int selectionImageSize;
		bool hasContent;
		std::list<SelectionImage*> selectionImages;
		Entity *grid;
	};
}
