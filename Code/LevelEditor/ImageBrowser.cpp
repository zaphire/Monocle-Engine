#include "ImageBrowser.h"
#include "../Input.h"

namespace Monocle
{
	SelectionImage::SelectionImage(const std::string &image, int width, int height)
		: Entity()
	{
	}

	void SelectionImage::Update()
	{
		Entity::Update();
	}

	ImageBrowser::ImageBrowser()
		: Entity(), hasContent(false)
	{
	}

	void ImageBrowser::Update()
	{
		if (hasContent)
		{
			for (int key = (int)KEY_A; key <= (int)KEY_Z; key++)
			{
				if (Input::IsKeyPressed((KeyCode)key))
				{
					// select next image with name starting with first letter "key"
					// i.e. if key A is pressed selected Aardvark
					// if key A is pressed again, select Aardvark2
				}
			}
		}
	}

	void ImageBrowser::ScanDirectory(const std::string &directory)
	{
		// for each file
			// get file name and type
			// if type is some kind of image type that we support
				// create a SelectionImage
				// add it to the grid/list
	}
}
